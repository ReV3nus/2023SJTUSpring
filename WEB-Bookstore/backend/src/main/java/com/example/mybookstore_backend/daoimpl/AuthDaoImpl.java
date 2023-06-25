package com.example.mybookstore_backend.daoimpl;

import com.example.mybookstore_backend.dao.AuthDao;
import com.example.mybookstore_backend.entity.UserAuth;
import com.example.mybookstore_backend.repository.AuthRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public class AuthDaoImpl implements AuthDao {
    @Autowired
    private AuthRepository authRepository;
    @Override
    public UserAuth findUser(String user)
    {
        return authRepository.findByUsername(user);
    }
    @Override
    public UserAuth AddAuth(UserAuth auth)
    {
        return authRepository.save(auth);
    }
    @Override
    public List<UserAuth> GetAllUsers()
    {
        return authRepository.findAll();
    }
}
