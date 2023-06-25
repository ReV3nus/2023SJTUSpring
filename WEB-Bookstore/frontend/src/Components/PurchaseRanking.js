import React, {useEffect, useState} from 'react';
import {Card, Table} from "antd";

const columns = [
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
const SaleRanking = (props) => {
    const [displayEntries2,setDisplayEntries2]=useState([]);

    useEffect(()=>{
        setDisplayEntries2([]);
        console.log(props.items);
        const ResultEntries2 = props.items.reduce((result,entry)=>{
            const NewEntry={
                username:entry.username,
                bought:entry.count,
                cost:entry.price,
            }
            const existing=result.find((item)=>item.username===NewEntry.username);
            if(existing)
            {
                console.log(entry.bookname+' '+existing.bought+' '+existing.username+' '+NewEntry.bought);
                existing.bought+=NewEntry.bought;
                existing.cost+=NewEntry.cost;
                existing.cost.toFixed(2);
            }
            else{
                console.log(NewEntry.username+' '+NewEntry.bought+' '+entry.bookname);
                NewEntry.cost.toFixed(2);
                result.push(NewEntry);
                console.log(result);
            }
            return result;
        },[])
        setDisplayEntries2(ResultEntries2);
    },[props])
    return (
        <>
            <Card title={`用户消费排行`} id="StaCard" hoverable>
                <Table columns={columns} dataSource={displayEntries2}/>
            </Card>
        </>
    );
};

export default SaleRanking;