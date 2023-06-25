package com.example.mybookstore_backend.dao;

import com.example.mybookstore_backend.entity.User;

public interface UserDao {
    User findUser(String user);
    User AddUser(User user);
}
