#include <Wire.h>
#include <AHT20.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED配置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_ADDRESS 0x3C

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
AHT20 aht20;

// 上次更新时间
unsigned long lastUpdate = 0; // 记录上次更新时间
const unsigned long updateInterval = 2000; // 2秒更新间隔

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  
  // 初始化AHT20传感器
  if(!aht20.begin()) {
    Serial.println("AHT20未检测到! 请检查接线");
    while(1); // 仅通过串口报错
  }

  // 初始化OLED
  if(!display.begin(I2C_ADDRESS, true)) {
    Serial.println("OLED初始化失败");
    while(1); // 仅通过串口报错
  }

  // 清屏显示初始信息
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 30);
  display.println("Initializing...");
  display.display();
  delay(1000);
}

void loop() {
  if(millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    
    float temperature = aht20.getTemperature();
    float humidity = aht20.getHumidity();
    
    // 精简的显示更新
    display.clearDisplay();
    display.setTextSize(1);

    // 绘制标题栏
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Hygrothermograph");
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SH110X_WHITE);
    
    // 温度显示（优化符号显示）
    display.setCursor(0, 25);
    display.print("T: ");
    display.print(temperature, 1);
    display.cp437(true);
    display.write(248); // °符号
    display.println("C");
    
    // 湿度显示
    display.setCursor(0, 45);
    display.print("RH: ");
    display.print(humidity, 1);
    display.println("%");
    
    display.display();
  }
}