#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include<HTTPClient.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPIFFS.h>
#include <Adafruit_Sensor.h>

LiquidCrystal_I2C lcd(0X27,16,2); 

#define Doam 34
int frame;
int f;
const char* ssid = "Van Anh";     
const char* password   = "vananh2911";   

AsyncWebServer server(80);

String Web_App_URL = "https://script.google.com/macros/s/AKfycbzqB6nK12Py1oo2p2uL86NVr8Xprxs4XpW-JdKA7v2Eom55kQpLWtdVZ84yumOMtrdE/exec";

float Temp;
int Humd;

#define DHTPIN 19              // Digital pin connected to the DHT sensor

#define DHTTYPE DHT11          // DHT 11

DHT dht(DHTPIN, DHTTYPE);

String readDHTTemperature() {
  float t = dht.readTemperature();

  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

void setup() {
  Serial.begin(115200);
  dht.begin();
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  Serial.println(WiFi.localIP());
  lcd.init();                    
  lcd.backlight(); 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });
  server.on("/1", HTTP_GET, [](AsyncWebServerRequest *request){
    f = 1;             //frame = 1;
    request->send(SPIFFS, "/index.html");
  });
  server.on("/4", HTTP_GET, [](AsyncWebServerRequest *request){
    f = 4;            //frame = 4;
    request->send(SPIFFS, "/index.html");
  });
  server.on("/5", HTTP_GET, [](AsyncWebServerRequest *request){
    f = 5;            //frame = 5;
    request->send(SPIFFS, "/index.html");
  });
  server.on("/6", HTTP_GET, [](AsyncWebServerRequest *request){
    f = 6;            //frame = 6;
    request->send(SPIFFS, "/index.html");
  });
  server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request){
    // Xử lý dữ liệu được gửi từ trình duyệt ở đây
    if (request->hasArg("tree")) {
      String tree = request->arg("tree");
      String air_temp = request->arg("air_temp");
      String air_humidity = request->arg("air_humidity");
      String soil_humidity = request->arg("soil_humidity");

      // Xử lý dữ liệu nhận được ở đây
      Serial.println("Tree: " + tree);
      Serial.println("Air Temperature: " + air_temp);
      Serial.println("Air Humidity: " + air_humidity);
      Serial.println("Soil Humidity: " + soil_humidity);
    }
    request->send(200);
  });
  server.begin();
  pinMode(Doam, INPUT);
}
void lcdout() {
  int SoilHum = analogRead(Doam);
  SoilHum = map(SoilHum, 0, 4095, 0, 100);
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.setCursor(5, 0);
  lcd.print(Temp);
  lcd.setCursor(8, 0);
  lcd.print("Humd:");
  lcd.setCursor(13, 0);
  lcd.setCursor(0, 1);
  lcd.print("Soil:");
  lcd.setCursor(5, 0);
  lcd.print(SoilHum);
}
void loop() {
//  Getting_DHT11_Sensor_Data();
//  lcdout();
//    if (WiFi.status() == WL_CONNECTED) {
//
//      String Send_Data_URL = Web_App_URL;
//      Send_Data_URL += "&Temperature=" + String(Temp);
//      Send_Data_URL += "&Humidity=" + String(Humd);
//
//        HTTPClient http;
//    
//        // HTTP GET Request.
//        http.begin(Send_Data_URL.c_str());
//        http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
//    
//        // Gets the HTTP status code.
//        int httpCode = http.GET(); 
//        Serial.print("HTTP Status Code : ");
//        Serial.println(httpCode);
//    
//        // Getting response from google sheets.
//        String payload;
//        if (httpCode > 0) {
//          payload = http.getString();
//          Serial.println("Payload : " + payload);    
//        }
//        http.end();
//      delay(2000);
//    }
  if((f>=1)&&(f<3))
  {
    f++;
  }
  else if((f>=6)&&(f<11))
  {
    f++;  
  }
  else if( f == 4)
  {
    
  }
  else if( f == 5)
  {
   
  }
  else 
  {
    
  }
}
