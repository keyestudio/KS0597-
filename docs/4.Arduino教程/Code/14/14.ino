#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <DFRobot_ENS160.h>

// 替换为您的WiFi凭证
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WebServer server(80);  // 创建Web服务器对象，端口80
DFRobot_ENS160_I2C ens160(&Wire, 0x53); // 创建ENS160传感器对象

void setup() {
  Serial.begin(115200);

  Wire.begin(); // 初始化I2C总线
  
  // 初始化ENS160传感器
  while(ens160.begin() != 0) {
    Serial.println("ENS160 sensor initialization failed, please check connection!");
    delay(1000);
  }
  Serial.println("ENS160 sensor initialized successfully!");
  
  // 设置工作模式
  ens160.setPWRMode(ENS160_STANDARD_MODE);
  // 设置测量环境数据（温度25°C，湿度50%RH作为默认值）
  ens160.setTempAndHum(25.0, 50.0);

  // 连接WiFi
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

  // 设置服务器路由
  server.on("/", handleRoot);       // 根路径
  server.on("/data", handleData);   // 数据API路径

  // 启动服务器
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();  // 处理传入的客户端请求
  
  // 更新传感器数据
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate >= 1000) { // 每秒更新一次
    lastUpdate = millis();
  }
}

// 处理根路径请求
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
    
    // 手动更新按钮
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
      fetch('/data')  // 发起API请求
        .then(response => response.json())  // 解析JSON响应
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
  // 获取ENS160传感器数据
  uint8_t aqi = ens160.getAQI();     // 空气质量指数
  uint16_t tvoc = ens160.getTVOC();  // 挥发性有机物
  uint16_t eco2 = ens160.getECO2();  // 等效二氧化碳
  
  // JSON数据构建
  String json = "{";
  json += "\"aqi\":" + String(aqi) + ",";
  json += "\"tvoc\":" + String(tvoc) + ",";
  json += "\"eco2\":" + String(eco2);
  json += "}";

  // HTTP响应发送
  server.send(200, "application/json", json);  // 状态码200，JSON格式
}