package com.dominik.service;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpEntity;
import org.springframework.http.HttpHeaders;
import org.springframework.http.HttpMethod;
import org.springframework.http.ResponseEntity;
import org.springframework.stereotype.Service;
import org.springframework.web.client.RestTemplate;

@Service
public class FastApiService {

    @Autowired
    private RestTemplate restTemplate;

    private final String FASTAPI_URL = "http://127.0.0.1:8000/predict";

    public String getPredictionFromFastAPI(double soilMoisture, double humidity, double temperature) {
        // Define the data to send to FastAPI
        String requestBody = String.format(
                "{\"Soil_Moisture\": %f, \"Humidity\": %f, \"Temperature\": %f}",
                soilMoisture, humidity, temperature);

        // Set headers
        HttpHeaders headers = new HttpHeaders();
        headers.set("Content-Type", "application/json");

        // Create the request entity
        HttpEntity<String> requestEntity = new HttpEntity<>(requestBody, headers);

        System.out.println(requestEntity);

        // Send a POST request to FastAPI
        ResponseEntity<String> response = restTemplate.exchange(
                FASTAPI_URL, HttpMethod.POST, requestEntity, String.class);

        // Return the response body (prediction result)
        return response.getBody();
    }
}

