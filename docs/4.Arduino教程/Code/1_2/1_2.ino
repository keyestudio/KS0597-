// 定义光敏电阻传感器连接的模拟引脚
const int lightSensorPin = 34;

void setup() {
  // 初始化串口通信，设置波特率为 115200
  Serial.begin(115200);
}

void loop() {
  // 读取光敏电阻传感器的模拟值（0~4095）
  int sensorValue = analogRead(lightSensorPin);

  // 将模拟值转换为电压值（0~3.3V）
  float voltage = sensorValue * (3.3 / 4095.0);

  // 将读取的值打印到串口监视器
  Serial.print("light intensity value: ");
  Serial.print(sensorValue);
  Serial.print("\t voltage value: ");
  Serial.println(voltage);

  // 延时 500 毫秒，避免数据刷新过快
  delay(500);
}