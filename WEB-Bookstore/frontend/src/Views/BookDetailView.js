import React, {useEffect, useRef, useState} from "react";
import {useLoaderData} from "react-router";
import BreadCrumb from "../Components/BreadCrumb";
import '../CSS/DetailCSS.css'
import {Spin} from 'antd';
import axios from "axios";
import {Navigate} from "react-router-dom";
import BookInfo from "../Components/BookInfo";
export default function BookDetailView(){
    const BookID=useLoaderData();
    const [isAuth, setIsAuth] = useState(false);
    const [isLoading,setIsLoading] = useState(true);
   // let Book;
    const Book = useRef(null);
    useEffect(() => {
        axios.get('/apiAccount/checkLogin')
            .then(res => {
                if (res.status >= 0) {
                    setIsAuth(true);
                    setIsLoading(true);
                }
            })
            .catch(err => {
                console.log('err:', err);
                setIsAuth(false);
                setIsLoading(true);
            });
        axios.get("/apiBook/getbook", {params:{id:BookID}})
            .then(res => {
                Book.current=res.data;
                setIsLoading(false);
            })
            .catch(err =>{
                console.log(err);
            });
    },[BookID]);
    if (isLoading) {
        // 如果仍在等待响应，则返回一个loading状态
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
    return (
        <>
            <BreadCrumb/>
            <div id="pageDiv">
                <BookInfo Book={Book.current} BookID={BookID}/>
            </div>
        </>
    );
}