package com.example.mybookstore_backend.entity;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import lombok.Data;

import java.util.ArrayList;
import java.util.List;

@Data
@Entity
@Table(name = "orders")
@JsonIgnoreProperties(value = {"handler","hibernateLazyInitializer","fieldHandler"})
public class OrderRecord {
    @Id
    @Column(name = "id")
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int recordId;
    private String username;
    private String time;
    public OrderRecord(String user)
    {
        username=user;
    }
    public OrderRecord(String user,String Time)
    {
        username=user;
        time=Time;
    }

    public OrderRecord() {

    }
    public int __TestOrderItem(){
        return orderItems.size();
    }
    @JsonIgnore
    @OneToMany(mappedBy = "orderRecord",cascade = CascadeType.REMOVE)
    private List<OrderItem> orderItems = new ArrayList<>();
}
