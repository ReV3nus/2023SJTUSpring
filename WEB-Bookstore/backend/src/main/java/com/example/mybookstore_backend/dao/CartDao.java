package com.example.mybookstore_backend.dao;

import com.example.mybookstore_backend.entity.Book;
import com.example.mybookstore_backend.entity.CartRecord;

import java.util.List;

public interface CartDao {
    CartRecord AddRecord(CartRecord record);
    void DeleteRecord(Integer bid,String user);
    List<CartRecord> getCart(String user);
    void ClearCart(String user);
}
