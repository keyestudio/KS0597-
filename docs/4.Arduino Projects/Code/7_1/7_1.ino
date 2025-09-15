#include <Wire.h>         // I2C communication library
#include "MFRC522_I2C.h"  // I2C driver library of MFRC522
MFRC522 mfrc522(0x28);    // Create an MFRC522 object and set I2C address to 0x28

void setup() {
  Serial.begin(115200);   // Initialize the serial port (for debugging)
  Wire.begin();           // Initialize the I2C bus (default SDA=GPIO21, SCL=GPIO22)
  mfrc522.PCD_Init();     // Initialize the MFRC522 card reader
  ShowReaderDetails();    // Display the version information of the card reader
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
}

void loop() {
  // Check if there are new cards and whether the card data can be successfully read
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);  // Reduce polling frequency and decrease CPU usage
    return;     // Exit when there is no card or the card reading fails
  }
  
  // Print the card UID (hexadecimal format)
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();
}

void ShowReaderDetails() {
  //  Obtain the MFRC522 software
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);

  // Version judgment
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");

  // When communication fault detection returns 0x00 or 0xFF, it may indicate that the communication signal transmission has failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}