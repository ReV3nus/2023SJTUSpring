package com.example.mybookstore_backend.entity;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import jakarta.persistence.*;

import lombok.Data;

@Data
@Entity
@Table(name = "book")
@JsonIgnoreProperties(value = {"handler","hibernateLazyInitializer","fieldHandler"})
@JsonIdentityInfo(generator = ObjectIdGenerators.PropertyGenerator.class,property = "bookId")
public class Book {
    @Id
    @Column(name = "id")
    @GeneratedValue(strategy = GenerationType.AUTO)

    private int bookId;

    private String isbn;
    private String name;
    private String type;
    private String author;
    private Double price;
    private String description;
    private Integer inventory;
    private String image;
    public Book()
    {

    }
    public Book(String Name,String Author,String Image,String Isbn,Integer Inventory)
    {
        name=Name;
        author=Author;
        image=Image;
        isbn=Isbn;
        inventory=Inventory;
    }
    public Book(int BookID,String Name,String Author,String Image,String Isbn,Integer Inventory)
    {
        bookId=BookID;
        name=Name;
        author=Author;
        image=Image;
        isbn=Isbn;
        inventory=Inventory;
    }
    public void Remove(int num)
    {
        inventory-=num;
    }
}
