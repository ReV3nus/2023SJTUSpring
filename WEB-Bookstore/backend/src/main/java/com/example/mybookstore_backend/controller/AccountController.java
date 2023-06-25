package com.example.mybookstore_backend.controller;


import com.example.mybookstore_backend.entity.User;
import com.example.mybookstore_backend.entity.UserAuth;
import com.example.mybookstore_backend.service.AuthService;
import com.example.mybookstore_backend.service.UserService;
import jakarta.servlet.http.HttpSession;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;
import java.util.Map;
import java.util.Objects;

@RestController
@RequestMapping("/apiAccount")
public class AccountController {
    @Autowired
    private AuthService authService;
    @Autowired
    private UserService userService;

    @GetMapping("/checkLogin")
    public ResponseEntity<String> checkLogin(HttpSession session) {
        String username=(String) session.getAttribute("username");
        if(username!=null)
        {
            return ResponseEntity.ok(username);
        }
        else
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
    }
    @GetMapping("/checkAdmin")
    public ResponseEntity<String> checkAdmin(HttpSession session) {
        String auth=(String) session.getAttribute("authority");
        if(auth!=null)
        {
            return ResponseEntity.ok(auth);
        }
        else
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).build();
    }
    @PostMapping("/login")
    public ResponseEntity<String> Login(@RequestBody Map<String, String> loginInfo, HttpSession session) {
        String username = loginInfo.get("username");
        String password = loginInfo.get("password");
        UserAuth User=authService.findUser(username);
        if(!Objects.equals(User.getPassword(), password))
        {
            String error="Wrong Password";
            return ResponseEntity.status(HttpStatus.UNAUTHORIZED).body(error);
        }
        session.setAttribute("username",username);
        session.setAttribute("authority",User.getUsertype());
        return ResponseEntity.ok(User.getUsertype());
    }
    @PostMapping("/checkName")
    public ResponseEntity<String> CheckName(@RequestBody Map<String, String> username, HttpSession session) {
        UserAuth user=authService.findUser(username.get("username"));
        if(user==null)return ResponseEntity.ok("NO");
        return ResponseEntity.ok("YES");
    }
    @PostMapping("/logout")
    public ResponseEntity<String> Logout(HttpSession session) {
        session.removeAttribute("username");
        session.removeAttribute("authority");
        return ResponseEntity.ok().build();
    }
    @PostMapping("/register")
    public ResponseEntity<String> Register(@RequestBody Map<String,String> registerInfo ,HttpSession session) {
        String username = (String) registerInfo.get("username");
        String password = (String) registerInfo.get("password");
        String email = (String) registerInfo.get("email");
        UserAuth userAuth=new UserAuth(username,password);
        User user=new User(username,email);
        user.setUserAuth(userAuth);
        userAuth.setUser(user);
        authService.AddAuth(userAuth);
//        userService.AddUser(user);
        return ResponseEntity.ok().build();
    }
    @PostMapping("/setstate")
    public ResponseEntity<String> SetState(@RequestBody Map<String,String> request ,HttpSession session) {
        String username = (String) request.get("username");
        String usertype = (String) request.get("usertype");
        System.out.println("Got username: "+username+" usertype: "+usertype);
        UserAuth userAuth=authService.findUser(username);
        if(userAuth==null)return ResponseEntity.notFound().build();
        userAuth.setUsertype(usertype);
        authService.AddAuth(userAuth);
        return ResponseEntity.ok().build();
    }
    @GetMapping("/getUsers")
    public ResponseEntity<List<UserAuth>> GetUsers(HttpSession session) {
        List<UserAuth> users=authService.GetAllUsers();
        return ResponseEntity.ok(users);
    }

}