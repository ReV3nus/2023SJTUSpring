package com.example.mybookstore_backend.daoimpl;

import com.example.mybookstore_backend.dao.UserDao;
import com.example.mybookstore_backend.entity.User;
import com.example.mybookstore_backend.repository.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

@Repository
public class UserDaoImpl implements UserDao {
    @Autowired
    private UserRepository userRepository;
    @Override
    public User findUser(String user)
    {
        return userRepository.findByUsername(user);
    }
    @Override
    public User AddUser(User user)
    {
        return userRepository.save(user);
    }
}
