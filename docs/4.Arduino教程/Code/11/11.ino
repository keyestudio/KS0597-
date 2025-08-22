#include <WiFi.h>
#include <WebServer.h>

// 设置WiFi名称和密码
const char* ssid = "YourWiFiSSID";     // 替换为你的WiFi名称
const char* password = "YourWiFiPassword"; // 替换为你的WiFi密码

// 定义LED引脚
const int ledPin = 12;  // GPIO12

WebServer server(80);  // 创建Web服务器对象，端口80

// HTML网页内容
const char* htmlContent = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED control</title>
  <style>
    body { 
      font-family: Arial, sans-serif; 
      text-align: center; 
      margin: 0 auto; 
      padding-top: 50px; 
    }
    .button {
      padding: 10px 20px;
      font-size: 16px;
      margin: 10px;
      cursor: pointer;
      background-color: #4CAF50;
      color: white;
      border: none;
      border-radius: 5px;
    }
    .button-off {
      background-color: #f44336;
    }
  </style>
</head>
<body>
  <h1>ESP32 LED control</h1>
  <p>LED status: <span id="ledState">%LEDSTATE%</span></p>
  <p>
    <button class="button" onclick="controlLED('ON')">LED ON</button>
    <button class="button button-off" onclick="controlLED('OFF')">LED  OFF</button>
  </p>
  <script>
    function controlLED(state) {
      var xhr = new XMLHttpRequest();
      xhr.open("GET", "/" + state, true);
      xhr.send();
      
      // 更新页面状态显示
      document.getElementById("ledState").innerHTML = (state === 'ON') ? 'ON' : 'OFF';
    }
  </script>
</body>
</html>
)rawliteral";

void handleRoot() {
  // 获取当前LED状态
  String ledState = (digitalRead(ledPin)) ? "开启" : "关闭";
  
  // 替换HTML中的占位符
  String html = htmlContent;
  html.replace("%LEDSTATE%", ledState);
  
  server.send(200, "text/html", html);
}

void handleLEDOn() {
  digitalWrite(ledPin, HIGH);
  server.send(200, "text/plain", "LED已开启");
}

void handleLEDOff() {
  digitalWrite(ledPin, LOW);
  server.send(200, "text/plain", "LED已关闭");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // 初始关闭LED

  // 连接WiFi
  WiFi.begin(ssid, password);
  Serial.println("正在连接WiFi...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi is connected.");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());

  // 设置服务器路由
  server.on("/", handleRoot);
  server.on("/ON", handleLEDOn);
  server.on("/OFF", handleLEDOff);
  server.onNotFound(handleNotFound);
  
  // 启动服务器
  server.begin();
  Serial.println("The HTTP server has been started.");
}

void loop() {
  server.handleClient();  // 处理客户端请求
}