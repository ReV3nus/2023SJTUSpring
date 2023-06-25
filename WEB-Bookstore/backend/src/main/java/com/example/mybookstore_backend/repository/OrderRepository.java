package com.example.mybookstore_backend.repository;

import com.example.mybookstore_backend.entity.OrderRecord;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface OrderRepository extends JpaRepository<OrderRecord,Integer> {
    List<OrderRecord> findOrderRecordsByUsername(String username);
    OrderRecord findOrderRecordByRecordId(Integer rid);
}
