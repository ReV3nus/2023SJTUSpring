package com.example.mybookstore_backend.dao;

import com.example.mybookstore_backend.entity.UserAuth;

import java.util.List;

public interface AuthDao {
    UserAuth findUser(String user);
    UserAuth AddAuth(UserAuth auth);
    List<UserAuth> GetAllUsers();

}
