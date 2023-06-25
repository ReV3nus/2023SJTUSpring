package com.example.mybookstore_backend.controller;

import com.example.mybookstore_backend.entity.Book;
import com.example.mybookstore_backend.entity.CartRecord;
import com.example.mybookstore_backend.service.BookService;
import com.example.mybookstore_backend.service.CartService;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;
import java.util.Objects;

@RestController
@RequestMapping("/apiCart")
public class CartController {
    @Autowired
    private CartService cartService;
    @Autowired
    private BookService bookService;
    @PostMapping("/add")
    public ResponseEntity<CartRecord> AddToCart(@RequestBody Integer BookId, HttpSession session) {
        System.out.println("##Received book ID: " + BookId);
        String username=(String) session.getAttribute("username");
        Book optBook=bookService.findBookById(BookId);
        CartRecord cartRecord;
        cartRecord=new CartRecord(BookId,username,optBook.getName(),optBook.getAuthor(),optBook.getType(),optBook.getPrice(),1);
        CartRecord ret=cartService.AddRecordToCart(cartRecord);
        if(Objects.equals(ret.getBookid(), BookId))
            return ResponseEntity.ok(ret);
        return ResponseEntity.badRequest().build();
    }
    @PostMapping("/del")
    public ResponseEntity<Void> DeleteCart(@RequestBody Integer BookId, HttpSession session) {
        String username=(String) session.getAttribute("username");
        cartService.DeleteCartRecord(BookId,username);
        return ResponseEntity.ok().build();
//        return ResponseEntity.notFound().build();
    }
    @GetMapping("/getCart")
    public ResponseEntity<List<CartRecord>> GetCart(HttpSession session) {
        String username=(String) session.getAttribute("username");
        return ResponseEntity.ok(cartService.getCart(username));
//        return ResponseEntity.notFound().build();
    }
    @PostMapping("/clear")
    public ResponseEntity<Void> ClearCart(HttpSession session){
        String username=(String) session.getAttribute("username");
        cartService.ClearCart(username);
        return ResponseEntity.ok().build();
    }
}
