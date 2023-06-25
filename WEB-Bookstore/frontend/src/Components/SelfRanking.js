import React, {useEffect, useState} from 'react';
import {Card, Table} from "antd";

const columns = [
    {
        title: '书籍编号',
        dataIndex: 'bookid',
        key:'bookid',
        sorter: (a, b) => a.bookid - b.bookid,
        sortDirections: ['ascend', 'descend'],
    },
    {
        title: '书名',
        dataIndex: 'bookname',
        key:'bookname',
        sorter: (a, b) => a.bookname.length - b.bookname.length,
        sortDirections: ['ascend', 'descend'],
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
        title:'数量',
        dataIndex:'count',
        key:'count',
        sorter: (a, b) => a.count - b.count,
        sortDirections: ['ascend', 'descend'],
        defaultSortOrder: 'descend',
    },
];
const SaleRanking = (props) => {
    const [displayEntries,setDisplayEntries]=useState([]);
    const [totalCount,setTotalCount]=useState(0);
    const [totalCost,setTotalCost]=useState(0);
    useEffect(()=>{
        setDisplayEntries([]);
        setTotalCost(0);
        setTotalCount(0);
        const ResultEntries = props.items.reduce((result,entry)=>{
            setTotalCost(prevState => prevState+entry.price);
            setTotalCount(prevState => prevState+entry.count);
            const existing=result.find((item)=>item.bookid===entry.bookid);
            if(existing)
            {
                existing.count+=entry.count;
            }
            else{
                result.push(entry);
            }
            return result;
        },[])
        setDisplayEntries(ResultEntries);
    },[props])
    return (
        <>
            <Card title={`用户消费排行`}  extra={`购买总数：`+totalCount+`本   总金额：`+totalCost.toFixed(2)+`元`} id="StaCard" hoverable>
                <Table columns={columns} dataSource={displayEntries}/>
            </Card>
        </>
    );
};

export default SaleRanking;