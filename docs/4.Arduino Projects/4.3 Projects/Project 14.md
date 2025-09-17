## 14. Web Page Remote Monitor Air Quality

In smart school era, environment perception and data interconnection are becoming important symbols of its modernization. In this project, with the theme of Remote Monitoring of Air Quality, we will guide you to deeply explore the innovative application of Internet of Things in school monitoring.

From now on, let’s protect the school environment with technology, build a smart learning environment with innovation, and jointly explore the infinite possibilities of Internet of Things in education!



#### Principle

1. Data collection

	ENS160 sensor → ESP32 (via I2C)

2. Data transmission

	ESP32 → Router → Mobile phone/Computer

3. Data display

	Browser request → Server response → Update web page



#### Code Flow

```mermaid
flowchart TD
    A[Initialization] --> B[Network connection]
    B --> C[Server starts]
    C --> D{Request processing}
    D --> E[Web page]
    D --> F[Sensor data]
    E --> G[Display in real time]
    F --> G
    G --> H[Application completed]
```



#### Test Code

```c++
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <DFRobot_ENS160.h>

// Replace with your WiFi name and passwords
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WebServer server(80);  // Create a Web server object on port 80
DFRobot_ENS160_I2C ens160(&Wire, 0x53); // Create an ENS160 sensor object

void setup() {
  Serial.begin(115200);

  Wire.begin(); // Initialize the I2C bus
  
  // Initialize the ENS160 sensor
  while(ens160.begin() != 0) {
    Serial.println("ENS160 sensor initialization failed, please check connection!");
    delay(1000);
  }
  Serial.println("ENS160 sensor initialized successfully!");
  
  // Set work mode
  ens160.setPWRMode(ENS160_STANDARD_MODE);
  // Set the measurement environment data (temperature 25°C and humidity 50%RH as default values)
  ens160.setTempAndHum(25.0, 50.0);

  // connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set server routing
  server.on("/", handleRoot);       // Root path
  server.on("/data", handleData);   // Data API path

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // Handle client requests
  
  // Update sensor data
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 1000) { // Updated once per second
    lastUpdate = millis();
  }
}

// Handle root path requests
void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ENS160 Air Quality Sensor</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; background-color: #f5f5f5; }
    .container { max-width: 600px; margin: 0 auto; }
    .sensor-box { 
      background-color: white; 
      border-radius: 10px; 
      padding: 20px; 
      margin: 15px 0; 
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    .value { font-size: 28px; font-weight: bold; color: #2c3e50; margin: 10px 0; }
    .unit { font-size: 16px; color: #7f8c8d; }
    .label { font-size: 18px; color: #34495e; margin-bottom: 5px; }
    .updated { font-size: 12px; color: #95a5a6; margin-top: 15px; }
    button { 
      background-color: #3498db; 
      color: white; 
      border: none; 
      padding: 12px 25px; 
      border-radius: 5px; 
      cursor: pointer; 
      font-size: 16px;
      margin-top: 15px;
      transition: background-color 0.3s;
    }
    button:hover { background-color: #2980b9; }
    .aqi-indicator {
      height: 20px;
      border-radius: 10px;
      margin: 10px 0;
      background: linear-gradient(to right, #00e400, #ffff00, #ff7e00, #ff0000, #8f3f97, #7e0023);
    }
    .status {
      padding: 8px;
      border-radius: 5px;
      color: white;
      font-weight: bold;
      margin-top: 5px;
      display: inline-block;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>ENS160 Air Quality Sensor</h1>
    
    <div class="sensor-box">
      <div class="label">Air Quality Index (AQI)</div>
      <div class="aqi-indicator"></div>
      <div><span id="aqi-value" class="value">--</span></div>
      <div id="aqi-status" class="status">--</div>
      <div class="unit">1-5 (1=Excellent, 5=Unhealthy)</div>
    </div>
    
    <div class="sensor-box">
      <div class="label">TVOC Concentration</div>
      <div><span id="tvoc-value" class="value">--</span> <span class="unit">ppb</span></div>
    </div>
    
    <div class="sensor-box">
      <div class="label">CO2 Equivalent</div>
      <div><span id="eco2-value" class="value">--</span> <span class="unit">ppm</span></div>
    </div>
    
    <div class="updated" id="last-updated">Last update: --</div>
    
    <button onclick="refreshData()">Refresh Data</button>
  </div>

  <script>
    function getAqiStatus(aqi) {
      if (aqi === 1) return {text: "Excellent", color: "#00e400"};
      if (aqi === 2) return {text: "Good", color: "#ffff00"};
      if (aqi === 3) return {text: "Moderate", color: "#ff7e00"};
      if (aqi === 4) return {text: "Poor", color: "#ff0000"};
      if (aqi === 5) return {text: "Unhealthy", color: "#8f3f97"};
      return {text: "Unknown", color: "#7f8c8d"};
    }
    
    function refreshData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          // Update AQI
          document.getElementById('aqi-value').textContent = data.aqi;
          const aqiStatus = getAqiStatus(data.aqi);
          const aqiElement = document.getElementById('aqi-status');
          aqiElement.textContent = aqiStatus.text;
          aqiElement.style.backgroundColor = aqiStatus.color;
          
          // Update TVOC
          document.getElementById('tvoc-value').textContent = data.tvoc;
          
          // Update eCO2
          document.getElementById('eco2-value').textContent = data.eco2;
          
          // Update timestamp
          const now = new Date();
          document.getElementById('last-updated').textContent = 
            `Last update: ${now.toLocaleTimeString()}`;
        })
        .catch(error => console.error('Error fetching data:', error));
    }
    
    // Obtain data when the page is loading
    window.onload = refreshData;
    
    // Refresh the data every 5 seconds
    setInterval(refreshData, 5000);
  </script>
</body>
</html>
)=====";

  server.send(200, "text/html", html);
}

// Handle data API requests
void handleData() {
  // Obtain the data from the ENS160 sensor
  uint8_t aqi = ens160.getAQI();
  uint16_t tvoc = ens160.getTVOC();
  uint16_t eco2 = ens160.getECO2();
  
  // Create a JSON response
  String json = "{";
  json += "\"aqi\":" + String(aqi) + ",";
  json += "\"tvoc\":" + String(tvoc) + ",";
  json += "\"eco2\":" + String(eco2);
  json += "}";
  
  server.send(200, "application/json", json);
}
```



