import React, {useEffect, useState} from 'react';
import BreadCrumb from "../Components/BreadCrumb";
import '../CSS/StatisticCSS.css'
import {DatePicker, Space, Spin} from 'antd';
import {Navigate} from "react-router-dom";
import axios from "axios";
import AdminRankings from "../Components/AdminRankings";
import PurchaseRanking from "../Components/PurchaseRanking";
import SelfRanking from "../Components/SelfRanking";
const StatisticView =(props)=>{
    const [isAuth,setIsAuth]=useState(false);
    const [isLoading,setIsLoading]=useState(true);
    const [usertype,setUsertype]=useState('');
    const [orders,setOrders]=useState([]);
    const [selectedDate,SetSelectedDate]=useState(null);
    const [filteredOrders,setFilteredOrders]=useState([]);
    const [filteredOrderItems1,setFilteredOrderItems1]=useState([]);
    const [filteredOrderItems2,setFilteredOrderItems2]=useState([]);
    const [filteredOrderItems3,setFilteredOrderItems3]=useState([]);
    const { RangePicker } = DatePicker;
    const [scannedOrders,setScannedOrders]=useState(0);
    const onChange = (value, dateString) => {
        SetSelectedDate(value);
        // console.log('Selected Time: ', value);
        // console.log('Formatted Selected Time: ', dateString);
    };
    const onOk = (value) => {
        // console.log('onOk: ', value);
    };
    useEffect(()=>{
        axios.get('/apiAccount/checkLogin')
            .then(res => {
                if (res.status >= 0) {
                    setIsAuth(true);
                    axios.get('/apiAccount/checkAdmin')
                        .then(res => {
                            if (res.status >= 0) {
                                setUsertype(res.data);
                                axios.get('/apiOrder/getOrderIds')
                                    .then(response => {
                                        setOrders(response.data);
                                        setIsLoading(false);
                                    })
                                    .catch(err=>{
                                        console.log(err);
                                        setIsLoading(false);
                                    })
                            }
                        })
                        .catch(err=>{
                            console.log(err);
                            setIsLoading(false);
                        })
                }
            })
            .catch(err => {
                console.log('err:', err);
                setIsAuth(false);
                setIsLoading(false);
            });
    },[])
    useEffect(()=>{
        setFilteredOrders([]);
        setFilteredOrderItems1([]);
        setFilteredOrderItems2([]);
        setFilteredOrderItems3([]);
        setScannedOrders(orders.length);
        orders.map((order)=>{
            const orderTime=order.time;
            if(selectedDate===null||
                ((selectedDate[0].isBefore(orderTime)||selectedDate[0].isSame(orderTime))
                    &&(selectedDate[1].isAfter(orderTime)||selectedDate[1].isSame(orderTime)))){
                setFilteredOrders(prevOrders => [...prevOrders, order]);
                axios.get('/apiOrder/getOrderItems',{params:{oid:order.recordId}})
                    .then(
                        response=>{
                            setFilteredOrderItems1(prevItems=>[...prevItems,...response.data]);
                            setFilteredOrderItems2(prevItems=>[...prevItems,...response.data]);
                            setFilteredOrderItems3(prevItems=>[...prevItems,...response.data]);
                            setScannedOrders(prevState => prevState-1);
                        }
                    )
                    .catch(err =>{
                        console.log(err);
                        setScannedOrders(prevState => prevState-1);
                    });
            }
            else{
                setScannedOrders(prevState => prevState-1);
            }
        })
    },[orders,selectedDate]);
    if (isLoading) {
        return(
            <Spin tip="Loading" size="small" id='SpinCSS'>
                <div className="content" />
            </Spin>
        );
    }
    if(!isAuth)
    {
        return(<Navigate to='/Login'/>);
    }
    if(scannedOrders>0)
    {
        return(
            <Spin tip="Loading" size="small" id='SpinCSS'>
                <div className="content" />
            </Spin>
        );
    }
    return (
        <>
            <BreadCrumb paths={["Home","Profile"]}/>
            <div id='pageDiv'>
                <Space align='center' className= "StaSpace" direction={"vertical"}>
                    <RangePicker
                        showTime={{
                            format: 'HH:mm',
                        }}
                        format="YYYY-MM-DD HH:mm"
                        onChange={onChange}
                        onOk={onOk}
                        placeholder={['起始时间', '截止时间']}
                    />
                    {usertype==="Admin"&&(<AdminRankings items={filteredOrderItems1}/>)}
                    {usertype==="Admin"&&(<PurchaseRanking items={filteredOrderItems2}/>)}
                    {usertype==="User"&&(<SelfRanking items={filteredOrderItems3}/>)}
                </Space>
            </div>
        </>
    )
}
export default StatisticView;