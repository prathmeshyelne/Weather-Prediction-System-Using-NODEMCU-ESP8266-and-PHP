#include "DHT.h"
int sensor = D2;
int val = 0;

const int DHTTYPE = DHT11;

// DHT Sensor
uint8_t DHTPin = D1;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

float Temperature;
float Humidity;

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "#"; //ENTER WIFI NAME
const char* password = "#"; //ENTER WIFI PASSWORD

//Your Domain name with URL path or IP address with path
String serverName = "#"; //ENTER SERVER URL

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
//unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

void setup() 
{
  Serial.begin(9600);
  pinMode(sensor, INPUT);
  pinMode(DHTPin, INPUT);
  dht.begin();
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");

}

void loop() 
{
  int val = digitalRead(sensor);

  Serial.print("Temperature in C : ");  
  Serial.println((dht.readTemperature( )));  
  Serial.print("Humidity in C : ");  
  Serial.println((dht.readHumidity())); 
  float temp = dht.readTemperature( );
  float hum = dht.readHumidity();
  // String temp = temp;
  // String hum = hum;
  
  String season;

if (val == 1) 
  {
    season = "Sunny";
    delay(800);
  }
  else
  {
    season = "Rainy";
    delay(800);
  }

    // Send an HTTP POST request depending on timerDelay
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;

      String serverPath = serverName + "?temp=" + temp + "&hum=" + hum + "&season=" + season;
      Serial.println(serverPath);
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
      }
      
      else 
      {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
