package com.dominik.service;

import com.dominik.entity.ControlPanel;
import com.dominik.repository.ControlPanelRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.util.List;
import java.util.Optional;

@Service
public class ControlPanelService {

    @Autowired
    private ControlPanelRepository controlPanelRepository;

    public List<ControlPanel> getAllData() {
        return controlPanelRepository.findAll();
    }
}
