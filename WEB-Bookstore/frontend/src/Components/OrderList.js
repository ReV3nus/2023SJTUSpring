import React, {useEffect, useState} from 'react';
import {Empty} from "antd";
import OrderCard from "./OrderCard";import {Space } from 'antd';
const OrderList=(props)=>{
    const [filteredOrders,setFilteredOrders]=useState(props.orders);
    useEffect(()=>{
        const date=props.selectedDate;
        setFilteredOrders([]);
        props.orders.map((order)=>{
            const orderTime=order.time;
            if(date===null||
                ((date[0].isBefore(orderTime)||date[0].isSame(orderTime))
                    &&(date[1].isAfter(orderTime)||date[1].isSame(orderTime)))){
                setFilteredOrders(prevOrders => [...prevOrders, order]);
            }})
    },[props.orders,props.selectedDate]);
    if(props.orders.length===0){
        return (<Empty/>)
    }
    return (
        <>
            <Space direction='vertical'>
                {filteredOrders.map(order=>(
                    <OrderCard order={order} filterWord={props.filterWord}/>
                ))}
            </Space>
        </>
    )
}
export default OrderList;