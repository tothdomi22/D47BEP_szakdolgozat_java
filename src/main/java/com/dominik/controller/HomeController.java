package com.dominik.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.view.RedirectView;

@Controller
public class HomeController {

    @GetMapping("/user/hello")
    public String userHello() {
        return "hello user";
    }

    @GetMapping("/admin/hello")
    public String adminHello() {
        return "hello admin";
    }

    @GetMapping("/")
    public RedirectView serveIndex() {
        return new RedirectView("/index.html");
    }

    @GetMapping("/newregister")
    public RedirectView serverRegister() {
        return new RedirectView("/newregister.html");
    }

//    @GetMapping("/login")
//    public String handleLogin(){
//        return"/login.html";
//    }
}
