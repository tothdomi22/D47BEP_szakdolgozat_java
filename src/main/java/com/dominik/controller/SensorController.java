package com.dominik.controller;


import com.dominik.entity.Sensor;
import com.dominik.service.SensorService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;
import java.util.List;

@RestController
@RequestMapping("/api/v1")
public class SensorController {

    @Autowired
    private SensorService sensorService;

    @GetMapping
    public List<Sensor> getAllSensorData() {
        return sensorService.getAllSensorData();
    }

    @PostMapping
    public Sensor createSensorData(@RequestBody Sensor sensor) {
        return sensorService.createSensorData(sensor);
    }

    @GetMapping("/latest")
    public Sensor getLatestSensorData() {
        return sensorService.getLatestData();
    }

}
