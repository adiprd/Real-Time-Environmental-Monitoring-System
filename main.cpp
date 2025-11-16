#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

// Konfigurasi WiFi
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Konfigurasi Sensor DHT
#define DHT_PIN 4
#define DHT_TYPE DHT11
DHT dht(DHT_PIN, DHT_TYPE);

// Konfigurasi Sensor Getar (Vibration)
#define VIBRATION_PIN 2

// Web Server pada port 80
WebServer server(80);

// Variabel untuk menyimpan data sensor
float temperature = 0;
float humidity = 0;
int vibration = 0;
int lightIntensity = 0;

// Variabel timing
unsigned long previousMillis = 0;
const long interval = 2000; // Interval pembacaan sensor (2 detik)

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi sensor
  dht.begin();
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(34, INPUT); // Pin untuk sensor cahaya (LDR)
  
  // Koneksi WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP Address: ");
  Serial.println(WiFi.localIP());

  // Setup routes web server
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/sensor", handleSensorData);
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    readSensors();
  }
}

void readSensors() {
  // Baca sensor DHT11
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  // Baca sensor getar
  vibration = digitalRead(VIBRATION_PIN);
  
  // Baca sensor cahaya (LDR di pin 34)
  lightIntensity = analogRead(34);
  
  // Cek jika ada error membaca DHT
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    temperature = 0;
    humidity = 0;
  }
  
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C, Humidity: ");
  Serial.print(humidity);
  Serial.print("%, Vibration: ");
  Serial.print(vibration);
  Serial.print(", Light: ");
  Serial.println(lightIntensity);
}

void handleRoot() {
  String html = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
    <title>Sistem Monitoring Sensor Real-Time</title>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <style>
      body { 
        font-family: Arial, sans-serif; 
        margin: 20px; 
        background-color: #f5f5f5;
      }
      .container { 
        max-width: 1200px; 
        margin: 0 auto; 
        background: white; 
        padding: 20px; 
        border-radius: 10px; 
        box-shadow: 0 2px 10px rgba(0,0,0,0.1);
      }
      .header { 
        text-align: center; 
        margin-bottom: 30px; 
        color: #333;
      }
      .sensor-grid { 
        display: grid; 
        grid-template-columns: repeat(auto-fit, minmax(250px, 1fr)); 
        gap: 20px; 
        margin-bottom: 30px;
      }
      .sensor-card { 
        background: linear-gradient(135deg, #667eea 0%, #764ba2 100%); 
        color: white; 
        padding: 20px; 
        border-radius: 10px; 
        text-align: center;
      }
      .sensor-value { 
        font-size: 2.5em; 
        font-weight: bold; 
        margin: 10px 0;
      }
      .sensor-unit { 
        font-size: 1em; 
        opacity: 0.8;
      }
      .chart-container { 
        background: white; 
        padding: 20px; 
        border-radius: 10px; 
        margin-top: 20px; 
        box-shadow: 0 2px 5px rgba(0,0,0,0.1);
      }
      .status { 
        padding: 10px; 
        border-radius: 5px; 
        margin: 10px 0; 
        text-align: center;
      }
      .online { 
        background: #d4edda; 
        color: #155724; 
        border: 1px solid #c3e6cb;
      }
    </style>
  </head>
  <body>
    <div class="container">
      <div class="header">
        <h1>🖥️ Sistem Monitoring Sensor Real-Time</h1>
        <p>Monitoring Data Lingkungan Secara Live</p>
        <div class="status online">🟢 Sistem Online - Data Real-Time</div>
      </div>
      
      <div class="sensor-grid">
        <div class="sensor-card">
          <h3>🌡️ Suhu</h3>
          <div class="sensor-value" id="temperature">--</div>
          <div class="sensor-unit">°C</div>
        </div>
        <div class="sensor-card">
          <h3>💧 Kelembaban</h3>
          <div class="sensor-value" id="humidity">--</div>
          <div class="sensor-unit">%</div>
        </div>
        <div class="sensor-card">
          <h3>📳 Getaran</h3>
          <div class="sensor-value" id="vibration">--</div>
          <div class="sensor-unit">Status</div>
        </div>
        <div class="sensor-card">
          <h3>💡 Intensitas Cahaya</h3>
          <div class="sensor-value" id="light">--</div>
          <div class="sensor-unit">LUX</div>
        </div>
      </div>
      
      <div class="chart-container">
        <h3>📊 Grafik Data Sensor</h3>
        <canvas id="sensorChart"></canvas>
      </div>
      
      <div style="text-align: center; margin-top: 20px; color: #666;">
        <p>Update Terakhir: <span id="lastUpdate">--</span></p>
      </div>
    </div>

    <script>
      let sensorChart;
      let chartData = {
        labels: [],
        temperature: [],
        humidity: [],
        light: []
      };
      
      // Inisialisasi chart
      function initializeChart() {
        const ctx = document.getElementById('sensorChart').getContext('2d');
        sensorChart = new Chart(ctx, {
          type: 'line',
          data: {
            labels: chartData.labels,
            datasets: [
              {
                label: 'Suhu (°C)',
                data: chartData.temperature,
                borderColor: 'rgb(255, 99, 132)',
                backgroundColor: 'rgba(255, 99, 132, 0.1)',
                tension: 0.4
              },
              {
                label: 'Kelembaban (%)',
                data: chartData.humidity,
                borderColor: 'rgb(54, 162, 235)',
                backgroundColor: 'rgba(54, 162, 235, 0.1)',
                tension: 0.4
              },
              {
                label: 'Cahaya (LUX)',
                data: chartData.light,
                borderColor: 'rgb(255, 205, 86)',
                backgroundColor: 'rgba(255, 205, 86, 0.1)',
                tension: 0.4
              }
            ]
          },
          options: {
            responsive: true,
            scales: {
              y: {
                beginAtZero: true
              }
            }
          }
        });
      }
      
      // Update data sensor
      function updateSensorData() {
        fetch('/sensor')
          .then(response => response.json())
          .then(data => {
            // Update nilai sensor
            document.getElementById('temperature').textContent = data.temperature.toFixed(1);
            document.getElementById('humidity').textContent = data.humidity.toFixed(1);
            document.getElementById('vibration').textContent = data.vibration ? 'Terdeteksi' : 'Aman';
            document.getElementById('light').textContent = data.light;
            
            // Update waktu terakhir
            document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
            
            // Update chart data (maksimal 20 data point)
            const now = new Date().toLocaleTimeString();
            chartData.labels.push(now);
            chartData.temperature.push(data.temperature);
            chartData.humidity.push(data.humidity);
            chartData.light.push(data.light / 10); // Scale down for chart
            
            if (chartData.labels.length > 20) {
              chartData.labels.shift();
              chartData.temperature.shift();
              chartData.humidity.shift();
              chartData.light.shift();
            }
            
            sensorChart.update();
          })
          .catch(error => console.error('Error fetching sensor data:', error));
      }
      
      // Inisialisasi
      document.addEventListener('DOMContentLoaded', function() {
        initializeChart();
        updateSensorData(); // Update immediately
        setInterval(updateSensorData, 2000); // Update every 2 seconds
      });
    </script>
  </body>
  </html>
  )rawliteral";
  
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity) + ",";
  json += "\"vibration\":" + String(vibration) + ",";
  json += "\"light\":" + String(lightIntensity);
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleSensorData() {
  StaticJsonDocument<200> doc;
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["vibration"] = vibration;
  doc["light"] = lightIntensity;
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}
