package com.dominik.controller;

import com.dominik.entity.User;
import com.dominik.repository.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.core.annotation.AuthenticationPrincipal;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.ModelAttribute;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.view.RedirectView;

import java.util.List;
import java.util.Optional;

@RestController
public class AuthController {

    @Autowired
    private UserRepository userRepository;
    @Autowired
    private PasswordEncoder passwordEncoder;

//    expects a json
//    @PostMapping("/register")
//    public User createUser(@RequestBody User user) {
//        user.setPassword(passwordEncoder.encode(user.getPassword()));
//        return userRepository.save(user);
//    }

    @PostMapping("/register")
    public RedirectView createUser(@ModelAttribute User user) {
        user.setPassword(passwordEncoder.encode(user.getPassword()));
        userRepository.save(user);
        return new RedirectView("/index.html");  // Redirect after successful registration
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
