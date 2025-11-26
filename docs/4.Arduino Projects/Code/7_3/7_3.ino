#include <Wire.h>
#include "MFRC522_I2C.h"
#include <ESP32Servo.h>

MFRC522 rfid(0x28);  // RFID card reader
Servo doorLock;      // Door lock servo
const int servoPin = 32;

// Authorized card UID (Replace with your own card number)
byte allowedCard[] = {0x2E, 0x37, 0x42, 0x05};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rfid.PCD_Init();
  doorLock.attach(servoPin);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { // A new card has been detected?
    if (memcmp(rfid.uid.uidByte, allowedCard, 4) == 0) { // Check if it is authorized
      doorLock.write(90);  // Open the door (90°)
      delay(2000);         // Keep the door open for 2 seconds
      doorLock.write(180); // Close the door (180°)
    }
    rfid.PICC_HaltA();     // Stop reading the card
  }
  delay(100);              
}