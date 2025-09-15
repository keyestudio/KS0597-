const int lightSensorPin = 34;  // Photoresistor pin
const int ledPin = 12;          // LED pin
const int threshold = 2000;     // Light intensity threshold

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);  // Set LED pin to output mode
}

void loop() {
  int sensorValue = analogRead(lightSensorPin);  // Read the light intensity value

  Serial.printf("light intensity value: %d \t voltage value: %.2f\n", sensorValue, voltage);

  // Control the LED according to the light intensity
  if (sensorValue < threshold) {
    digitalWrite(ledPin, HIGH);  // Turn on LED
  } else {
    digitalWrite(ledPin, LOW);   // Turn off LED
  }

  delay(500);
}