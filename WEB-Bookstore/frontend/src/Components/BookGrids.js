import React, {useEffect, useState} from 'react';
import Books from "./Books";
import {Row, Col, Pagination, Space} from "antd";

const BookGrids =(props)=>{
    const Span=4;
    const pageSize=18;
    const [currentPage,setCurrentPage]=useState(1);
    const [startIndex,setStartIndex]=useState(0);
    const [endIndex,setEndIndex]=useState(pageSize);
    const [currentBooks,setCurrentBooks]=useState(props.books);
    const handlePageChange = (page, range) => {
        setCurrentPage(page);
        setStartIndex((page-1)*pageSize);
        setEndIndex(page*pageSize);

    };
    useEffect(()=>{
        // console.log(currentPage);
        // console.log(startIndex);
        // console.log(endIndex);
        // console.log(currentBooks);
        setCurrentBooks(props.books.slice(startIndex,endIndex));
    },[currentPage,startIndex,endIndex,props.books])
    return (
        <Space align={"center"} direction={"vertical"}>
            <div style={{marginTop:'340px',}}>
                <Row gutter={[{ xs: 8, sm: 16, md: 24, lg: 32 },{ xs: 8, sm: 16, md: 24, lg: 32 }]}>
                    {currentBooks.map(book => (
                        <Col className="gutter-row" span={Span}>
                            <Books Book={book} usertype={props.usertype}/>
                        </Col>
                    ))}
                </Row>
            </div>
            <div>
                {props.books.length > 0 && (
                <Pagination
                    current={currentPage}
                    pageSize={pageSize}
                    total={props.books.length}
                    onChange={handlePageChange}
                    showTotal={(total, range) => `${range[0]}-${range[1]} of ${total} items`}
                />)}
            </div>
        </Space>
    );
}
export default BookGrids;