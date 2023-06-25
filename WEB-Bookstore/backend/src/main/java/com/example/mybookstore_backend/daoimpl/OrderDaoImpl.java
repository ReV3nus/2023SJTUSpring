package com.example.mybookstore_backend.daoimpl;

import com.example.mybookstore_backend.dao.OrderDao;
import com.example.mybookstore_backend.entity.OrderRecord;
import com.example.mybookstore_backend.repository.OrderRepository;
import org.aspectj.weaver.ast.Or;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public class OrderDaoImpl implements OrderDao {
    @Autowired
    private OrderRepository orderRepository;

    @Override
    public OrderRecord AddRecord(OrderRecord record){
        return orderRepository.save(record);
    }
    @Override
    public List<OrderRecord> getOrder(String user){
        return orderRepository.findOrderRecordsByUsername(user);
    }
    @Override
    public OrderRecord getRecord(Integer rid){
        return orderRepository.findOrderRecordByRecordId(rid);
    }
    @Override
    public List<OrderRecord> getAllOrders(){return orderRepository.findAll();}
}
