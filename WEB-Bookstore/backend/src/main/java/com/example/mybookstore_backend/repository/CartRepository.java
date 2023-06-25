package com.example.mybookstore_backend.repository;

import com.example.mybookstore_backend.entity.CartRecord;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface CartRepository  extends JpaRepository<CartRecord,Integer> {
//    CartRecord AddRecord()
//    {
//        return CartRecord save(CartRecord record);
//    }
//    void deleteById(Integer rid);
//    @Query("select r from CartRecord r WHERE r.username=?Username")
//    List<CartRecord> getCart(String Username);
    List<CartRecord> findByUsername(String username);
    void deleteByBookidAndUsername(Integer bid,String user);

    void deleteByUsername(String username);
}
