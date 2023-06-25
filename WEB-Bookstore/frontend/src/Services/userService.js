import axios from "axios";

export const login=(values,navigate)=>{
    const username=values.username;
    const password=values.password;
    console.log('username:',username);
    console.log('password:',password);
    axios.post('/apiAccount/login', {username, password})
        .then(response => {
            console.log(response);
            localStorage.setItem('user', JSON.stringify(values.username));
            navigate('/');
        })
}
