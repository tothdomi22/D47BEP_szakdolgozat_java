package com.dominik.controller;

import com.dominik.entity.User;
import com.dominik.repository.UserRepository;
import com.dominik.service.UserDetailService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.*;
import java.util.List;
import java.util.Optional;

@RestController
public class AuthController {

    @Autowired
    private UserRepository userRepository;
    @Autowired
    private UserDetailService userDetailService;

//    expects a json
//    @PostMapping("/register")
//    public User createUser(@RequestBody User user) {
//        user.setPassword(passwordEncoder.encode(user.getPassword()));
//        return userRepository.save(user);
//    }

    @PostMapping("/register")
    public User createUser(@ModelAttribute User user) {
        //user.setPassword(passwordEncoder.encode(user.getPassword()));
        userRepository.save(user);
        return user;
    }


    @PostMapping("/change-password/store")
        public User updateUser(@ModelAttribute User user) {
        return userDetailService.updateUser(user);
        }

    @GetMapping("/api/current-user")
    public UserDetails currentUser(@AuthenticationPrincipal UserDetails currentUser) {
        return currentUser;
    }



    @GetMapping("/admin/users_list")
    public List<String> getAllUsers(){
        return userRepository.getAllUsername();
    }
}
