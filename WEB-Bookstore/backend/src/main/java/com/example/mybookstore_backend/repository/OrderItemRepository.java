package com.example.mybookstore_backend.repository;

import com.example.mybookstore_backend.entity.OrderItem;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface OrderItemRepository  extends JpaRepository<OrderItem,Integer> {
}
