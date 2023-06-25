import React, {useEffect, useState} from 'react';
import BreadCrumb from '../Components/BreadCrumb';
import '../CSS/OrderCSS.css'
import {Button, Card, Space, Spin, Table} from "antd";
import {Navigate} from "react-router-dom";
import axios from "axios";

const UMView = () => {
    const [isAuth, setIsAuth] = useState(false);
    const [isLoading, setIsLoading] = useState(true);
    const [isLoadingType, setIsLoadingType] = useState(true);
    const [usertype, setUsertype] = useState("");
    const [users, setUsers] = useState([]);
    const columns = [
        {
            title: '用户名',
            dataIndex: 'username',
            key:'username',
        },
        {
            title: '密码',
            dataIndex: 'password',
            key:'password',
        },
        {
            title: '用户状态',
            dataIndex: 'usertype',
            key:'usertype',
        },
        {
            title:'操作',
            key:'action',
            render: (_, record) => (
                <>
                    {record.usertype==='User'&&<Button size='middle' type="primary" danger onClick={()=>{
                        let username=record.username;
                        let usertype="Banned";
                        axios.post('/apiAccount/setstate',{username,usertype})
                            .then(res=>{
                                axios.get('/apiAccount/getUsers')
                                    .then(response => {
                                        setUsers(response.data);
                                    });
                                console.log(res);
                            })
                            .catch(err=>{
                                console.log(err);
                            })
                    }}>封禁</Button>}
                    {record.usertype==='Banned'&&<Button size='middle' type="primary" onClick={()=>{
                        let username=record.username;
                        let usertype="User";
                        axios.post('/apiAccount/setstate',{username,usertype})
                            .then(res=>{
                                axios.get('/apiAccount/getUsers')
                                    .then(response => {
                                        setUsers(response.data);
                                    });
                                console.log(res);
                            })
                            .catch(err=>{
                                console.log(err);
                            })
                    }}>解禁</Button>}
                </>
            ),
        },
    ];

    useEffect(() => {
        axios.get('/apiAccount/checkLogin')
            .then(res => {
                if (res.status >= 0) {
                    setIsAuth(true);
                    setIsLoading(false);

                    axios.get('/apiAccount/checkAdmin')
                        .then(res => {
                            if (res.status >= 0) {
                                setUsertype(res.data);
                                setIsLoadingType(false);
                            }

                            axios.get('/apiAccount/getUsers')
                                .then(response => {
                                    setUsers(response.data);
                                })
                                .catch(err => {
                                    console.log(err);
                                });
                        });
                }
            })
            .catch(err => {
                console.log('err:', err);
                setIsAuth(false);
                setIsLoading(false);
            });
    }, []);

    if (isLoading || isLoadingType) {
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

    if (usertype !== "Admin") {
        return <Navigate to='/Home' />;
    }

    return (
        <>
            <BreadCrumb />
            <div id='pageDiv'>
                <Space align='center' direction='vertical' size='large' className="OrderSpace">
                    <Card title={`用户列表`} id="OrderCard">
                        <Table columns={columns} dataSource={users} />
                    </Card>
                </Space>
            </div>
        </>
    );
}
export default UMView;