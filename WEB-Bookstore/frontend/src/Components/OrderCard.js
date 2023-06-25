import React from "react";
import axios from "axios";
import {Card, Spin, Table} from "antd";
import '../CSS/OrderCSS.css'
const columns = [
    {
        title: '书籍编号',
        dataIndex: 'bookid',
        key:'bookid',
    },
    {
        title: '书名',
        dataIndex: 'bookname',
        key:'bookname',
    },
    {
        title: '作者',
        dataIndex: 'author',
        key:'author',
    },
    {
        title: '类型',
        dataIndex: 'type',
        key:'type',
    },
    {
        title:'价格',
        dataIndex:'price',
        key:'price',
    },
    {
        title:'数量',
        dataIndex:'count',
        key:'count',
    },
];
class OrderCard extends React.Component{
    constructor(props) {
        super(props);
        this.state={
            items:[],
            isLoading:true,
        }

    }
    componentDidMount() {
        console.log("getting "+this.props.order.recordId);
        axios.get('/apiOrder/getOrderItems',{params:{oid:this.props.order.recordId}})
            .then(
                response=>{
                    this.setState({
                        items: response.data,
                        isLoading: false,
                    });
                }
            )
            .catch(err =>{
                console.log(err);
                this.setState({
                    isLoading: false,
                })
            });

    }

    render() {

        if (this.state.isLoading) {
            return (
                <div id='pageDiv'>
                    <Spin tip="Loading" size="small" id='SpinCSS'>
                        <div className="content" />
                    </Spin>
                </div>
            );
        }
        if(this.props.filterWord===''||
            (this.state.items?.some((item)=>item.bookname.includes(this.props.filterWord)))
            ) {
            return (
                <Card title={`下单时间：` + this.props.order.time} extra={`购买者：`+this.props.order.username} id="OrderCard" hoverable>
                    <Table columns={columns} dataSource={this.state.items}/>
                </Card>
            );
        }
    }

}
export default OrderCard;