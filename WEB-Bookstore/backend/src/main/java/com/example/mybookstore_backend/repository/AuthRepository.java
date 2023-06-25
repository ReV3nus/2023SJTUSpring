package com.example.mybookstore_backend.repository;

import com.example.mybookstore_backend.entity.UserAuth;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface AuthRepository extends JpaRepository<UserAuth,Integer> {
    UserAuth findByUsername(String user);
    List<UserAuth> findAll();

}
