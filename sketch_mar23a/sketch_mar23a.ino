#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "FPT Telecom-9A38";
const char* password = "00043D22";

WebServer server(80);

String generateInputRow(int index) {
  String row = "<tr>";
  row += "<td><input type='checkbox' name='select' value='" + String(index) + "' onchange='enableOnlyOne(this)'></td>";
  row += "<td><input type='text' name='plant" + String(index) + "'></td>";
  row += "<td><input type='text' name='temperature" + String(index) + "'></td>";
  row += "<td><input type='text' name='humidity_air" + String(index) + "'></td>";
  row += "<td><input type='text' name='humidity_soil" + String(index) + "'></td>";
  row += "</tr>";
  return row;
}

void handleRoot() {
  String content = "<html><body>";
  content += "<h1>Plant Monitoring</h1>";
  content += "<form action='/save' method='post'>";
  content += "<table border='1'><tr><th>Select</th><th>Plant</th><th>Temperature (°C)</th><th>Humidity Air (%)</th><th>Humidity Soil (%)</th></tr>";
  
  // Generate input rows
  for (int i = 0; i < 5; i++) {
    content += generateInputRow(i);
  }
  
  content += "</table>";
  content += "<br><input type='submit' value='Save'>";
  content += "<input type='button' value='Add' onclick='addRow()'>";
  content += "<input type='button' value='Remove' onclick='removeRow()'>";
  content += "</form>";
  content += "<script>function addRow() { window.location.href = '/add'; } function removeRow() { window.location.href = '/remove'; } function enableOnlyOne(checkbox) { var checkboxes = document.getElementsByName('select'); checkboxes.forEach(function(item) { if (item !== checkbox) item.checked = false; }); }</script>";
  content += "</body></html>";
  
  server.send(200, "text/html", content);
}

void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}

void handleAddRow() {
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleRemoveRow() {
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void handleSave() {
  for (int i = 0; i < 5; i++) {
    if (server.hasArg("select" + String(i))) {
      String plant = server.arg("plant" + String(i));
      String temperature = server.arg("temperature" + String(i));
      String humidity_air = server.arg("humidity_air" + String(i));
      String humidity_soil = server.arg("humidity_soil" + String(i));
      
      // Process the data as needed (e.g., send it to ESP32)
      Serial.print("Plant: ");
      Serial.println(plant);
      Serial.print("Temperature: ");
      Serial.println(temperature);
      Serial.print("Humidity Air: ");
      Serial.println(humidity_air);
      Serial.print("Humidity Soil: ");
      Serial.println(humidity_soil);
    }
  }
  
  server.sendHeader("Location", String("/"), true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/add", handleAddRow);
  server.on("/remove", handleRemoveRow);
  server.on("/save", HTTP_POST, handleSave);
  server.onNotFound(handleNotFound);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
