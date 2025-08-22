#include <WiFi.h>
#include <WebServer.h>

// 替换为你的网络凭据
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WebServer server(80);

// 光敏电阻连接的引脚
const int lightSensorPin = 34;

// 存储传感器值
int sensorValue = 0;

void setup() {
  Serial.begin(115200);
  
  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.println("正在连接WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi已连接");
  Serial.println("IP地址: ");
  Serial.println(WiFi.localIP());

  // 设置路由
  server.on("/", handleRoot);
  server.on("/readLight", handleLightRead);
  
  // 启动服务器
  server.begin();
  Serial.println("The HTTP server has been started.");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Photoresistor</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin: 0;
      padding: 20px;
      background-color: #f5f5f5;
    }
    .container {
      max-width: 600px;
      margin: 0 auto;
      background-color: white;
      padding: 20px;
      border-radius: 10px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    h1 {
      color: #333;
    }
    .sensor-value {
      font-size: 24px;
      margin: 20px 0;
      padding: 10px;
      background-color: #f0f0f0;
      border-radius: 5px;
    }
    button {
      background-color: #4CAF50;
      color: white;
      border: none;
      padding: 10px 20px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 10px 2px;
      cursor: pointer;
      border-radius: 5px;
    }
    button:hover {
      background-color: #45a049;
    }
  </style>
</head>
<body>
  <div class="container">
    <h1>ESP32 Photoresistor</h1>
    <div class="sensor-value" id="sensorValue">--</div>
    <button onclick="readSensor()">read the sensor</button>
    <button onclick="startAutoRead()">automatically refresh</button>
    <button onclick="stopAutoRead()">stop automatically refreshing</button>
  </div>
  
  <script>
    let autoReadInterval;
    
    function readSensor() {
      fetch('/readLight')
        .then(response => response.text())
        .then(data => {
          document.getElementById('sensorValue').innerText = data;
        });
    }
    
    function startAutoRead() {
      stopAutoRead(); // 先停止任何现有的自动刷新
      readSensor();   // 立即读取一次
      autoReadInterval = setInterval(readSensor, 2000); // 每2秒读取一次
    }
    
    function stopAutoRead() {
      if (autoReadInterval) {
        clearInterval(autoReadInterval);
        autoReadInterval = null;
      }
    }
  </script>
</body>
</html>
)=====";

  server.send(200, "text/html", html);
}

void handleLightRead() {
  sensorValue = analogRead(lightSensorPin);  // 读取光照值
  String valueStr = String(sensorValue);
  server.send(200, "text/plain", valueStr); // 返回纯文本数据
  Serial.println("photoresistor value: " + valueStr);
}