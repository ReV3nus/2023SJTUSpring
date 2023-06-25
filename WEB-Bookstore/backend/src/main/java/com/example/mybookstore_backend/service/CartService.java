package com.example.mybookstore_backend.service;

import com.example.mybookstore_backend.entity.CartRecord;
import java.util.List;

public interface CartService {
    CartRecord AddRecordToCart(CartRecord record);
    void DeleteCartRecord(Integer bid,String user);
    List<CartRecord>getCart(String user);
    void ClearCart(String user);
}
