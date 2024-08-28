
#include <DHT.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_Sensor.h>

#define DHTPIN 19         // Chân kết nối cảm biến DHT11
#define DHTTYPE DHT11     // Loại cảm biến DHT11

const char* ssid = "FPT Telecom-9A38";       // Tên mạng WiFi
const char* password = "00043D22";   // Mật khẩu mạng WiFi

DHT dht(DHTPIN, DHTTYPE);
WebServer server(80);


float temperature = 0;
float humidity = 0;

void handleRoot() {
  String webpage = "<html><body>";
  webpage += "<h1>Temperature and Humidity</h1>";
  webpage += "<p>Temperature: " + String(temperature) + " &#8451;</p>";
  webpage += "<p>Humidity: " + String(humidity) + " %</p>";
  webpage += "<canvas id=\"chart\"></canvas>";
  webpage += "<script src=\"https://cdn.jsdelivr.net/npm/chart.js\"></script>";
  webpage += "<script>";
  webpage += "var ctx = document.getElementById('chart').getContext('2d');";
  webpage += "var data = {";
  webpage += "labels: ['January', 'February', 'March', 'April', 'May', 'June', 'July'],";
  webpage += "datasets: [{";
  webpage += "label: 'Temperature',";
  webpage += "backgroundColor: 'rgba(255, 99, 132, 0.2)',";
  webpage += "borderColor: 'rgba(255, 99, 132, 1)',";
  webpage += "data: [20, 25, 22, 23, 21, 24, 19]";
  webpage += "}, {";
  webpage += "label: 'Humidity',";
  webpage += "backgroundColor: 'rgba(54, 162, 235, 0.2)',";
  webpage += "borderColor: 'rgba(54, 162, 235, 1)',";
  webpage += "data: [50, 55, 52, 53, 51, 54, 49]";
  webpage += "}]";
  webpage += "};";
  webpage += "var chart = new Chart(ctx, {";
  webpage += "type: 'line',";
  webpage += "data: data";
  webpage += "});";
  webpage += "</script>";
  webpage += "</body></html>";

  server.send(200, "text/html", webpage);
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  dht.begin();

  server.on("/", handleRoot);
  server.begin();

}

void loop() {
  server.handleClient();

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  Serial.print(temperature);
  Serial.print(" ");
  Serial.println(humidity);
  delay(2000);
}