#### Code Explanation

**Here covers extracurricular knowledge of HTML, CSS, and JS, so we only provide a brief introduction.**

**1.Hardware initialization**

```c++
Wire.begin(); // Initialize the I2C bus
  
// Initialize the ENS160 sensor
while(ens160.begin() != 0) {
    Serial.println("ENS160 sensor initialization failed, please check connection!");
    delay(1000);
}
Serial.println("ENS160 sensor initialized successfully!");
```

- Communicate with the sensor via the I2C protocol. If the sensor is not connected, the program will keep detecting.

<br>

**2. Network service**

**WiFi connectioin**

```c++
WiFi.begin(ssid, password);
  Serial.print("CWiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi is connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
```

- After connection, the serial port will print the local area network IP of the ESP32

**Server initialization**

```c++
WebServer server(80);  // Create an HTTP server on port 80

// Route registration
server.on("/", handleRoot);      // Root path → Return to HTML page
server.on("/data", handleData);  // Data path → Return JSON data

server.begin();  // Start the server
Serial.println("HTTP server started");
```

- `/` : Return the HTML of the visual web page
- `/data` : Return sensor data in JSON format

**Request processing loop**

```c++
void loop() {
  server.handleClient();  // Handle incoming client requests
}
```

<br>

**3. Data processing**

**Data API processing function**

```c++
void handleData() {
  // 1. Sensor data acquisition
  uint8_t aqi = ens160.getAQI();     // Air Quality index
  uint16_t tvoc = ens160.getTVOC();  // Volatile organic compounds
  uint16_t eco2 = ens160.getECO2();  // Equivalent carbon dioxide

  // 2. JSON data construction
  String json = "{";
  json += "\"aqi\":" + String(aqi) + ",";
  json += "\"tvoc\":" + String(tvoc) + ",";
  json += "\"eco2\":" + String(eco2);
  json += "}";

  // 3. HTTP response sending
  server.send(200, "application/json", json);  // Status code 200, in JSON format
}
```

- Data processing flow

  Sensor reading → Data formatting → JSON construction → HTTP response

<br>

**4. Dynamic update**

```javascript
function refreshData() {
  fetch('/data')  // Initiate an API request
    .then(response => response.json())  // Parse the JSON response
    .then(data => {
      // DOM update
      document.getElementById('aqi-value').textContent = data.aqi;
      document.getElementById('tvoc-value').textContent = data.tvoc;
      document.getElementById('eco2-value').textContent = data.eco2;
      
      // Status visualization update
      const aqiStatus = getAqiStatus(data.aqi);
      aqiElement.textContent = aqiStatus.text;
      aqiElement.style.backgroundColor = aqiStatus.color;
      
      // Timestamp update
      document.getElementById('last-updated').textContent = 
        `Last update: ${new Date().toLocaleTimeString()}`;
    })
}

// Timed automatic update
setInterval(refreshData, 5000);  // Updated every 5 seconds

// Update the page loading immediately
window.onload = refreshData;

// Manual update button
<button onclick="refreshData()">Refresh Data</button>
```



#### Test Result

1. After uploading the code, open the serial monitor and set the baud rate to 115200. You can see the printed IP information:

   ![1102](../../img/1102.png)

2. Enter this IP address in the browser of your mobile phone or computer to access the air quality monitoring page.

   - Automatic update: Data is obtained immediately when the page is opened, and the page automatically refreshes with data updated every 5 seconds.
   - Manual update: Click the refresh button to update immediately. New data will be displayed immediately after the operation.
   - Visual feedback: The status box dynamically changes color based on the AQI value, and the timestamp shows the last update time.
   
   <span style="color: rgb(200, 70, 100);">Note: Make sure your mobile phone/computer and ESP32 are connected to the same WiFi.</span>
   
   ![1401](../../img/1401.png)



#### FAQ

1. If nothing is printed on the serial monitor, please press the reset button on the board.

   ![RESET](../../img/RESET.png)

2. If the ESP32 has not been able to obtain an IP address, it is usually because the WiFi connection has failed. Solutions:

   - Make sure that the WiFi name and password in the code have been replaced with yours.
   - Make sure your WiFi network is 2.4GHz. ESP32 does not support 5GHz WiFi.

3. If there is no page when entering the IP address,

	- Make sure the IP address is entered correctly.
	- Check whether your mobile phone/computer is on the same network as the ESP32.