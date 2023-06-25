import React from 'react';
import {Avatar, Button, Space} from 'antd';
import '../CSS/RootCSS.css'
import axios from "axios";
import {useNavigate} from "react-router";


const UserAvatar = (props) => {
    const navigate = useNavigate();
    return (
        <Space id="avatar" style={{ float: 'right' ,width:'300px'}}>
            <span className="greeting">Hi, {props.usertype} {props.username}!</span>
            <Avatar size='large' gap='50px' src={props.UserIcon} />
            <Button type='primary' danger size='middle' onClick={()=>{
                axios.post("/apiAccount/logout")
                    .then(res=>{
                        navigate('/');
                    })
            }}>log out</Button>
        </Space>
    );
};

export default UserAvatar;