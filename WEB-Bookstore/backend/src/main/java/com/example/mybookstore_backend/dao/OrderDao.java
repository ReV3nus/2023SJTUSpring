package com.example.mybookstore_backend.dao;

import com.example.mybookstore_backend.entity.OrderRecord;

import java.util.List;

public interface OrderDao {
    OrderRecord AddRecord(OrderRecord record);
    List<OrderRecord> getOrder(String user);
    OrderRecord getRecord(Integer rid);
    List<OrderRecord> getAllOrders();
}
