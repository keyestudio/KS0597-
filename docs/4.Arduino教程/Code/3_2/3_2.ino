#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED配置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // 共用I2C复位
#define I2C_ADDRESS 0x3C  // SH1106默认地址

// 创建显示对象
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // 初始化串口
  Serial.begin(115200);
  
  // 初始化OLED
  if(!display.begin(I2C_ADDRESS, true)) {  // true表示128x64分辨率
    Serial.println("SH1106初始化失败");
    while(1);  // 卡住不再继续
  }
  
  // 清屏并设置文本属性
  display.clearDisplay();
  display.setTextSize(1);      // 文本大小
  display.setTextColor(SH110X_WHITE);  // 单色显示
  display.setCursor(10, 25);   // 设置起始位置(居中)
  
  // 显示"Hello world!"
  display.println("Hello world!");
  
  // 更新显示
  display.display();
}

void loop() {
  // 不需要循环内容
  delay(1000);
}