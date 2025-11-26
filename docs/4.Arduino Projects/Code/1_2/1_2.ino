// Define the analog pin of the photoresistor
const int lightSensorPin = 34;

void setup() {
  // Set baud rate to 115200
  Serial.begin(115200);
}

void loop() {
  // Read the analog value of the photoresistor（0~4095）
  int sensorValue = analogRead(lightSensorPin);

  // Convert the analog value to voltage value（0~3.3V）
  float voltage = sensorValue * (3.3 / 4095.0);

  // Print the read value on the serial monitor
  Serial.print("light intensity value: ");
  Serial.print(sensorValue);
  Serial.print("\t voltage value: ");
  Serial.println(voltage);

  // Delay 500 ms to avoid too fast refreshing
  delay(500);
}