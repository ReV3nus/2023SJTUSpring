import React, {useEffect, useState} from 'react';
import BreadCrumb from '../Components/BreadCrumb';
import '../CSS/OrderCSS.css'
import {DatePicker, Space, Spin} from "antd";
import {Navigate} from "react-router-dom";
import axios from "axios";
import OrderList from "../Components/OrderList";
import Search from "antd/es/input/Search";

const OrderView = () => {
    const [isAuth, setIsAuth] = useState(false);
    const [isLoading, setIsLoading] = useState(true);
    const [orders, setOrders] = useState([]);
    const [filterWord,setFilterWord]=useState('');
    const [selectedDate,SetSelectedDate]=useState(null);
    const { RangePicker } = DatePicker;
    const onChange = (value, dateString) => {
        SetSelectedDate(value);
        // console.log('Selected Time: ', value);
        // console.log('Formatted Selected Time: ', dateString);
    };
    const onOk = (value) => {
        // console.log('onOk: ', value);
    };

    useEffect(() => {
        axios.get('/apiAccount/checkLogin')
            .then(res => {
                if (res.status >= 0) {
                    setIsAuth(true);
                    axios.get('/apiOrder/getOrderIds')
                        .then(response => {
                            setOrders(response.data);
                            console.log('#' + response.data);
                            setIsLoading(false);
                        })
                        .catch(err => {
                            console.log('err:', err);
                            setIsLoading(false);
                        });
                }
            })
            .catch(err => {
                console.log('err:', err);
                setIsAuth(false);
                setIsLoading(false);
            });
    }, []);

    if (isLoading) {
        // 如果仍在等待响应，则返回一个loading状态
        return (
            <div id='pageDiv'>
                <Spin tip="Loading" size="small" id='SpinCSS'>
                    <div className="content" />
                </Spin>
            </div>
        );
    }

    if (!isAuth) {
        return <Navigate to='/Login' />;
    }

    return (
        <>
            <BreadCrumb />
            <div id='pageDiv'>
                <Space align='center' direction='vertical' size='large' className="OrderSpace">
                    <h1>我的订单</h1>
                    <RangePicker
                        showTime={{
                            format: 'HH:mm',
                        }}
                        format="YYYY-MM-DD HH:mm"
                        onChange={onChange}
                        onOk={onOk}
                        placeholder={['起始时间', '截止时间']}
                    />
                    <Search
                        placeholder="输入书名以搜索相关订单"
                        onSearch={(value)=>{
                            setFilterWord(value);
                        }}
                        enterButton
                        allowClear
                        style={{width: 400,}}
                    />
                    <OrderList orders={orders} filterWord={filterWord} selectedDate={selectedDate}/>
                </Space>
            </div>
        </>
    );
};

export default OrderView;