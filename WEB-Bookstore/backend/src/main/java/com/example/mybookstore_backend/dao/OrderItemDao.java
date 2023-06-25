package com.example.mybookstore_backend.dao;
import com.example.mybookstore_backend.entity.OrderItem;

import java.util.List;

public interface OrderItemDao {
    OrderItem AddItem(OrderItem item);
}
