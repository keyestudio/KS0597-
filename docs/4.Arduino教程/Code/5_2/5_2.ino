int val = 0;             // 用于存储键值
int button = 26;         // 将按钮的引脚连接到IO26

void setup() {
  Serial.begin(115200);      
  pinMode(button, INPUT);    
}

void loop() {
  val = digitalRead(button);  // 读取按键的值
  Serial.print(val);          // 打印
  if (val == 0) {             // 按下按钮时读取到低电平，并打印出相关的信息
    Serial.println("\t Press the botton");
    delay(100);
  }

  else {                      // 打印按钮的释放信息
    Serial.println("\t Loosen the botton");
    delay(100);
  }
}