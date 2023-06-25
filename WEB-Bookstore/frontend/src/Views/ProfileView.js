import React from 'react';
import BreadCrumb from "../Components/BreadCrumb";
import '../CSS/ProfileCSS.css'
import {Button, Descriptions, Space, Spin} from 'antd';
import { Image } from 'antd';
import { Input } from 'antd';
import {DownloadOutlined} from "@ant-design/icons";
import {Navigate} from "react-router-dom";
import axios from "axios";
const { TextArea } = Input;
class ProfileView extends React.Component{
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
        const onChange = (e) => {
            console.log('Change:', e.target.value);
        };
        return (
            <>
                <BreadCrumb paths={["Home","Profile"]}/>
                <div id='pageDiv'>
                    <h1>User Avatar</h1>
                    <Space direction='horizontal' wrap align='end'>
                        <Image
                            width={200}
                            src={this.props.UserIcon}
                        />
                        <Button type="primary" shape="round" icon={<DownloadOutlined />} size='large'>
                          Download
                        </Button>
                    </Space>
                    <br/>
                    <br/>
                    <Descriptions title="用户信息" layout="vertical" bordered column={{ xxl: 4, xl: 3, lg: 3, md: 3, sm: 2, xs: 1 }}>
                        <Descriptions.Item label="用户姓名" span={2}>{this.props.username}</Descriptions.Item>
                        <Descriptions.Item label="用户ID" span={2}>34544587642</Descriptions.Item>
                        <Descriptions.Item label="用户签名" span={6}>
                            <TextArea
                                rows={4}
                                placeholder="Input Your Sign"
                                maxLength={100}
                                onChange={onChange}
                                showCount
                                allowClear
                                defaultValue='AAAAAA'
                            />
                        </Descriptions.Item>

                    </Descriptions>
                    <Space wrap>
                        <Button type="primary">保存</Button>
                        <Button>取消</Button>
                    </Space>
                </div>
            </>
        )
    }
}
export default ProfileView;