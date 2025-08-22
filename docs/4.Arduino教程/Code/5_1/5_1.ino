// 引脚定义
const int INA = 5;  
const int INB = 13; 

void setup() {
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  // 调整旗帜的等待时间
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(8000);
}

void loop() {
  // 正转1秒
  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
  delay(1000);

  // 停止2秒
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(2000);

  // 反转1秒
  digitalWrite(INA, LOW);
  digitalWrite(INB, HIGH);
  delay(1000);

  // 停止2秒
  digitalWrite(INA, LOW);
  digitalWrite(INB, LOW);
  delay(2000);
}