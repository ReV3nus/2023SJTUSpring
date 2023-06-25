package com.example.mybookstore_backend.controller;

import com.example.mybookstore_backend.entity.Book;
import com.example.mybookstore_backend.entity.CartRecord;
import com.example.mybookstore_backend.entity.OrderRecord;
import com.example.mybookstore_backend.entity.OrderItem;
import com.example.mybookstore_backend.service.BookService;
import com.example.mybookstore_backend.service.CartService;
import com.example.mybookstore_backend.service.OrderItemService;
import com.example.mybookstore_backend.service.OrderService;
import jakarta.servlet.http.HttpSession;
import org.aspectj.weaver.ast.Or;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.List;
import java.util.Objects;

@RestController
@RequestMapping("/apiOrder")
public class OrderController {
    @Autowired
    private OrderService orderService;
    @Autowired
    private OrderItemService orderItemService;
    @Autowired
    private BookService bookService;
    @Autowired
    private CartService cartService;
    @PostMapping("/addItem")
    public ResponseEntity<OrderItem> AddToOrder(@RequestBody Integer BookId, HttpSession session) {
        String username=(String) session.getAttribute("username");
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formattedDateTime = now.format(formatter);
        Book optBook=bookService.findBookById(BookId);

        optBook.Remove(1);
        bookService.SaveBook(optBook);

        OrderRecord orderRecord=new OrderRecord(username,formattedDateTime);
        OrderItem orderItem=new OrderItem(optBook.getBookId(),optBook.getName(),optBook.getAuthor(),optBook.getType(),optBook.getPrice(),1,username);
        System.out.print(orderRecord);
        System.out.println();
        orderItem.setOrderRecord(orderRecord);
        orderRecord.getOrderItems().add(orderItem);
        System.out.print(orderItem.__TestOrderRecord());
        System.out.println();
        orderService.AddRecordToOrder(orderRecord);
        orderItemService.AddItemToOrder(orderItem);
        return ResponseEntity.ok(orderItem);
    }
    @PostMapping("/cartPurchase")
    public ResponseEntity<OrderRecord> CartPurchase(HttpSession session) {
        String username=(String) session.getAttribute("username");
        LocalDateTime now = LocalDateTime.now();
        DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss");
        String formattedDateTime = now.format(formatter);

        OrderRecord orderRecord=new OrderRecord(username,formattedDateTime);

        List<CartRecord>UserCart=cartService.getCart(username);
        for (CartRecord cartRecord : UserCart) {
            Book optBook=bookService.findBookById(cartRecord.getBookid());
            optBook.Remove(1);
            bookService.SaveBook(optBook);

            OrderItem orderItem=new OrderItem(cartRecord.getBookid(),cartRecord.getBookname(),cartRecord.getAuthor(),cartRecord.getType(),cartRecord.getPrice(),cartRecord.getCount(),username);
            orderItem.setOrderRecord(orderRecord);
            orderRecord.getOrderItems().add(orderItem);
            orderService.AddRecordToOrder(orderRecord);
            orderItemService.AddItemToOrder(orderItem);
        }
        cartService.ClearCart(username);
        return ResponseEntity.ok().build();
    }
    @GetMapping("/getOrderIds")
    public ResponseEntity<List<OrderRecord>> GetOrderIds(HttpSession session) {
        String username=(String) session.getAttribute("username");
        String usertype=(String) session.getAttribute("authority");

        List<OrderRecord>ret = null;
        if(Objects.equals(usertype, "User")) {
            ret=orderService.getOrder(username);
        }
        else if(Objects.equals(usertype, "Admin")){
            ret=orderService.getAllOrders();
        }
        return ResponseEntity.ok(ret);
    }
    @GetMapping("/getOrderItems")
    public ResponseEntity<List<OrderItem>> GetOrderItems(@RequestParam("oid") Integer oid) {
        System.out.println("get order id"+oid);
        OrderRecord orderRecord=orderService.getRecord(oid);
        List<OrderItem> orderItems=orderRecord.getOrderItems();
        return ResponseEntity.ok(orderItems);
    }
}
