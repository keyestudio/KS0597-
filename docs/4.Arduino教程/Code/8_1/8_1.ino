/*  
 * 名称    : 8_1
 * 功能    : PIR传感器检测人体移动
 * IDE版本 ：ARDUINO 2.3.4
 * 作者    : https://www.keyestudio.com/
 */
int val = 0;
int PIRPin = 25;   //PIR传感器的引脚定义为GPIO25
void setup() {
  Serial.begin(115200);   //波特率设置为115200
  pinMode(PIRPin, INPUT);    //将传感器设置为输入模式
}

void loop() {
  val = digitalRead(PIRPin);    //读取传感器值
  Serial.print(val);    //打印传感器值
  if (val == 1) {    //附近有人移动，输出高电平
    Serial.println("\t Some body is in this area!");
    delay(100);
  }
  else {    //如果附近没有人移动，输出低电平
    Serial.println("\t No one!");
    delay(100);
  }
}