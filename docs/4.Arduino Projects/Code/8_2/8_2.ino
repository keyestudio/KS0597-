/*  
 * Name: 8_2
 * Function: Control the sound frequency of the buzzer to play sounds of different tones
 * IDE：ARDUINO 2.3.4
 * Auther: https://www.keyestudio.com/
 */
#define BUZZER_PIN 19  // The buzzer is connected to GPIO19 (it must be a pin that supports PWM).

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  // Test 1: Emit a 1kHz beeping sound (lasting for 1 second)
  tone(BUZZER_PIN, 1000);  // Frequency of 1000Hz
  delay(1000);
  noTone(BUZZER_PIN);      // Turn off the buzzer
  delay(1000);

  // Test 2: Emit a 2kHz beeping sound (lasting for 0.5 seconds)
  tone(BUZZER_PIN, 2000);  // Frequency of 2000Hz
  delay(500);
  noTone(BUZZER_PIN);
  delay(1000);

  // Test 3: Play a simple melody (Do-Re-Mi)
  int melody[] = {262, 294, 330};  // C4, D4, E4 frequency
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, melody[i]);
    delay(300);
    noTone(BUZZER_PIN);
    delay(50);
  }
  delay(2000);  // Wait for 2 seconds and then loop
}