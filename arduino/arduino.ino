#include <WiFiS3.h>
#include <WiFiClient.h>
#include <DHT.h>


//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
const int relayPin = A5;
const int dry = 810;
const int wet = 350;
const long interval = 30000;  // 30 seconds interval for data upload


//Variables
unsigned long previousMillis = 0;
int trigpin = 8;
int echopin = 9;
float hum;  //Stores humidity value
float temp; //Stores temperature value
int moisture; //Stores moisture value
int distance;
int luminance;


// Replace with your network credentials
const char* ssid = "SSID";
const char* password = "PASSWORD";

// Server details
const char* server = "IP";  // Replace with your Spring Boot server IP
const int port = 8080; // Spring Boot server port



void setup() {
  Serial.begin(115200);
  dht.begin();
  pinMode (trigpin, OUTPUT);
  pinMode (echopin, INPUT);
  pinMode (relayPin, OUTPUT);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

//function to calculate the distance
int getDistance(){
  int value;
  int duration;
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn (echopin, HIGH);
  value = duration * 0.034 / 2;
  return value;
}

//function to start the watering
void doWatering (int pin) {
  digitalWrite (pin, LOW);
  delay(5000);
  digitalWrite (pin, HIGH);
}


void loop() {
    unsigned long currentMillis = millis();
    // Check if it's time to upload data
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        Serial.print("Previous millis: ");
        Serial.println(previousMillis);
        Serial.print("Current Millis: ");
        Serial.println(currentMillis);
        moisture = map(analogRead(A4), wet, dry, 100, 0); //Reads the A4 pin for the humidity
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
            if (client.connect(server, port)) {
                Serial.println("Connected to server");
                String jsonPayload = "{\"temperature\":" + String(temp) + ",\"humidity\":" + String(hum) + ",\"moisture\":" +
                String(moisture) + ",\"waterLevel\":" + String(distance) +  ",\"light\":" + String(luminance) +"}";

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
                }
            else {
                Serial.println("Connection to server failed");
            }
        }
        else {
            Serial.println("WiFi not connected");
        }

        if (moisture < 50) {
            doWatering(relayPin);
        }
    }
}
