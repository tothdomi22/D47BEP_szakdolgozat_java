package com.dominik.controller;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.servlet.view.RedirectView;

@Controller
public class HomeController {

    @GetMapping("/")
    public RedirectView serveIndex() {
        return new RedirectView("/index.html");
    }

    @GetMapping("/newregister")
    public RedirectView serverRegister() {
        return new RedirectView("/newregister.html");
    }

    @GetMapping("/change-password")
    public RedirectView serveChangePassword() {
        return new RedirectView("/change-password.html");
    }

}
