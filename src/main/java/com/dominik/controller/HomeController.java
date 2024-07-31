package com.dominik.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RestController;
import org.springframework.web.servlet.view.RedirectView;

@Controller
public class HomeController {

    @GetMapping("/test")
    public RedirectView getControlPanel() {
        return new RedirectView("/control-panel.html");
    }

    @GetMapping("/xd")
    public String hello(){
        return "test";
    }
}
