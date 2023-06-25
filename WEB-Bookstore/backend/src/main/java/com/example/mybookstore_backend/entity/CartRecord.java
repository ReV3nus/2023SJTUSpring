package com.example.mybookstore_backend.entity;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import jakarta.persistence.*;
import lombok.Data;

@Data
@Entity
@Table(name = "cart")
@JsonIgnoreProperties(value = {"handler","hibernateLazyInitializer","fieldHandler"})
//@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class,property = "RecordId")
public class CartRecord {
    @Id
    @Column(name = "id")
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int recordId;
    private Integer bookid;
    private String username;
    private String bookname;
    private String author;
    private String type;
    private Double price;
    private Integer count;
    public CartRecord(Integer bid,String user,String Bookname,String Author,String Type,Double Price,Integer Count)
    {
        bookid=bid;
        username=user;
        bookname=Bookname;
        author=Author;
        type=Type;
        price=Price;
        count=Count;
    }

    public CartRecord() {

    }

}
