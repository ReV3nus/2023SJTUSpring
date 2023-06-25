import React from 'react';
import logo from '../Assets/logo.svg';
import logoFont from '../Assets/logo-name.svg';
import '../CSS/RootCSS.css'

class SiteLogo extends React.Component {

    render(){

        return(
            <div id="logos" style={{float:'left'}}>
                <img alt="logo"  id="logo" src={logo} />
                <img alt="Book Store"  id="LogoFont" className="logo-font" src={logoFont}/>
            </div>
        );
    }
}
export default SiteLogo;