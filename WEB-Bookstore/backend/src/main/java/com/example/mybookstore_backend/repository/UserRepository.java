package com.example.mybookstore_backend.repository;

import com.example.mybookstore_backend.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;

public interface UserRepository extends JpaRepository<User,Integer> {
    User findByUsername(String user);
}
