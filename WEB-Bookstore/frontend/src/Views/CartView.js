import React from 'react';
import BreadCrumb from '../Components/BreadCrumb';
import '../CSS/CartCSS.css'
import {Space, Spin} from "antd";
import SearchBar from "../Components/SearchBar";
import {Navigate} from "react-router-dom";
import axios from "axios";
import CartList from "../Components/CartList";

class CartView extends React.Component{
    constructor(props) {
        super(props);
        this.state={
            isAuth:false,
            isLoading:true,
        };
    }

    componentDidMount() {
        axios.get('/apiAccount/checkLogin')
            .then(res => {
                if (res.status >= 0) {
                    this.setState(() => ({
                        isAuth: true,
                        isLoading:false,
                    }), () => {
                        console.log(this.state.isAuth);
                    });
                }
            })
            .catch(err => {
                console.log('err:', err);
                this.setState({
                    isAuth: false ,
                    isLoading:false,
                }, () => {
                    console.log(this.state.isAuth);
                });
            });
    }
    render()
    {
        const { isLoading, isAuth } = this.state;
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
                <div id='CartPageDiv'>
                    <Space align='center' direction='vertical' className="CartSpace">
                        <h1>我的购物车</h1>
                        <SearchBar/>
                        <CartList/>

                    </Space>
                </div>
            </>
        )
    }
}
export default CartView;