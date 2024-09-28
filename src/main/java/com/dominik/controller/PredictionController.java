package com.dominik.controller;

import com.dominik.service.FastApiService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

@RestController
@RequestMapping("/api")
public class PredictionController {

    @Autowired
    private FastApiService fastApiService;

    @PostMapping("/predict")
    public String getPrediction(
            @RequestParam double soilMoisture,
            @RequestParam double humidity,
            @RequestParam double temperature) {

        // Call the FastAPI service and get the prediction
        String prediction = fastApiService.getPredictionFromFastAPI(soilMoisture, humidity, temperature);
        return prediction;
    }
}
