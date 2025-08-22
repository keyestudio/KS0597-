#include <Wire.h>
#include "MFRC522_I2C.h"
#include <ESP32Servo.h>

MFRC522 rfid(0x28);  // RFID读卡器
Servo doorLock;      // 门锁舵机
const int servoPin = 32;

// 授权卡UID (替换为你自己的卡号)
byte allowedCard[] = {0x2E, 0x37, 0x42, 0x05};

void setup() {
  Serial.begin(115200);
  Wire.begin();
  rfid.PCD_Init();
  doorLock.attach(servoPin);
}

void loop() {
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) { // 检测到新卡？
    if (memcmp(rfid.uid.uidByte, allowedCard, 4) == 0) { // 检查是否授权卡
      doorLock.write(90);  // 开门（90°）
      delay(2000);         // 保持开门 2 秒
      doorLock.write(180); // 关门（180°）
    }
    rfid.PICC_HaltA();     // 停止读卡
  }
  delay(100); 
}