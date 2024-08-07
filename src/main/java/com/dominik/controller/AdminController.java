package com.dominik.controller;

import com.dominik.entity.User;
import com.dominik.repository.UserRepository;
import com.dominik.service.UserDetailService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.servlet.view.RedirectView;

import java.util.List;

@RequestMapping("/admin")
@RestController
public class AdminController {

    @Autowired
    private UserRepository userRepository;

    @Autowired
    private UserDetailService userDetailService;

    @GetMapping("/users")
    public RedirectView usersList() {
        return new RedirectView("/users.html");
    }

    @PutMapping("/remove-admin/{id}")
    public User removeAdmin(@PathVariable("id") User user) {
        user.setRole("USER");
        userRepository.save(user);
        return user;
    }

    @PutMapping("/add-admin/{id}")
    public User addAdmin(@PathVariable("id") User user) {
        user.setRole("ADMIN");
        userRepository.save(user);
        return user;
    }

    @DeleteMapping("/delete/{id}")
    public void  deleteUser(@PathVariable("id") User user) {
        userRepository.delete(user);
    }

    @GetMapping("/users_list")
    public List<User> getAllUsers(){
        return userRepository.findAll();
    }
}
