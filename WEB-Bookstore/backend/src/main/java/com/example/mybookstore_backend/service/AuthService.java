package com.example.mybookstore_backend.service;

import com.example.mybookstore_backend.entity.UserAuth;

import java.util.List;

public interface AuthService {
    UserAuth findUser(String user);
    UserAuth AddAuth(UserAuth auth);
    List<UserAuth> GetAllUsers();
}
