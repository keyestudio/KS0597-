/*  
 * 名称    : 8_3
 * 功能    : 检测人员闯入并触发报警
 * IDE版本 ：ARDUINO 2.3.4
 * 作者    : https://www.keyestudio.com/
*/
#define PIR_PIN 25     // 人体红外传感器信号引脚
#define BUZZER_PIN 19  // 蜂鸣器控制引脚

// 报警音效参数
const int alarmTone = 2000;  // 报警频率(Hz)
const int beepDuration = 200; // 单次鸣响时长(ms)
const int pauseDuration = 100; // 鸣响间隔(ms)

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    triggerAlarm();
    delay(5000); // 报警持续5秒
  }
}

// 触发报警
void triggerAlarm() {
  for (int i = 0; i < 10; i++) { // 循环10次产生急促警报声
    tone(BUZZER_PIN, alarmTone);
    delay(beepDuration);
    noTone(BUZZER_PIN);
    delay(pauseDuration);
  }
}