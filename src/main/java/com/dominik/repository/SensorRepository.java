package com.dominik.repository;

import com.dominik.entity.Sensor;
import org.springframework.data.jpa.repository.JpaRepository;

import java.util.List;

public interface SensorRepository extends JpaRepository<Sensor, Integer>{

    Sensor findFirstByOrderByIdDesc();
    List<Sensor> findTop42ByOrderByIdDesc();
}
