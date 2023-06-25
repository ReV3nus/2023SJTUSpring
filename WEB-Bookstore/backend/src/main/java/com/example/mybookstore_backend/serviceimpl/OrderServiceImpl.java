package com.example.mybookstore_backend.serviceimpl;

import com.example.mybookstore_backend.dao.OrderDao;
import com.example.mybookstore_backend.entity.OrderItem;
import com.example.mybookstore_backend.entity.OrderRecord;
import com.example.mybookstore_backend.service.OrderService;
import jakarta.persistence.criteria.Order;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;

@Service
public class OrderServiceImpl implements OrderService {
    @Autowired
    private OrderDao orderDao;
    @Override
    public OrderRecord AddRecordToOrder(OrderRecord record){
        return orderDao.AddRecord(record);
    }
    @Override
    public List<OrderRecord> getOrder(String user){
        return orderDao.getOrder(user);
    }
    @Override
    public OrderRecord getRecord(Integer rid)
    {
        return orderDao.getRecord(rid);
    }
    @Override
    public List<OrderRecord> getAllOrders(){return orderDao.getAllOrders();}
}
