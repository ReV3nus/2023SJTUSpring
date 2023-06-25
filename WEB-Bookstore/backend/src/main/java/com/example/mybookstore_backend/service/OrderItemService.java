package com.example.mybookstore_backend.service;

import com.example.mybookstore_backend.entity.OrderItem;

import java.util.List;

public interface OrderItemService {
    OrderItem AddItemToOrder(OrderItem item);
}
