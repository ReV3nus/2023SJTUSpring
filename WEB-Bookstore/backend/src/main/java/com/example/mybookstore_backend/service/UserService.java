package com.example.mybookstore_backend.service;

import com.example.mybookstore_backend.entity.User;

public interface UserService {
    User findUser(String username);
    User AddUser(User user);
}
