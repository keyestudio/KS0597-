#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// Replace it with your network credentials
const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";

WebServer server(80);
Servo myServo;

// Servo control pins
const int servoPin = 32;
void handleRoot() {
  // Send the HTML page
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Servo Control</title>
    <style>
        body {
            font-family: Arial, sans-serif;
            text-align: center;
            margin: 0;
            padding: 20px;
            background-color: #f5f5f5;
        }
        .container {
            max-width: 400px;
            margin: 0 auto;
            background: white;
            padding: 20px;
            border-radius: 10px;
            box-shadow: 0 0 10px rgba(0,0,0,0.1);
        }
        h1 {
            color: #333;
        }
        .btn {
            display: inline-block;
            padding: 15px 30px;
            margin: 10px;
            font-size: 18px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            transition: background-color 0.3s;
        }
        .open-btn {
            background-color: #4CAF50;
            color: white;
        }
        .close-btn {
            background-color: #f44336;
            color: white;
        }
        .btn:hover {
            opacity: 0.9;
        }
        .status {
            margin-top: 20px;
            padding: 10px;
            border-radius: 5px;
            font-weight: bold;
        }
        .open {
            background-color: #d4edda;
            color: #155724;
        }
        .closed {
            background-color: #f8d7da;
            color: #721c24;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Door Control</h1>
        <button class="btn open-btn" onclick="controlServo(90)">Door open</button>
        <button class="btn close-btn" onclick="controlServo(180)">Door close</button>
        <div id="status" class="status">State: Unknown</div>
    </div>

    <script>
        function controlServo(angle) {
            // Update status display
            const statusElem = document.getElementById('status');
            statusElem.textContent = angle === 90 ? 'State: Door opening...' : 'State: Door closeing...';
            statusElem.className = 'status';
            
            // Send a request to ESP32
            fetch(`/control?angle=${angle}`)
                .then(response => response.text())
                .then(data => {
                    statusElem.textContent = `State: ${angle === 90 ? 'Door opened' : 'Door closed'}`;
                    statusElem.className = `status ${angle === 90 ? 'open' : 'closed'}`;
                })
                .catch(error => {
                    console.error('Error:', error);
                    statusElem.textContent = 'Operation failed. Please try again';
                    statusElem.className = 'status';
                });
        }
    </script>
</body>
</html>
)rawliteral";
  
  server.send(200, "text/html", html);
}

void handleControl() {
  if (server.hasArg("angle")) {
    int angle = server.arg("angle").toInt();
    
    // Control the servo to rotate to the specified angle
    myServo.write(angle);
    
    // Return response
    String message = angle == 90 ? "Door opened" : "Door closed";
    server.send(200, "text/plain", message);
    
    Serial.print("Servo rotates to: ");
    Serial.print(angle);
    Serial.println("°");
  } else {
    server.send(400, "text/plain", "Parameter error");
  }
}

void setup() {
  Serial.begin(115200);
  
  // Allow ESP32 to use servos
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Set servo
  myServo.setPeriodHertz(50);    // Standard 50Hz servo
  myServo.attach(servoPin, 500, 2400); // Connect to the servo pins and set the minimum and maximum pulse widths
  
  // Initialize the servo position to door-closed state(180°)
  myServo.write(180);
  
  // Set server routing
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  
  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}