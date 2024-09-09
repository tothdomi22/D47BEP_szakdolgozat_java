#include <WiFiS3.h>
#include <WiFiClient.h>
#include <DHT.h>
#include <WebSocketServer.h>
#include <ArduinoJson.h>

using namespace net;

WebSocketServer wss(81);
WiFiServer server(80);

const char ssid[] = "SSID";  // Change to your network SSID
const char pass[] = "PASSWORD";     // Change to your network password

int status = WL_IDLE_STATUS;


//Constants
#define DHTPIN 2           // what pin we're connected to
#define DHTTYPE DHT22      // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);  //// Initialize DHT sensor for normal 16mhz Arduino
const int relayPin = A5;
const int dry = 810;
const int wet = 350;
const long interval = 30000;  // 30 seconds interval for data upload


//Variables
unsigned long previousMillis = 0;
int trigpin = 8;
int echopin = 9;
float hum;     //Stores humidity value
float temp;    //Stores temperature value
int moisture;  //Stores moisture value
int distance;
int luminance;
int wateringDuration = 0;
int wateringThreshold = 0;
int tankDepth = 0;

// Timing variables for periodic tasks
unsigned long lastTaskMillis = 0;
unsigned long taskInterval = 30000;  // 1 second interval for periodic tasks


// Server details
const char *springServer = "IP";  // Replace with your Spring Boot server IP
const int port = 8080;                 // Spring Boot server port



void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(relayPin, OUTPUT);

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(4000);  // Wait 4 seconds for connection
  }
  Serial.println("Connected to WiFi");


  // Print the IP address once connected
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket and WiFi servers
  wss.begin();
  server.begin();

  // Setup WebSocket event handlers
  wss.onConnection([](WebSocket &ws) {
    Serial.print("New WebSocket connection from: ");
    Serial.println(ws.getRemoteIP());

    ws.onMessage([](WebSocket &ws, const WebSocket::DataType dataType, const char *message, uint16_t length) {
      if (dataType == WebSocket::DataType::TEXT) {
        Serial.print("Received: ");
        Serial.println(message);  // Log the received message

        // Parse incoming JSON
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, message);
        if (error) {
          Serial.print("deserializeJson() failed: ");
          Serial.println(error.f_str());
          return;
        }

        // Update the variables
        if (doc.containsKey("wateringDuration")) {
          wateringDuration = doc["wateringDuration"];
        }
        if (doc.containsKey("wateringThreshold")) {
          wateringThreshold = doc["wateringThreshold"];
        }
        if (doc.containsKey("tankDepth")) {
          tankDepth = doc["tankDepth"];
        }

        // Send acknowledgment to the client
        ws.send(WebSocket::DataType::TEXT, "Data received and updated", strlen("Data received and updated"));

        // Log the updated variables
        Serial.print("Updated wateringDuration: ");
        Serial.println(wateringDuration);
        Serial.print("Updated wateringThreshold: ");
        Serial.println(wateringThreshold);
        Serial.print("Updated tankDepth: ");
        Serial.println(tankDepth);
      }
    });

    ws.onClose([](WebSocket &ws, const WebSocket::CloseCode, const char *, uint16_t) {
      Serial.println("WebSocket connection closed");
    });
  });
}

//function to calculate the distance
int getDistance() {
  int value;
  int duration;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  value = duration * 0.034 / 2;
  return value;
}

//function to start the watering
void doWatering(int pin) {
  digitalWrite(pin, LOW);
  delay(5000);
  digitalWrite(pin, HIGH);
}


void loop() {

  // Non-blocking WebSocket handling
  wss.listen();

  // Periodic tasks: check every taskInterval (1 second here)
  unsigned long currentMillis = millis();
  if (currentMillis - lastTaskMillis >= taskInterval) {
    lastTaskMillis = currentMillis;

    moisture = map(analogRead(A4), wet, dry, 100, 0);  //Reads the A4 pin for the humidity
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    distance = getDistance();
    luminance = analogRead(A0);

    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.print(" Celsius, Moisture: ");
    Serial.print(moisture);
    Serial.print(" %, Distance: ");
    Serial.print(distance);
    Serial.print(" Cm, Luminance: ");
    Serial.print(luminance);
    Serial.println(" LUM");


    // Check if connected to WiFi
    if (WiFi.status() == WL_CONNECTED) {
      WiFiClient client;
      if (client.connect(springServer, port)) {
        Serial.println("Connected to server");
        String jsonPayload = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(hum) + ",\"moisture\":" + String(moisture) + ",\"waterLevel\":" + String(distance) + ",\"light\":" + String(luminance) + "}";

        // Create the HTTP POST request
        client.println("POST /api/v1/data HTTP/1.1");
        client.println("Host: 192.168.0.102");
        client.println("Content-Type: application/json");
        client.print("Content-Length: ");
        client.println(jsonPayload.length());
        client.println();
        client.println(jsonPayload);

        // Wait for the response
        while (client.connected()) {
          String line = client.readStringUntil('\n');
          if (line == "\r") {
            break;
          }
        }

        // Read the response
        String response = client.readString();
        Serial.println("Server response: " + response);

        // Close the connection
        client.stop();
      } else {
        Serial.println("Connection to server failed");
      }
    }

    /*if (moisture < 50) {
      doWatering(relayPin);
    }*/
  }

  // Allow the processor to handle other tasks
  delay(1);  // Prevent the watchdog from resetting, if applicable
}
