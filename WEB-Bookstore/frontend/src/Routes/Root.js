import React from 'react';
import {Layout, Menu, Space} from 'antd';
import SiteLogo from '../Components/SiteLogo'
import UserAvatar from "../Components/UserAvatar";
import {
    AccountBookOutlined,
    ShoppingCartOutlined,
    ReadOutlined,
    ProfileOutlined,
    PieChartOutlined, PartitionOutlined
} from '@ant-design/icons';
import { Outlet,Link} from "react-router-dom";
import '../CSS/RootCSS.css'
import axios from "axios";

const { Header ,Content,Footer} = Layout;



class Root extends React.Component{

    constructor(props) {
        super(props);
        this.state= {
            username:"",
            usertype:"",
            isLoadingName:true,
            isLoadingType:true,
            isAuth:false,
        }
    }
    componentDidMount() {
        axios.get('/apiAccount/checkLogin')
            .then(res => {
                if (res.status >= 0) {
                    this.setState(() => ({
                        username:res.data,
                        isLoadingName:false,
                        isAuth:true,
                    }));
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
            .catch(err=>{
                this.setState(()=>({
                    isLoadingName:false,
                    isAuth:false,
                }))
            })
    }

    render(){
        if(this.state.isLoadingName)return (<></>);
        return(
            <div id="Root">
                <Layout>
                    <Header id="RootHeader">
                        <div id="SiteLogo">
                            <SiteLogo/>
                        </div>
                        <div id="UserAvatar">
                            <UserAvatar username={this.state.username} usertype={this.state.usertype} UserIcon={this.props.UserIcon}/>
                        </div>
                        <Menu
                            theme="dark"
                            mode="horizontal"
                            defaultSelectedKeys={['1']}
                        >
                            <Menu.Item key="1">
                                <Link to="../Home">
                                    <ReadOutlined />
                                    <span>Books</span>
                                    </Link>
                            </Menu.Item>
                            <Menu.Item key="2">
                                <Link to="../Cart">
                                    <ShoppingCartOutlined />
                                    <span>My Cart</span>
                                </Link>
                            </Menu.Item>
                            <Menu.Item key="3">
                                <Link to="../Orders">
                                    <AccountBookOutlined />
                                    <span>My Orders</span>
                                </Link>
                            </Menu.Item>
                            <Menu.Item key="4">
                                <Link to="../Profile">
                                    <ProfileOutlined />
                                    <span>My Profile</span>
                                </Link>
                            </Menu.Item>
                            <Menu.Item key="5">
                                <Link to="../Statistic">
                                    <PieChartOutlined/>
                                    <span>Statistics and ranking</span>
                                </Link>
                            </Menu.Item>
                            {(this.state.usertype==="Admin")&&
                                <Menu.Item key="6">
                                    <Space>
                                        <PartitionOutlined />
                                        <Link to="../UserManager">
                                            <span style={{color:'red',}}>User Manager</span>
                                        </Link>
                                    </Space>
                                </Menu.Item>}
                        </Menu>
                    </Header>
                    <Content id="RootContent">
                        <Outlet />
                    </Content>
                    <Footer id="RootFooter">
                        My Bookstore Webpage
                    </Footer>
                </Layout>
            </div>
        );
    }
}
export default Root;