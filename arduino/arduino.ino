#include <WiFiS3.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <WebSocketServer.h>
#include <ArduinoJson.h>

using namespace net;

// Constants
#define DHTPIN 2       // Pin where DHT sensor is connected
#define DHTTYPE DHT22  // Type of DHT sensor
#define relayPin A5    // Pin for the relay
#define dry 810        // Dry threshold for the moisture sensor
#define wet 350        // Wet threshold for the moisture sensor

DHT dht(DHTPIN, DHTTYPE);
WebSocketServer wss(81);
WiFiServer server(80);

// WiFi Credentials
const char ssid[] = "****";  // Change to your WiFi SSID
const char pass[] = "*****";     // Change to your WiFi password

// Variables for sensor readings
float hum;      // Stores humidity value
float temp;     // Stores temperature value
int moisture;   // Stores moisture value
int distance;   // Stores distance from ultrasonic sensor
int luminance;  // Stores luminance value
int wateringDuration = 5;
int wateringThreshold = 0;
int tankDepth = 0;

// Timing variables for periodic tasks
unsigned long lastTaskMillis = 0;
unsigned long taskInterval = 14400000;  // 4 hour interval for periodic tasks

// Server details for the Spring Boot server
const char *springServer = "192.168.0.106";  // Replace with your Spring Boot server IP
const int springPort = 8080;                 // Spring Boot server port

// Function to calculate the distance using ultrasonic sensor
int getDistance() {
  int duration;
  digitalWrite(8, LOW);
  delayMicroseconds(2);
  digitalWrite(8, HIGH);
  delayMicroseconds(10);
  digitalWrite(8, LOW);
  duration = pulseIn(9, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Function to perform the watering task
void doWatering(int pin) {
  Serial.println("WATERING!");
  digitalWrite(pin, LOW);   // Turn the relay on
  delay(wateringDuration*1000);              // Water for 5 seconds (adjust as needed)
  digitalWrite(pin, HIGH);  // Turn the relay off
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(8, OUTPUT);  // Trigger pin for ultrasonic sensor
  pinMode(9, INPUT);   // Echo pin for ultrasonic sensor
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  // Connect to WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(4000);
  }
  Serial.println("Connected to WiFi");

  // Print the IP address
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket and WiFi servers
  wss.begin();
  server.begin();

  // WebSocket event handlers
  wss.onConnection([](WebSocket &ws) {
    Serial.print("New WebSocket connection from: ");
    Serial.println(ws.getRemoteIP());

    ws.onMessage([](WebSocket &ws, const WebSocket::DataType dataType, const char *message, uint16_t length) {
      if (dataType == WebSocket::DataType::TEXT) {
        Serial.print("Received: ");
        Serial.println(message);

        // Parse incoming JSON
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.f_str());
          return;
        }

        // Check if it's a watering command
        if (doc.containsKey("command") && doc["command"] == "doWatering") {
          Serial.println("Triggering watering...");
          doWatering(relayPin);
          ws.send(WebSocket::DataType::TEXT, "Plant watered successfully", strlen("Plant watered successfully"));
        }

        // Update other variables from the JSON message
        if (doc.containsKey("wateringDuration")) {
          wateringDuration = doc["wateringDuration"];
          ws.send(WebSocket::DataType::TEXT, "Data received and processed", strlen("Data received and processed"));
        }
        if (doc.containsKey("wateringThreshold")) {
          wateringThreshold = doc["wateringThreshold"];
        }
        if (doc.containsKey("tankDepth")) {
          tankDepth = doc["tankDepth"];

          // Log updated variables
          Serial.print("Updated wateringDuration: ");
          Serial.println(wateringDuration);
          Serial.print("Updated wateringThreshold: ");
          Serial.println(wateringThreshold);
          Serial.print("Updated tankDepth: ");
          Serial.println(tankDepth);
        }
      }
    });

    ws.onClose([](WebSocket &ws, const WebSocket::CloseCode, const char *, uint16_t) {
      Serial.println("WebSocket connection closed");
    });
  });
}

void loop() {
  // Handle WebSocket connections
  wss.listen();

  // Periodic sensor reading and logging
  unsigned long currentMillis = millis();
  if (currentMillis - lastTaskMillis >= taskInterval) {
    lastTaskMillis = currentMillis;

    moisture = map(analogRead(A4), wet, dry, 100, 0);  // Reads moisture sensor
    hum = dht.readHumidity();                          // Reads humidity
    temp = dht.readTemperature();                      // Reads temperature
    distance = getDistance();                          // Reads ultrasonic distance
    luminance = analogRead(A0);                        // Reads luminance sensor

    // Print sensor data to the Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.print(" C, Moisture: ");
    Serial.print(moisture);
    Serial.print(" %, Distance: ");
    Serial.print(distance);
    Serial.print(" Cm, Luminance: ");
    Serial.print(luminance);
    Serial.print(" LUM,  watering %: ");
    Serial.println(wateringThreshold);

    // Check if moisture is below threshold and start watering if needed
    if (moisture < wateringThreshold) {
      doWatering(relayPin);
    }

    // If connected to WiFi, upload data to the Spring Boot server
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      if (client.connect(springServer, springPort)) {
        Serial.println("Connected to Spring Boot server");

        // Create JSON payload
        String jsonPayload = "{\"temperature\":" + String(temp)
                              + ",\"humidity\":" + String(hum)
                              + ",\"moisture\":" + String(moisture)
                              + ",\"waterLevel\":" + String(distance)
                              + ",\"light\":" + String(luminance) + "}";

        // Send HTTP POST request
        client.println("POST /api/v1/data HTTP/1.1");
        client.println("Host: " + String(springServer));
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(jsonPayload.length());
        client.println();
        client.println(jsonPayload);

        // Wait for response
        while (client.connected()) {
          String line = client.readStringUntil('\n');
          if (line == "\r") {
            break;
          }
        }

        // Read response from server
        String response = client.readString();
        Serial.println("Server response: " + response);

        // Close the connection
        client.stop();
      } else {
        Serial.println("Failed to connect to Spring Boot server");
      }
    }
  }

  // Allow the processor to handle other tasks
  delay(1);  // Prevent watchdog resets, if applicable
}
