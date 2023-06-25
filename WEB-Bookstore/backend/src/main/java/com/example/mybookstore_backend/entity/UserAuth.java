package com.example.mybookstore_backend.entity;

import com.fasterxml.jackson.annotation.JsonIdentityInfo;
import com.fasterxml.jackson.annotation.JsonIgnore;
import com.fasterxml.jackson.annotation.JsonIgnoreProperties;
import com.fasterxml.jackson.annotation.ObjectIdGenerators;
import jakarta.persistence.*;
import lombok.Data;

@Data
@Entity
@Table(name="user_auth")
@JsonIgnoreProperties(value = {"handler","hibernateLazyInitializer","fieldHandler"})
public class UserAuth {
    @Id
    @Column(name="user_id")
    @GeneratedValue(strategy = GenerationType.AUTO)
    private int userid;
    private String username;
    private String password;
    private String usertype;
    public UserAuth(String Username,String Password){
        username=Username;
        password=Password;
        usertype="User";
    }
    public UserAuth() {}

    @OneToOne(mappedBy = "userAuth",cascade = CascadeType.ALL)
    @JsonIgnore
    private User user;
}
