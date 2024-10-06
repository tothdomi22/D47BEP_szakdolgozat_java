package com.dominik.service;


import com.dominik.entity.Sensor;
import com.dominik.repository.SensorRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import java.time.Instant;
import java.util.List;

@Service
public class SensorService {

    @Autowired
    private SensorRepository sensorRepository;

    public List<Sensor> getAllSensorData() {
        return sensorRepository.findAll();
    }


    //this is the method used to store the data
    public Sensor createSensorData(Sensor sensor) {
        Instant instant = Instant.now();
        sensor.setCreatedAt(instant);
        sensor.setUpdateAt(instant);
        return sensorRepository.save(sensor);
    }

    public Sensor getLatestData() {
        return sensorRepository.findFirstByOrderByIdDesc();
    }

    public List<Sensor> getLastWeekData() {
        return sensorRepository.findTop42ByOrderByIdDesc();
    }



}
