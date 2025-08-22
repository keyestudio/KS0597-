int ledPin = 12;  // 定义变量 ledPin，表示 LED 灯连接在 12 号引脚

void setup() {
  pinMode(ledPin, OUTPUT);  // 将 12 号引脚设置为输出模式
}

void loop() {
  digitalWrite(ledPin, HIGH);  // 点亮 LED 灯
  delay(1000);                 // 延时 1 秒
  digitalWrite(ledPin, LOW);   // 熄灭 LED 灯
  delay(1000);                 // 延时 1 秒
}