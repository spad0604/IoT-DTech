#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include<DHT.h>
#define DHTPIN  19
#define DHTTYPE DHT11

const char* ssid = "FPT Telecom-9A38"; 
const char* password = "00043D22"; 

String Web_App_URL = "https://script.google.com/macros/s/AKfycbzqB6nK12Py1oo2p2uL86NVr8Xprxs4XpW-JdKA7v2Eom55kQpLWtdVZ84yumOMtrdE/exec";

float Temp;
int Humd;

DHT dht11(DHTPIN, DHTTYPE);

void Getting_DHT11_Sensor_Data() {
  
  Humd = dht11.readHumidity();
  
  Temp = dht11.readTemperature();
  
  if (isnan(Humd) || isnan(Temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    Temp = 0.00;
    Humd = 0;
  } 
}

void setup() {

  Serial.begin(115200);
  delay(1000);
  WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {

  }

  Serial.println("WiFi connected");

  dht11.begin();

}

void loop() {
  Getting_DHT11_Sensor_Data();
  if (WiFi.status() == WL_CONNECTED) {

    String Send_Data_URL = Web_App_URL;
    Send_Data_URL += "&Temperature=" + String(Temp);
    Send_Data_URL += "&Humidity=" + String(Humd);

      HTTPClient http;
  
      // HTTP GET Request.
      http.begin(Send_Data_URL.c_str());
      http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  
      // Gets the HTTP status code.
      int httpCode = http.GET(); 
      Serial.print("HTTP Status Code : ");
      Serial.println(httpCode);
  
      // Getting response from google sheets.
      String payload;
      if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload : " + payload);    
      }
      
      http.end();
  }
  delay(2000);
}

