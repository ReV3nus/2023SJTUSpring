import axios from "axios";

export const AddToCart=(BookId,messageApi)=>{
    console.log(BookId);
    axios({
        method:'POST',
        url:'/apiCart/add',
        headers:{
          "Content-Type":'application/json',
        },
        data: BookId,
    })
        .then(response => {
            messageApi.open({
                type: 'success',
                content: '成功添加至购物车',
                duration: 3,
            }).then(()=>{});
            console.log(response);
        })
        .catch(error => {
            messageApi.open({
                type: 'error',
                content: '添加至购物车失败',
                duration: 3,
            }).then(()=>{});
            console.log(error);
        });
}
export const AddToOrder=(BookId,messageApi)=>{
    console.log(BookId);
    axios({
        method:'POST',
        url:'/apiOrder/addItem',
        headers:{
            "Content-Type":'application/json',
        },
        data: BookId,
    })
        .then(response => {
            messageApi.open({
                type: 'success',
                content: '购买成功',
                duration: 3,
            }).then(()=>{});
            console.log(response);
        })
        .catch(error => {
            messageApi.open({
                type: 'error',
                content: '购买失败',
                duration: 3,
            }).then(()=>{});
            console.log(error);
        });
}
export const CartPurchase=(messageApi)=>{
    axios({
        method:'POST',
        url:'/apiOrder/cartPurchase',
        headers:{
            "Content-Type":'application/json',
        },
    })
        .then(response => {
            messageApi.open({
                type: 'success',
                content: '购物车清单购买成功',
                duration: 3,
            }).then(()=>{});
            console.log(response);
        })
        .catch(error => {
            messageApi.open({
                type: 'error',
                content: '购买失败',
                duration: 3,
            }).then(()=>{});
            console.log(error);
            console.log(error);
        });
}
export const CartClear=(messageApi)=>{
    axios({
        method:'POST',
        url:'/apiCart/clear',
        headers:{
            "Content-Type":'application/json',
        },
    })
        .then(response => {
            messageApi.open({
                type: 'success',
                content: '清空成功',
                duration: 3,
            }).then(()=>{});
            console.log(response);
        })
        .catch(error => {
            messageApi.open({
                type: 'error',
                content: '清空失败',
                duration: 3,
            }).then(()=>{});
            console.log(error);
        });
}