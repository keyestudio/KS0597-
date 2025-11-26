#include <ESP32Servo.h>

Servo myservo;  // Create a servo object
const int servoPin = 32; // GPIO32

void setup() {
  myservo.attach(servoPin); // Initialize the servo
}

void loop() {
  myservo.write(180);    // 180°
  delay(1000);
  myservo.write(135);   // 135°
  delay(1000);
  myservo.write(90);  // 90°
  delay(1000);
}