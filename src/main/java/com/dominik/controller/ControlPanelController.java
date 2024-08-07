package com.dominik.controller;


import com.dominik.entity.ControlPanel;
import com.dominik.repository.ControlPanelRepository;
import com.dominik.service.ControlPanelService;
import com.sun.tools.jconsole.JConsoleContext;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/control-panel")
public class ControlPanelController {

    @Autowired
    private ControlPanelService controlPanelService;
    @Autowired
    private ControlPanelRepository controlPanelRepository;

    @GetMapping
    public List<ControlPanel> getAllData() {
        return controlPanelService.getAllData();
    }

    @PostMapping("/update")
    public ControlPanel updateData(@RequestBody ControlPanel controlPanel) {
        return controlPanelRepository.save(controlPanel);
    }


}
