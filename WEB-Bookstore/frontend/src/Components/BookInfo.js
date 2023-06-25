import React, {useState} from "react";
import {Button, Descriptions, message, Space} from "antd";
// import axios from "axios";
import * as bookService from "../Services/bookService";
import '../CSS/DetailCSS.css'
const BookInfo = (props) => {
    const [Book] = useState(props.Book);
    const [messageApi, contextHolder] = message.useMessage();

    return (
        <Space id='detailSpace'>
            {contextHolder}
            <img alt={Book.name} src={Book.image} width={'300px'}/>
            <br/>
            <Space direction={"vertical"} align={"center"}>
                <Descriptions title="书籍信息" column={8} layout="vertical" bordered>
                    <Descriptions.Item label="书籍名" span={4}>
                        {Book.name}
                    </Descriptions.Item>
                    <Descriptions.Item label="作者" span={2}>
                        {Book.author}
                    </Descriptions.Item>
                    <Descriptions.Item label="类型" span={2}>
                        {Book.type}
                    </Descriptions.Item>
                    <Descriptions.Item label="描述信息" span={6}>
                        {Book.description}
                    </Descriptions.Item>
                    <Descriptions.Item label="isbn" span={2}>
                        {Book.isbn}
                    </Descriptions.Item>
                    <Descriptions.Item label="价格" span={4}>
                        {`￥`+Book.price}
                    </Descriptions.Item>
                    <Descriptions.Item label="存量" span={4}>
                        {Book.inventory}
                    </Descriptions.Item>
                </Descriptions>
                <Space wrap >
                    <Button type="primary" onClick={()=>{
                        const BookId=parseInt(props.BookID.toString(),10);
                        bookService.AddToOrder(BookId,messageApi);
                    }}>立即购买</Button>
                    <Button onClick={() =>{
                        const BookId=parseInt(props.BookID.toString(),10);
                        bookService.AddToCart(BookId,messageApi);
                    }}>添加到购物车</Button>
                </Space>
            </Space>
        </Space>
    );
}
export default BookInfo;