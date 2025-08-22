#include <Wire.h>         // I2C通信库
#include "MFRC522_I2C.h"  // MFRC522的I2C驱动库
MFRC522 mfrc522(0x28);    // 创建MFRC522对象，I2C地址设为0x28

void setup() {
  Serial.begin(115200);   // 初始化串口（调试用）        
  Wire.begin();           // 初始化I2C总线（默认SDA=GPIO21, SCL=GPIO22）
  mfrc522.PCD_Init();     // 初始化MFRC522读卡器
  ShowReaderDetails();    // 显示读卡器版本信息
  Serial.println(F("Scan PICC to see UID, type, and data blocks..."));
}

void loop() {
  // 检测是否有新卡片 && 能否成功读取卡片数据
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    delay(50);  // 降低轮询频率，减少CPU占用
    return;     // 无卡或读卡失败时退出
  }
  
  // 打印卡片UID（16进制格式）
  Serial.print(F("Card UID:"));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();
}

void ShowReaderDetails() {
  //  获得MFRC522软件
  byte v = mfrc522.PCD_ReadRegister(mfrc522.VersionReg);
  Serial.print(F("MFRC522 Software Version: 0x"));
  Serial.print(v, HEX);

  // 版本判断
  if (v == 0x91)
    Serial.print(F(" = v1.0"));
  else if (v == 0x92)
    Serial.print(F(" = v2.0"));
  else
    Serial.print(F(" (unknown)"));
  Serial.println("");

  // 通信故障检测，返回0x00或0xFF时，可能是通信信号传输失败
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println(F("WARNING: Communication failure, is the MFRC522 properly connected?"));
  }
}