import React, {useState} from "react";
import '../CSS/HomeCSS.css'
import {Button, Space, Modal, Form, Input} from "antd";
import axios from "axios";
const BookAdder = (props) => {
    const [isModalOpen, setIsModalOpen] = useState(false);
    const [confirmLoading, setConfirmLoading] = useState(false);
    const [form] = Form.useForm();
    const showModal = () => {
        setIsModalOpen(true);
    };
    const handleCancel = () => {
        setIsModalOpen(false);
    };
    const handleSave = () => {
        form
            .validateFields()
            .then((values) => {
                const newBook={
                    name:values.name,
                    author:values.author,
                    url:values.url,
                    isbn:values.isbn,
                    inventory:values.inventory,
                }
                axios.post('apiBook/AddBook',newBook)
                    .then(res=>{
                        console.log(res);
                        setConfirmLoading(false);
                        setIsModalOpen(false);
                        window.location.reload();
                    })
                    .catch(err=>{
                        console.log(err);
                    })
            })
            .catch((info) => {
                console.log('Validate Failed:', info);
            });
    };
    return (
        <Space>
            <Button type="primary" onClick={showModal}>
                新建书籍
            </Button>
            <Modal
                title="新建书籍信息"
                open={isModalOpen}
                onOk={handleSave}
                onCancel={handleCancel}
                confirmLoading={confirmLoading}
                footer={[
                    <Button key="back" onClick={handleCancel}>
                        取消
                    </Button>,
                    <Button
                        key="save"
                        type="primary"
                        loading={confirmLoading}
                        onClick={handleSave}
                    >
                        新建书籍
                    </Button>,
                ]}>
                <Form
                    form={form}
                    layout="vertical"
                    name="form_in_modal"
                    initialValues={{ modifier: 'public' }}
                >
                    <Form.Item
                        name="name"
                        label="书名"
                        rules={[{ required: true, message: '请输入书籍名称！' }]}
                    >
                        <Input />
                    </Form.Item>
                    <Form.Item
                        name="author"
                        label="作者"
                        rules={[{ required: true, message: '请输入书籍作者！' }]}
                    >
                        <Input />
                    </Form.Item>
                    <Form.Item
                        name="url"
                        label="封面url"
                        rules={[{ required: true, message: '请输入书籍封面（以url地址输入）！' }]}
                    >
                        <Input />
                    </Form.Item>
                    <Form.Item
                        name="isbn"
                        label="ISBN号码"
                        rules={[{ required: true, message: '请输入书籍ISBN号码！' }]}
                    >
                        <Input />
                    </Form.Item>
                    <Form.Item
                        name="inventory"
                        label="库存量"
                        rules={[{ required: true, message: '请输入书籍库存量！' }]}
                    >
                        <Input />
                    </Form.Item>
                </Form>
            </Modal>
        </Space>
    );
};

export default BookAdder;