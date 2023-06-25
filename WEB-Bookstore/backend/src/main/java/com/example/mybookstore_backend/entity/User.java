package com.example.mybookstore_backend.entity;

import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import jakarta.persistence.*;
import lombok.Data;

@Data
@Entity
@Table(name = "user")
@JsonIgnoreProperties(value = {"handler","hibernateLazyInitializer","fieldHandler"})
public class User {

    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    private int userId;
    private String username;
    private String email;
    public User(String Username,String em){
        username=Username;
        email=em;
    }
    public User() {}
    @OneToOne
    @JoinColumn(name="auth_id")
    @JsonIgnore
    private UserAuth userAuth;
}