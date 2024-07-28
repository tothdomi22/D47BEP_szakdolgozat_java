package com.dominik;


import com.dominik.entity.Sensor;
import com.dominik.repository.SensorRepository;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.List;

@SpringBootApplication
@EnableJpaAuditing
@RestController
@RequestMapping("/api/v2")
public class Main {

    private final SensorRepository sensorRepository;

    public Main(SensorRepository sensorRepository) {
        this.sensorRepository = sensorRepository;
    }

    public static void main(String[] args) {
        SpringApplication.run(Main.class, args);
    }

    @GetMapping
    public List<Sensor> getSensorData() {
        return sensorRepository.findAll();
    }

}
