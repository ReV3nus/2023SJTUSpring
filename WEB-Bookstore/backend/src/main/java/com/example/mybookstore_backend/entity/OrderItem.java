package com.example.mybookstore_backend.entity;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import lombok.Data;

@Data
@Entity
@Table(name = "order_item")
@JsonIgnoreProperties(value = {"handler","hibernateLazyInitializer","fieldHandler"})
public class OrderItem {

    @Id
    @Column(name = "id")
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int recordId;
//    private Integer orderid;
    private Integer bookid;
    private String bookname;
    private String author;
    private String type;
    private Double price;
    private Integer count;
    private String username;
    public OrderItem(Integer Bookid,String Bookname,String Author,String Type,Double Price,Integer Count,String Username)
    {
        bookid=Bookid;
        bookname=Bookname;
        author=Author;
        type=Type;
        price=Price;
        count=Count;
        username=Username;
    }

    public OrderItem() {

    }
    public String __TestOrderRecord(){
        return orderRecord.getTime();
    }
    @JsonIgnore
    @ManyToOne
    @JoinColumn(name = "orderid")
    private OrderRecord orderRecord;
}
