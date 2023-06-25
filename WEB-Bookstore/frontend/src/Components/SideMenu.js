import React from 'react'
export class SideMenu extends React.Component {
    render() {
        return (
            <div className="TheMenu">
            <li>
                <h2>Books</h2>
                <a href="/Cart">My Cart</a>
                <a href="/Orders">My Orders</a>
                <a href="/Profile">My Profile</a>
            </li>
            </div>

        );
    }

}