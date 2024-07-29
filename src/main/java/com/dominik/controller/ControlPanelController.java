package com.dominik.controller;


import com.dominik.entity.ControlPanel;
import com.dominik.service.ControlPanelService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@RestController
@RequestMapping("/api/control-panel")
public class ControlPanelController {

    @Autowired
    private ControlPanelService controlPanelService;

    @GetMapping
    public List<ControlPanel> getAllData() {
        return controlPanelService.getAllData();
    }
}
