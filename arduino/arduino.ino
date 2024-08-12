#include <WiFiS3.h>
#include <WiFiClient.h>
#include <DHT.h>


//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino


//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

// Replace with your network credentials
const char* ssid = "ssid";
const char* password = "password";

// Server details
const char* server = "192.168.0.102";  // Replace with your Spring Boot server IP
const int port = 8080; // Spring Boot server port



void setup() {
  Serial.begin(115200);
  dht.begin();




  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {


    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(hum);
    Serial.print(" %, Temp: ");
    Serial.print(temp);
    Serial.println(" Celsius");



  // Check if connected to WiFi
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;

    if (client.connect(server, port)) {
      Serial.println("Connected to server");

      String jsonPayload = "{\"temp\":" + String(temp) + ",\"hum\":" + String(hum) + "}";


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
  } else {
    Serial.println("WiFi not connected");
  }

  // Send data every 10 seconds
  delay(10000);
}
