#include <Stepper.h>   // 步进电机控制库
#include <WiFi.h>      // ESP32 WiFi功能库
#include <WebServer.h> // 网页服务器库

// 电机参数（28BYJ-48）
const int STEPS_PER_REV = 2038;  // 实际步数/圈
const int MOTOR_PIN1 = 14;       // IN1
const int MOTOR_PIN2 = 27;       // IN2
const int MOTOR_PIN3 = 16;       // IN3
const int MOTOR_PIN4 = 17;       // IN4

// 默认参数
int motorSpeed = 10;      // 默认转速10转/分钟
int rotationCount = 2;    // 默认转动2圈

// WiFi凭证
const char* ssid = "YourWiFiSSID";         // 修改为你的WiFi名称
const char* password = "YourWiFiPassword"; // 修改为你的WiFi密码

// 初始化步进电机（注意引脚顺序IN1-IN3-IN2-IN4）
Stepper myStepper(STEPS_PER_REV, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

WebServer server(80);  // 创建Web服务器，端口80

void setup() {
  Serial.begin(115200);
  
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
  Serial.println(WiFi.localIP()); // 打印获取到的IP地址
  
  // 设置路由
  server.on("/", handleRoot);           // 根目录请求
  server.on("/control", handleControl); // 控制请求
  
  server.begin(); // 启动Web服务器
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); // 处理客户端请求
}

// 电机转动函数
void rotateMotor(int turns, bool reverse) {
  myStepper.setSpeed(motorSpeed); // 设置转速
  int steps = STEPS_PER_REV * turns * (reverse ? -1 : 1); // 计算总步数(方向由reverse决定)
  myStepper.step(steps); // 执行转动
}

// 处理根目录请求
void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 Stepper motor control</title>
  <style>
    body { font-family: Arial; text-align: center; margin: 0 auto; padding: 20px; }
    .control { margin: 20px; padding: 15px; background: #f0f0f0; border-radius: 10px; }
    input, button { padding: 10px; margin: 5px; }
    button { background: #4CAF50; color: white; border: none; cursor: pointer; }
    button:hover { background: #45a049; }
  </style>
</head>
<body>
  <h1>ESP32 Stepper motor control</h1>
  <div class="control">
    <h2>Motor control</h2>
    <button onclick="controlMotor('forward')">Forward rotation</button>
    <button onclick="controlMotor('reverse')">Reverse rotation</button>
  </div>
  <div class="control">
    <h2>Parameter setting</h2>
    <label for="speed">Rotation speed (RPM): </label>
    <input type="number" id="speed" value=")"=====" + String(motorSpeed) + R"=====(" min="1" max="15"><br>
    <label for="turns">Number of rotations: </label>
    <input type="number" id="turns" value=")"=====" + String(rotationCount) + R"=====(" min="1" max="10"><br>
    <button onclick="updateSettings()">Update settings</button>
  </div>
  <script>
    function controlMotor(direction) {
      fetch('/control?dir=' + direction)
        .then(response => response.text())
        .then(data => console.log(data));
    }
    function updateSettings() {
      let speed = document.getElementById('speed').value;
      let turns = document.getElementById('turns').value;
      fetch('/control?speed=' + speed + '&turns=' + turns)
        .then(response => response.text())
        .then(data => console.log(data));
    }
  </script>
</body>
</html>
)=====";
  
  server.send(200, "text/html", html);  // 发送完整HTML页面
}

// 处理控制请求
void handleControl() {
  // 处理方向控制
  if (server.hasArg("dir")) {          // 检查是否有方向参数
    String direction = server.arg("dir"); // 获取参数值
    if (direction == "forward") {
      rotateMotor(rotationCount, false); // 正转
      server.send(200, "text/plain", "电机正转 " + String(rotationCount) + " 圈");
    } else if (direction == "reverse") {
      rotateMotor(rotationCount, true); // 反转
      server.send(200, "text/plain", "电机反转 " + String(rotationCount) + " 圈");
    }
  }
  
  // 处理参数设置
  if (server.hasArg("speed") || server.hasArg("turns")) {
    if (server.hasArg("speed")) {
      motorSpeed = server.arg("speed").toInt();
      motorSpeed = constrain(motorSpeed, 1, 15); // 限制在1-15 RPM之间
    }
    if (server.hasArg("turns")) {
      rotationCount = server.arg("turns").toInt();
      rotationCount = constrain(rotationCount, 1, 10); // 限制在1-10圈之间
    }
    server.send(200, "text/plain", "参数已更新: 转速=" + String(motorSpeed) + " RPM, 圈数=" + String(rotationCount));
  }
}