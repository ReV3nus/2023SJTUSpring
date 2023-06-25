import React, {useEffect, useState} from 'react';
import {Button, Empty, message, Space, Table} from "antd";
import * as bookService from "../Services/bookService";
import axios from "axios";
const CartList = () => {
    const columns = [
        {
            title: '书名',
            dataIndex: 'bookname',
            key: 'bookname',
        },
        {
            title: '作者',
            dataIndex: 'author',
            key: 'author',
        },
        {
            title: '类型',
            dataIndex: 'type',
            key: 'type',
        },
        {
            title: '数量',
            dataIndex: 'count',
            key: 'count',
        },
        {
            title: '价格',
            dataIndex: 'price',
            key: 'price',
        },
    ];
    const [carts, setCarts] = useState([]);
    const [messageApi, contextHolder] = message.useMessage();
    const getCart = () => {
        axios.get('/apiCart/getCart')
            .then(response => {
                setCarts(response.data);
            });
    }
    useEffect(() => {
        getCart();
    }, []);
    if (carts.length === 0) {
        return (<Empty />)
    }
    return (
        <div>
            {contextHolder}
            <Table columns={columns} dataSource={carts} id='CartList' />
            <Space wrap>
                <Button type="primary" onClick={() => {
                    bookService.CartPurchase(messageApi);
                    setTimeout(() => {
                        getCart();
                    }, 3000);
                }}>立即购买</Button>
                <Button onClick={() => {
                    bookService.CartClear(messageApi);
                    setTimeout(() => {
                        getCart();
                    }, 1000);
                }}>清空购物车</Button>
            </Space>
        </div>
    )
};
export default CartList;