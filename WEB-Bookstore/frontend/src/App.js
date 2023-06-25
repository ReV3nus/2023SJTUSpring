import React from 'react';
import {createBrowserRouter,RouterProvider,Navigate} from 'react-router-dom';
import './App.css';
import HomeView from './Views/HomeView';
import CartView from './Views/CartView';
import ProfileView from './Views/ProfileView';
import OrdersView from './Views/OrdersView';
import LoginView from './Views/LoginView';
import Root from './Routes/Root'
import BookDetailView from './Views/BookDetailView';

import UserIcon from '../src/Assets/usericon.jpg'
import UMView from "./Views/UMView";
import StatisticView from "./Views/StatisticView";

const router = createBrowserRouter([
    {
        path: "/Login",
        element: <LoginView />,
    },
    {
        path: "/",
        element: <Root username="?" UserIcon={UserIcon}/>,
        errorElement: <h1>error</h1>,
        children: [
            {
                path:"/",
                element: <Navigate to="/Home"/>,
            },
            {
                path:"Home",
                element:<HomeView />,
            },
            {
                path: "Home/Books/:BookID",
                loader:({params})=> {
                    console.log(params.BookID);
                    return params.BookID;
                },
                element:<BookDetailView />
            },
            {
                path: "Cart",
                element: <CartView />,
            },
            {
                path: "Profile",
                element: <ProfileView  username="ReV3nus" UserIcon={UserIcon}/>,
            },
            {
                path: "Orders",
                element: <OrdersView />,
            },
            {
                path: "UserManager",
                element: <UMView />,
            },
            {
                path:"Statistic",
                element: <StatisticView/>
            },
            {
                path: "*",
                element: <Navigate to="/Home"/>,
            },
        ],
    },
    {
        path: "*",
        element: <Navigate to="../Home"/>,
    }
]);
class App extends React.Component {
    constructor(props) {
        super(props);
        this.state={
            isAuthenticated: false,
        }
    }
    render() {
        return (
            <React.StrictMode>
                <RouterProvider router={router} />
            </React.StrictMode>
        );
    }
}


export default App;
