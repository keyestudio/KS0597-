#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // 共用I2C复位
#define I2C_ADDRESS 0x3C  // SH1106默认地址

// 创建显示对象
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensorPin = 18;
volatile int goalCount = 0;  // 使用volatile保证中断安全
unsigned long lastDetectTime = 0;
const int debounceTime = 50; // 消抖时间(ms)

void setup() {
  Serial.begin(115200);

  // 初始化OLED
  if(!display.begin(I2C_ADDRESS, true)) {  // true表示128x64分辨率
    Serial.println("SH1106初始化失败");
    while(1);  // 卡住不再继续
  }

  // 清屏并设置文本属性
  display.clearDisplay();
  display.setTextSize(3);      // 文本大小
  display.setTextColor(SH110X_WHITE);
  
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), detectCallback, FALLING);
}

void detectCallback() {
  if (millis() - lastDetectTime > debounceTime) {
    goalCount++;
    lastDetectTime = millis();
  }
}

void loop() {
  // 显示
  display.clearDisplay();
  display.setCursor(40, 20);  
  display.println(goalCount);
  display.display();
}