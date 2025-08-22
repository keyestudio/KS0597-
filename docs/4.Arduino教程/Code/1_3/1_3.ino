const int lightSensorPin = 34;  // 光敏电阻传感器引脚
const int ledPin = 12;          // LED 灯引脚
const int threshold = 2000;     // 光照强度阈值

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);  // 设置 LED 灯引脚为输出模式
}

void loop() {
  int sensorValue = analogRead(lightSensorPin);  // 读取光照强度值

  Serial.printf("light intensity value: %d \t voltage value: %.2f\n", sensorValue, voltage);

  // 根据光照强度控制 LED 灯
  if (sensorValue < threshold) {
    digitalWrite(ledPin, HIGH);  // 点亮 LED 灯
  } else {
    digitalWrite(ledPin, LOW);   // 熄灭 LED 灯
  }

  delay(500);
}