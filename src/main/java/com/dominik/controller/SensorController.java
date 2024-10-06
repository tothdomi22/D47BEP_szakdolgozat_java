package com.dominik.controller;


import com.dominik.entity.Sensor;
import com.dominik.repository.SensorRepository;
import com.dominik.service.SensorService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/v1")
public class SensorController {

    @Autowired
    private SensorService sensorService;
    @Autowired
    private SensorRepository sensorRepository;

    @GetMapping
    public List<Sensor> getAllSensorData() {
        return sensorService.getAllSensorData();
    }

    @PostMapping("/data")
    public ResponseEntity<String> storeData(@RequestBody Sensor sensor) {
        sensorService.createSensorData(sensor);
        return ResponseEntity.ok("Data uploaded");
    }

    @PostMapping
    public Sensor createSensorData(@RequestBody Sensor sensor) {
        return sensorService.createSensorData(sensor);
    }

    @GetMapping("/latest")
    public Sensor getLatestSensorData() {
        return sensorService.getLatestData();
    }

    @GetMapping("/lastweek")
    public List<Sensor> getLastWeekData() {
        return sensorService.getLastWeekData();
    }

}
