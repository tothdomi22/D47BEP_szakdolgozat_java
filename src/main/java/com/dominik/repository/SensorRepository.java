package com.dominik.repository;

import com.dominik.entity.Sensor;
import org.springframework.data.jpa.repository.JpaRepository;

public interface SensorRepository extends JpaRepository<Sensor, Integer>{

    Sensor findFirstByOrderByIdDesc();
}
