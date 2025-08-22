/*  
 * 名称    : 8_2
 * 功能    : 控制蜂鸣器的发声频率，播放不同音调的声音
 * IDE版本 ：ARDUINO 2.3.4
 * 作者    : https://www.keyestudio.com/
 */
#define BUZZER_PIN 19  // 蜂鸣器连接至GPIO19（必须是支持PWM的引脚）

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // 测试1：发出1kHz蜂鸣声（持续1秒）
  tone(BUZZER_PIN, 1000);  // 频率1000Hz
  delay(1000);
  noTone(BUZZER_PIN);      // 关闭蜂鸣器
  delay(1000);

  // 测试2：发出2kHz蜂鸣声（持续0.5秒）
  tone(BUZZER_PIN, 2000);  // 频率2000Hz
  delay(500);
  noTone(BUZZER_PIN);
  delay(1000);

  // 测试3：播放简单旋律（Do-Re-Mi）
  int melody[] = {262, 294, 330};  // C4, D4, E4 频率
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, melody[i]);
    delay(300);
    noTone(BUZZER_PIN);
    delay(50);
  }
  delay(2000);  // 等待2秒后循环
}