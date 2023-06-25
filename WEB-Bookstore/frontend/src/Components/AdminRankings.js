import React, {useEffect, useState} from 'react';
import {Card, Table} from "antd";

const columns1 = [
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
        title:'销量',
        dataIndex:'sale',
        key:'sale',
        sorter: (a, b) => a.sale - b.sale,
        sortDirections: ['ascend', 'descend', 'ascend'],
        defaultSortOrder: 'descend',
    },
];
const columns2 = [
    {
        title: '用户名',
        dataIndex: 'username',
        key:'username',
    },
    {
        title: '购买数量',
        dataIndex: 'bought',
        key:'bought',
    },
    {
        title: '消费金额',
        dataIndex: 'cost',
        key:'cost',
        sorter: (a, b) => a.cost - b.cost,
        sortDirections: ['ascend', 'descend', 'ascend'],
        defaultSortOrder: 'descend',
    },
];
const AdminRankings = (props) => {
    const [displayEntries1,setDisplayEntries1]=useState([]);
    const [displayEntries2,setDisplayEntries2]=useState([]);

    useEffect(()=>{
        setDisplayEntries1([]);
        setDisplayEntries2([]);
        const ResultEntries1 = props.items.reduce((result,entry)=>{
            const existing=result.find((item)=>item.bookid===entry.bookid);
            if(existing)
            {
                existing.sale+=entry.count;
            }
            else{
                entry.sale=entry.count;
                result.push(entry);
            }
            return result;
        },[])
        setDisplayEntries1(ResultEntries1);
    },[props])
    return (
        <>
            <Card title={`书籍销量排行`} id="StaCard" hoverable>
                <Table columns={columns1} dataSource={displayEntries1}/>
            </Card>
            {/*<Card title={`用户消费排行`} id="StaCard" hoverable>*/}
            {/*    <Table columns={columns2} dataSource={displayEntries2}/>*/}
            {/*</Card>*/}
        </>
    );
};

export default AdminRankings;