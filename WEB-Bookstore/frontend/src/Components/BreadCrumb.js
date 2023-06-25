import React from 'react';
import { Breadcrumb} from 'antd';
import {useLocation, Link} from "react-router-dom";

const breadcrumbNameMap = {
    '/Cart': 'Your Cart',
    '/Profile': 'Profile',
    '/Orders': 'Your Orders',
    '/Home':'NULL',
    '/Home/Books':'NULL',
    '/UserManager':'User Manager',
    '/Statistic':'Statistic',
};
const RouteBreadCrumb = () => {
    const location = useLocation();
    const pathSnippets = location.pathname.split('/').filter((i) => i);
    let breadcrumbItems=[];
    breadcrumbItems.push({
                title: <Link to="/">Home</Link>,
                key: 'Home',
    });
    pathSnippets.map((_, index) => {
        const url = `/${pathSnippets.slice(0, index + 1).join('/')}`;
        if(breadcrumbNameMap[url]!=='NULL') {
            if(breadcrumbNameMap[url]) {
                breadcrumbItems.push({
                    key: url,
                    title: <Link to={url}>{breadcrumbNameMap[url]}</Link>,
                });
            }
            else if(url.substring(0,12)==='/Home/Books/') {
                breadcrumbItems.push({
                    key: url,
                    title: <Link to={url}>Book Detail-{url.substring(12)}</Link>,
                })
            }
        }
        return 0;
        });
    return (
        <>
            <Breadcrumb style={{
                        margin: '16px 0',
                    }}
                        items={breadcrumbItems}
            />
        </>
    );
};
class BreadCrumb extends React.Component{

    constructor(props) {
        super(props);
        this.state=props;
    }

    render(){
        return(
            <RouteBreadCrumb/>
        );
    }
}
export default BreadCrumb;
