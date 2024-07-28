package com.dominik.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;


import java.time.Instant;

@AllArgsConstructor
@NoArgsConstructor
@Data
public class SensorDTO {

    private Integer id;

    private Double temperature;

    private Double humidity;

    private Integer moisture;

    private Integer water_level;

    private Integer light;

    private Instant created_at;

    private Instant update_at;
}
