package com.example.mybookstore_backend.serviceimpl;

import com.example.mybookstore_backend.dao.CartDao;
import com.example.mybookstore_backend.entity.CartRecord;
import com.example.mybookstore_backend.service.CartService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.List;

@Service
public class CartServiceImpl implements CartService {
    @Autowired
    private CartDao cartDao;
    @Override
    public CartRecord AddRecordToCart(CartRecord record){
        return cartDao.AddRecord(record);
    }
    @Override
    public void DeleteCartRecord(Integer bid,String user){
        cartDao.DeleteRecord(bid,user);
    }
    @Override
    public List<CartRecord> getCart(String user){
        return cartDao.getCart(user);
    }
    @Override
    @Transactional
    public void ClearCart(String user){cartDao.ClearCart(user);}
}
