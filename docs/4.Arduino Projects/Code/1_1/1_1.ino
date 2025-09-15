int ledPin = 12;  // Define the variable ledPin, connect LED to pin 12

void setup() {
  pinMode(ledPin, OUTPUT);  // Set pin 12 to output mode
}

void loop() {
  digitalWrite(ledPin, HIGH);  // Turn on the LED
  delay(1000);                 // Delay 1s
  digitalWrite(ledPin, LOW);   // Turn off the LED
  delay(1000);                 // Delay 1s
}