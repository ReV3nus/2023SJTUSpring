import React from 'react';
import BreadCrumb from '../Components/BreadCrumb';
import HomeBooksCarousel from "../Components/HomeBooksCarousel";
import BookGrids from '../Components/BookGrids'
import '../CSS/HomeCSS.css'
import {Space, Spin} from "antd";
import {Navigate} from "react-router-dom";
import axios from "axios";
import Search from "antd/es/input/Search";
import BookAdder from "../Components/BookAdder";


class HomeView extends React.Component{

    constructor(props) {
        super(props);
        this.state={
            isAuth:false,
            isLoading:true,
            books:[],
            filterKeyword:'',
            usertype:'',
            isLoadingType:true,
            FilteredBooks:[],
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
                        console.log(res);
                    });
                    axios.get('/apiAccount/checkAdmin')
                        .then(res => {
                            if (res.status >= 0) {
                                this.setState(() => ({
                                    usertype: res.data,
                                    isLoadingType: false,
                                }));
                            }
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
        axios.get("/apiBook/getbooks")
            .then(response => {
                this.setState({ books: response.data });
                this.setState({ FilteredBooks: response.data});
            })
            .catch(error => {
                console.log(error);
            });
    }
    render(){
        const { isLoading, isAuth , usertype, isLoadingType} = this.state;
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
        if(isLoadingType) {
            // 如果仍在等待响应，则返回一个loading状态
            return(
                <Spin tip="Loading" size="small" id='SpinCSS'>
                    <div className="content" />
                </Spin>
            );
        }
        return(
            <>
                <BreadCrumb/>
                <div id="pageDiv">
                    <Space direction='vertical' id='HomePageSpace' size='large' className="HomeSpace" align='center'>
                        <HomeBooksCarousel/>
                        <Search
                            placeholder="输入书名以搜索"
                           // value={this.state.filterKeyword}
                            onSearch={(value)=>{
                                this.setState({
                                    FilteredBooks:this.state.books.filter((book) =>
                                        book.name.includes(value))
                                })}
                            }
                            enterButton
                            allowClear
                            style={{width: 400,}}
                        />
                        {(this.state.usertype==="Admin")&&(
                            <BookAdder/>
                        )}
                    </Space>
                    {(this.state.usertype==="Admin")&&(
                        <><br/><br/></>
                    )}
                        <BookGrids books={this.state.FilteredBooks} usertype={usertype}/>
                </div>
            </>
        );
    }
}
export default HomeView;