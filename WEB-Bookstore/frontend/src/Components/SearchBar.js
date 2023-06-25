import React from 'react';
import '../CSS/HomeCSS.css'
import { Input } from 'antd';

const { Search } = Input;

class SearchBar extends React.Component {
    render() {
        const onSearch=()=>{return (<h1>OnSearch</h1>)};
        return (
            <div id='SearchBar'>
                <Search
                    placeholder="input search text"
                    onSearch={onSearch}
                    enterButton
                    allowClear
                    style={{width: 400,}}

                />
            </div>
        );
    }
}
export default SearchBar;