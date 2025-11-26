#include <Stepper.h>
#include <WiFi.h>
#include <WebServer.h>

// motor parameters（28BYJ-48）
const int STEPS_PER_REV = 2038;  // actual step/turn
const int MOTOR_PIN1 = 14;       // IN1
const int MOTOR_PIN2 = 27;       // IN2
const int MOTOR_PIN3 = 16;       // IN3
const int MOTOR_PIN4 = 17;       // IN4

// fixed parameters
const int motorSpeed = 10;      // fixed rotational speed 10 RPM
const int rotationCount = 2;    // fixedly rotate for 2 turns

// WiFi credential
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

// Initialize the stepper motor (note the pin sequence: IN1-IN3-IN2-IN4)
Stepper myStepper(STEPS_PER_REV, MOTOR_PIN1, MOTOR_PIN3, MOTOR_PIN2, MOTOR_PIN4);

WebServer server(80);  // create Web server at port 80

void setup() {
  Serial.begin(115200);
  
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
  
  // set the router
  server.on("/", handleRoot);
  server.on("/forward", []() {
    rotateMotor(rotationCount, false);
    server.send(200, "text/plain", "");
  });
  server.on("/reverse", []() {
    rotateMotor(rotationCount, true);
    server.send(200, "text/plain", "");
  });
  
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}

// motor rotation function
void rotateMotor(int turns, bool reverse) {
  myStepper.setSpeed(motorSpeed);
  int steps = STEPS_PER_REV * turns * (reverse ? -1 : 1);
  myStepper.step(steps);
}

// web page interface
void handleRoot() {
  String html = R"=====(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Stepper Motor Control</title>
  <style>
    body { 
      font-family: Arial; 
      text-align: center; 
      margin: 0 auto; 
      padding: 20px; 
      max-width: 400px;
    }
    .control-panel {
      margin: 20px auto;
      padding: 20px;
      background: #f5f5f5;
      border-radius: 10px;
      box-shadow: 0 2px 5px rgba(0,0,0,0.1);
    }
    .btn {
      display: inline-block;
      padding: 12px 24px;
      margin: 10px;
      background: #3498db;
      color: white;
      text-decoration: none;
      border-radius: 5px;
      border: none;
      font-size: 16px;
      cursor: pointer;
      transition: background 0.3s;
    }
    .btn:hover {
      background: #2980b9;
    }
    .btn-reverse {
      background: #e74c3c;
    }
    .btn-reverse:hover {
      background: #c0392b;
    }
  </style>
</head>
<body>
  <div class="control-panel">
    <h2>Stepper Motor Control</h2>
    <p>Fixed setting: Speed 10 RPM, 2 turns each time</p>
    <button class="btn" onclick="controlMotor('forward')">Forward</button>
    <button class="btn btn-reverse" onclick="controlMotor('reverse')">Reversal</button>
  </div>

  <script>
    function controlMotor(direction) {
      fetch('/' + direction)
        .catch(err => console.log('Request failed', err));
    }
  </script>
</body>
</html>
)=====";
  
  server.send(200, "text/html", html);
}