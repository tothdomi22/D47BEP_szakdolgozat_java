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
        //setting fix values because those sensors haven't been installed yet
        //and it can't be NULL
        sensor.setLight(400);
        sensor.setWaterLevel(43);
        sensor.setMoisture(54);
        return sensorRepository.save(sensor);
    }

    public Sensor getLatestData() {
        return sensorRepository.findFirstByOrderByIdDesc();
    }



}
