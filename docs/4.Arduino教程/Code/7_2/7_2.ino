#include <ESP32Servo.h>

Servo myservo;  // 创建舵机对象
const int servoPin = 32; // GPIO32

void setup() {
  myservo.attach(servoPin); // 初始化舵机
}

void loop() {
  myservo.write(180);   // 180°
  delay(1000);
  myservo.write(135);   // 135°
  delay(1000);
  myservo.write(90);    // 90°
  delay(1000);
}