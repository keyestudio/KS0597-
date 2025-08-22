int val = 0;
void setup() {
  Serial.begin(115200);   
  pinMode(18, INPUT);    
}

void loop() {
  val = digitalRead(18);  //读取数字电平
  Serial.print(val);     //打印读取的电平信号
  if (val == 0) {  //障碍物检测
    Serial.println("\t There are obstacles");
    delay(100);
  }
  else {  //未发现障碍物
    Serial.println("\t All going well");
    delay(100);
  }
}