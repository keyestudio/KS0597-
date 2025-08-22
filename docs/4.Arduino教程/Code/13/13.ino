#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <AHT20.h>

// 替换为您的WiFi凭证
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WebServer server(80);  // 创建Web服务器对象，端口80
AHT20 aht20;           // 创建AHT20传感器对象

void setup() {
  Serial.begin(115200);

  Wire.begin(); // 初始化I2C总线
  
  // 检查AHT20是否连接正常
  if (aht20.begin() == false) {
    Serial.println("AHT20 not detected. Please check wiring.");
    while (1);
  }
  Serial.println("AHT20 acknowledged");

  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.print("正在连接到WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi is connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // 设置服务器路由
  server.on("/", handleRoot);       // 根路径
  server.on("/data", handleData);   // 数据API路径

  // 启动服务器
  server.begin();
  Serial.println("The HTTP server has been started.");
}

void loop() {
  server.handleClient();  // 处理客户端请求
}

// 处理根路径请求
void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>AHT20 temperature and humidity measuring</title>
  <style>
    body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 20px; }
    .container { max-width: 600px; margin: 0 auto; }
    .data-box { 
      background-color: #f0f0f0; 
      border-radius: 10px; 
      padding: 20px; 
      margin: 20px 0; 
      box-shadow: 0 4px 8px rgba(0,0,0,0.1);
    }
    .value { font-size: 24px; font-weight: bold; color: #2c3e50; }
    .unit { font-size: 16px; color: #7f8c8d; }
    .updated { font-size: 12px; color: #95a5a6; margin-top: 10px; }
    button { 
      background-color: #3498db; 
      color: white; 
      border: none; 
      padding: 10px 20px; 
      border-radius: 5px; 
      cursor: pointer; 
      font-size: 16px;
    }
    button:hover { background-color: #2980b9; }
  </style>
</head>
<body>
  <div class="container">
    <h1>AHT20 temperature and humidity measuring</h1>
    
    <div class="data-box">
      <h2>Temperature</h2>
      <div><span id="temp-value" class="value">--</span> <span class="unit">&deg;C</span></div>
    </div>
    
    <div class="data-box">
      <h2>Humidity</h2>
      <div><span id="humi-value" class="value">--</span> <span class="unit">%</span></div>
    </div>
    
    <div class="updated" id="last-updated">last update: --</div>
    
    <button onclick="refreshData()">refresh data</button>
  </div>

  <script>
    function refreshData() {
      fetch('/data')
        .then(response => response.json())
        .then(data => {
          document.getElementById('temp-value').textContent = data.temperature.toFixed(1);
          document.getElementById('humi-value').textContent = data.humidity.toFixed(1);
          const now = new Date();
          document.getElementById('last-updated').textContent = 
            `last update: ${now.toLocaleTimeString()}`;
        })
        .catch(error => console.error('Obtain data failed:', error));
    }
    
    // 页面加载时获取数据
    window.onload = refreshData;
    
    // 每5秒自动刷新数据
    setInterval(refreshData, 5000);
  </script>
</body>
</html>
)=====";

  server.send(200, "text/html", html);
}

// 处理数据API请求
void handleData() {
  // 获取温湿度数据
  float temperature = aht20.getTemperature();
  float humidity = aht20.getHumidity();
  
  // 创建JSON响应
  String json = "{";
  json += "\"temperature\":" + String(temperature) + ",";
  json += "\"humidity\":" + String(humidity);
  json += "}";
  
  server.send(200, "application/json", json);
}