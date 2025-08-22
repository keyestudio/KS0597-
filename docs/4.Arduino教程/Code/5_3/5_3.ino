const int INA = 5;
const int INB = 13;
const int buttonPin = 26; // 按键引脚

int currentDirection = 1; // 1=正转 2=反转
bool motorRunning = false;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 50;

void setup() {
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int buttonReading = digitalRead(buttonPin);
  
  // 检测按键按下（下降沿）
  if (buttonReading == LOW && millis() - lastDebounceTime > debounceDelay) {
    lastDebounceTime = millis();
    
    if (!motorRunning) {
      if (currentDirection == 1) {
        digitalWrite(INA, HIGH); // 正转
        digitalWrite(INB, LOW);
      } else {
        digitalWrite(INA, LOW); // 反转
        digitalWrite(INB, HIGH);
      }
      
      motorRunning = true;
      delay(1000); // 运行1秒
      
      digitalWrite(INA, LOW);
      digitalWrite(INB, LOW);
      motorRunning = false;
      
      // 切换下次方向
      currentDirection = (currentDirection == 1) ? 2 : 1; 
    }
  }
}