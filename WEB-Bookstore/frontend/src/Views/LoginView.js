import React, {useEffect, useState} from 'react';
import {Alert, Button, Checkbox, Form, Input, Space} from 'antd';
import axios from "axios";
import {Navigate } from "react-router-dom";
import {useNavigate} from "react-router";
import loginBackgroundImage from "../Assets/loginbg.jpg";
// import * as userService from "../Services/userService";
import '../CSS/LoginCSS.css'

const onFinishFailed = (errorInfo) => {
    console.log('Failed:', errorInfo);
};

function LoginView()
{
    const [FormMode,setFormMode] = useState('login');
    const [isAuth, setIsAuth] = useState(false);
    const [errorMessage,setErrorMessage] = useState('');
    const navigate = useNavigate();
    useEffect(() => {
        axios
            .get("/apiAccount/checkLogin")
            .then((res) => {
                setIsAuth(true);
            })
            .catch((err) => {
                setIsAuth(false);
            });
    }, []);
    const onFinish = (values) => {
        const username=values.username;
        const password=values.password;
        console.log('username:',username);
        console.log('password:',password);
        axios.post('/apiAccount/login', {username, password})
            .then(response => {
                console.log('#'+response.data);
                if(response.data==="Banned"){
                    setErrorMessage('该用户已被封禁！');
                    axios.post('/apiAccount/logout')
                        .then();
                }
                else{
                    console.log(response);
                    localStorage.setItem('user', JSON.stringify(values.username));
                    navigate('/');
                }
            })
            .catch(error => {
                    console.log(error);
                setErrorMessage('用户名或密码错误！');
                });
    };
    const registeronFinish = (values) => {
        const username=values.username;
        const password=values.password;
        const password2=values.password2;
        const email=values.mail;
        axios.post('/apiAccount/checkName',{username})
            .then(res=>{
                if(res.data==='YES') {
                    setErrorMessage('用户名已经存在！');
                    return;
                }
            })
            .catch(err=>{console.log(err);})
        if(password!==password2)
        {
            setErrorMessage('两次输入密码不一致！');
            return;
        }
        const emailRegex = /^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$/;
        if(!emailRegex.test(email))
        {
            setErrorMessage('邮箱不符合格式！');
            return;
        }
        axios.post('/apiAccount/register',{username,password,email})
            .then(res=>{
                setErrorMessage('注册成功');
                setFormMode('login');
            })
            .catch(err=>{
                setErrorMessage('注册失败！');
                console.log(err);
                return;
            })
    };

    if(isAuth)
        return(<Navigate to='/Home'/>);

    const loginForm=(
        <>
            <Form
                name="basic"
                labelCol={{
                    span: 8,
                }}
                wrapperCol={{
                    span: 16,
                }}
                style={{
                    maxWidth: 600,
                    alignItems:"center",
                }}
                initialValues={{
                    remember: true,
                }}
                onFinish={onFinish}
                onFinishFailed={onFinishFailed}
                autoComplete="off"
            >
                <Form.Item
                    label="用户名"
                    name="username"
                    rules={[
                        {
                            required: true,
                            message: 'Please input your username!',
                        },
                    ]}
                >
                    <Input />
                </Form.Item>

                <Form.Item
                    label="密码"
                    name="password"
                    rules={[
                        {
                            required: true,
                            message: 'Please input your password!',
                        },
                    ]}
                >
                    <Input.Password />
                </Form.Item>

                <Form.Item
                    name="记住我"
                    valuePropName="checked"
                    wrapperCol={{
                        offset: 8,
                        span: 16,
                    }}
                >
                    <Checkbox>Remember me</Checkbox>
                </Form.Item>

                <Form.Item
                    wrapperCol={{
                        offset: 8,
                        span: 16,
                    }}
                >
                    <Space>
                        <Button type="primary" htmlType="submit">
                            Submit
                        </Button>
                        <Button onClick={()=>{setFormMode('register');setErrorMessage('');}}>
                            Register
                        </Button>
                    </Space>
                </Form.Item>
            </Form>
        </>
    );
    const registerForm=(
        <>
            <Form
                name="basic"
                labelCol={{
                    span: 8,
                }}
                wrapperCol={{
                    span: 16,
                }}
                style={{
                    maxWidth: 600,
                    alignItems:"center",
                }}
                initialValues={{
                    remember: true,
                }}
                onFinish={registeronFinish}
                onFinishFailed={onFinishFailed}
                autoComplete="off"
            >
                <Form.Item
                    label="用户名"
                    name="username"
                    rules={[
                        {
                            required: true,
                            message: 'Please input your username!',
                        },
                    ]}
                >
                    <Input />
                </Form.Item>

                <Form.Item
                    label="密码"
                    name="password"
                    rules={[
                        {
                            required: true,
                            message: 'Please input your password!',
                        },
                    ]}
                >
                    <Input.Password />
                </Form.Item>


                <Form.Item
                    label="重复密码"
                    name="password2"
                    rules={[
                        {
                            required: true,
                            message: 'Please repeat your password!',
                        },
                    ]}
                >
                    <Input.Password />
                </Form.Item>

                <Form.Item
                    label="邮箱"
                    name="mail"
                    rules={[
                        {
                            required: true,
                            message: 'Please repeat your email address!',
                        },
                    ]}
                >
                    <Input />
                </Form.Item>
                <Form.Item
                    wrapperCol={{
                        offset: 8,
                        span: 16,
                    }}
                >
                    <Space>
                        <Button type="primary" htmlType="submit">
                            Submit
                        </Button>
                        <Button onClick={()=>{setFormMode('login');setErrorMessage('');}}>
                            Login
                        </Button>
                    </Space>
                </Form.Item>
            </Form>
        </>
    );
    return (
        <div id="LoginPage">
            <img src={loginBackgroundImage} id='LoginBG' alt="PxHere"/>
            <div id="LoginDiv">
                {errorMessage!==''&&errorMessage!=='注册成功'&&
                    <Alert type="error" message={errorMessage} banner />}
                {errorMessage==='注册成功'&&
                    <Alert message={errorMessage} banner />}
                <div id="LoginForm">
                    {FormMode==='login'&&loginForm}
                    {FormMode==='register'&&registerForm}
                </div>
            </div>
        </div>
    );
}
export default LoginView;