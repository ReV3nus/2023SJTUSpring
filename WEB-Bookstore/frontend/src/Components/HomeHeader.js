import React from 'react';
import SiteLogo from './SiteLogo.js'
import UserAvatar from "./UserAvatar";
import { Row, Col } from 'antd';

class HomeHeader extends React.Component{

    constructor(props) {
        super(props);
        this.state=props;
    }
    render() {
        return(
            <div id="HomeHeader">
                <Row>
                    <Col xs={24} sm={24} md={5} lg={5} xl={5} xxl={4}>
                        <SiteLogo/>
                    </Col>
                    <Col xs={0} sm={0} md={19} lg={19} xl={19} xxl={20}>
                        <UserAvatar username={this.props.username} UserIcon={this.props.UserIcon}/>
                    </Col>
                </Row>
            </div>
        )
    }
}
export default HomeHeader;