import React, {useState} from "react";
import '../CSS/HomeCSS.css'
import {Button, Card, Space, Modal, Form, Input} from "antd";
import {Link} from "react-router-dom";
import axios from "axios";
const {Meta}=Card;
const Books = (props) => {
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
                    bookid:props.Book.bookId,
                }
                axios.post('apiBook/ModifyBook',newBook)
                    .then(res=>{
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
    const handleDelete =() =>{
        const delBook={
            bookid:props.Book.bookId,
        }
        axios.post('apiBook/DeleteBook',delBook)
            .then(res=>{
                setConfirmLoading(false);
                setIsModalOpen(false);
                window.location.reload();
            })
            .catch(err=>{
                console.log(err);
            })
    }
    return (
            <Card
                hoverable
                style={{ width: 240 }}
                cover={<img alt={props.Book.name} src={props.Book.image} />}
            >
                <h6>{`ISBN: ${props.Book.isbn}`}</h6>
                <Meta title={props.Book.name} description={props.Book.author} />
                <Space direction="horizontal" size="large">
                    {/*<h4>{`￥${props.Book.price}`}</h4>*/}
                    {/*<br />*/}
                    {/*<br />*/}
                    <h5>{`库存: ${props.Book.inventory}`}</h5>
                </Space>
                <br/>
                <Space align={"center"} direction={"horizontal"}>
                    <Link to={`Books/${props.Book.bookId}`}>
                        <Button type="primary">
                            浏览详情
                        </Button>
                    </Link>
                    {props.usertype === 'Admin' && (
                        <Button danger type="text" onClick={showModal}>
                            修改信息
                        </Button>
                    )}
                    <Modal
                        title="编辑书籍信息"
                        open={isModalOpen}
                        onOk={handleSave}
                        onCancel={handleCancel}
                        confirmLoading={confirmLoading}
                        footer={[
                            <Button key="back" onClick={handleCancel}>
                                取消
                            </Button>,
                            <Button key="delete" danger type="primary" loading={confirmLoading} onClick={handleDelete}>
                                删除
                            </Button>,
                            <Button
                                key="save"
                                type="primary"
                                loading={confirmLoading}
                                onClick={handleSave}
                            >
                                保存改动
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
                                    initialValue={props.Book.name}
                                >
                                    <Input />
                                </Form.Item>
                                <Form.Item
                                    name="author"
                                    label="作者"
                                    rules={[{ required: true, message: '请输入书籍作者！' }]}
                                    initialValue={props.Book.author}
                                >
                                    <Input />
                                </Form.Item>
                                <Form.Item
                                    name="url"
                                    label="封面url"
                                    rules={[{ required: true, message: '请输入书籍封面（以url地址输入）！' }]}
                                    initialValue={props.Book.image}
                                >
                                    <Input />
                                </Form.Item>
                                <Form.Item
                                    name="isbn"
                                    label="ISBN号码"
                                    rules={[{ required: true, message: '请输入书籍ISBN号码！' }]}
                                    initialValue={props.Book.isbn}
                                >
                                    <Input />
                                </Form.Item>
                                <Form.Item
                                    name="inventory"
                                    label="库存量"
                                    rules={[{ required: true, message: '请输入书籍库存量！' }]}
                                    initialValue={props.Book.inventory}
                                >
                                    <Input />
                                </Form.Item>
                            </Form>
                    </Modal>
                </Space>
            </Card>
    );
};

export default Books;