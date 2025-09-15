/*  
 * Name: 8_3
 * Function: The inspectors broke in and triggered the alarm
 * IDE：ARDUINO 2.3.4
 * Auther: https://www.keyestudio.com/
*/
#define PIR_PIN 25     // Signal pin of PIR motion sensor
#define BUZZER_PIN 19  // Buzzer control pin

// Alarm sound parameters
const int alarmTone = 2000;  // Alarm frequency (Hz)
const int beepDuration = 200; // Single sound duration (ms)
const int pauseDuration = 100; // Sound interval (ms)

void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  if (digitalRead(PIR_PIN) == HIGH) {
    triggerAlarm();
    delay(5000); // The alarm lasts for 5 seconds
  }
}

// trigger alarm
void triggerAlarm() {
  for (int i = 0; i < 10; i++) { // Reapeat 10 times of rapid alarm sound
    tone(BUZZER_PIN, alarmTone);
    delay(beepDuration);
    noTone(BUZZER_PIN);
    delay(pauseDuration);
  }
}