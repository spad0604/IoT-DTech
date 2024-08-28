
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN 19         // Chân kết nối cảm biến DHT11

const char* ssid = "FPT Telecom-9A38";
const char* password = "00043D22";

AsyncWebServer server(80);

String htmlTemplate = R"html(
  <html>
    <body>
      <h2>Plant Information:</h2>
      <form method='POST' action='/add'>
        <table id='plant-table'>O
          <tr>
            <th>Plant Name</th>
            <th>Temperature</th>
            <th>Air Moisture</th>
            <th>Soil Moisture</th>
            <th>Select Plant</th>
          </tr>
          <tr>
            <td><input type='text' name='plantName[]' placeholder='Plant Name'></td>
            <td><input type='text' name='temperature[]' placeholder='Temperature'></td>
            <td><input type='text' name='airMoisture[]' placeholder='Air Moisture'></td>
            <td><input type='text' name='soilMoisture[]' placeholder='Soil Moisture'></td>
            <td><input type='checkbox' name='selectedPlant[]'></td>
          </tr>
        </table>
        <button type='button' onclick='addPlantRow()'>Add</button>
        <input type='submit' value='Save'>
        <button type='button' onclick='Infor()'>Information</button>
      </form>

      <div id='chart_div'></div>

      <script>
        function addPlantRow() {
          var plantTable = document.getElementById('plant-table');
          var newRow = document.createElement('tr');

          var plantNameCell = document.createElement('td');
          var plantNameInput = document.createElement('input');
          plantNameInput.type = 'text';
          plantNameInput.name = 'plantName[]';
          plantNameInput.placeholder = 'Plant Name';
          plantNameCell.appendChild(plantNameInput);
          newRow.appendChild(plantNameCell);

          var temperatureCell = document.createElement('td');
          var temperatureInput = document.createElement('input');
          temperatureInput.type = 'text';
          temperatureInput.name = 'temperature[]';
          temperatureInput.placeholder = 'Temperature';
          temperatureCell.appendChild(temperatureInput);
          newRow.appendChild(temperatureCell);

          var airMoistureCell = document.createElement('td');
          var airMoistureInput = document.createElement('input');
          airMoistureInput.type = 'text';
          airMoistureInput.name = 'airMoisture[]';
          airMoistureInput.placeholder = 'Air Moisture';
          airMoistureCell.appendChild(airMoistureInput);
          newRow.appendChild(airMoistureCell);

          var soilMoistureCell = document.createElement('td');
          var soilMoistureInput = document.createElement('input');
          soilMoistureInput.type = 'text';
          soilMoistureInput.name = 'soilMoisture[]';
          soilMoistureInput.placeholder = 'Soil Moisture';
          soilMoistureCell.appendChild(soilMoistureInput);
          newRow.appendChild(soilMoistureCell);

          var selectedPlantCell = document.createElement('td');
          var selectedPlantCheckbox = document.createElement('input');
          selectedPlantCheckbox.type = 'checkbox';
          selectedPlantCheckbox.name = 'selectedPlant[]';
          selectedPlantCell.appendChild(selectedPlantCheckbox);
          newRow.appendChild(selectedPlantCell);

          plantTable.appendChild(newRow);

          function
        }
      </script>
    </body>
  </html>
)html";

DHT dht(DHTPIN, DHT11);

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/html", htmlTemplate);
  });

  server.on("/add", HTTP_POST, [](AsyncWebServerRequest *request){
    int params = request->params();
    String message = "Plant information saved:<br>";

    for (int i = 0; i < params; i += 5) {
      String plantName = request->getParam(i)->value();
      String temperature = request->getParam(i + 1)->value();
      String airMoisture = request->getParam(i + 2)->value();
      String soilMoisture = request->getParam(i + 3)->value();
      String selectedPlant = request->getParam(i + 4)->value();

      // Do something with the plant information
      // For example, store it in a database or perform actions based on the values

      message += "Plant Name: " + plantName + "<br>";
      message += "Temperature: " + temperature + "<br>";
      message += "Air Moisture: " + airMoisture + "<br>";
      message += "Soil Moisture: " + soilMoisture + "<br>";
      message += "Selected: " + selectedPlant + "<br>";
      message += "<br>";
    }

    request->send(200, "text/html", message);
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request){
    const int bufferSize = JSON_OBJECT_SIZE(2);
    StaticJsonDocument<bufferSize> jsonDocument;
    JsonArray jsonArray = jsonDocument.to<JsonArray>();

    // Read DHT11 sensor values
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (!isnan(temperature) && !isnan(humidity)) {
      JsonObject data = jsonArray.createNestedObject();
      data["timestamp"] = millis();
      data["temperature"] = temperature;
      data["humidity"] = humidity;
    }

    String jsonData;
    serializeJson(jsonArray, jsonData);

    request->send(200, "application/json", jsonData);
  });

  server.begin();
}

void loop() {
  // Đọc dữ liệu từ cảm biến DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // In giá trị đọc được lên Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  delay(2000);
}