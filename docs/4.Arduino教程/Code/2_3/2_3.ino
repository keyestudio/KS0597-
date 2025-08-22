#include <DFRobot_ENS160.h>
#include <Adafruit_NeoPixel.h>

// 硬件配置
#define LED_PIN     4       // RGB灯控制引脚
#define LED_COUNT   4       // LED灯珠数量
#define I2C_ADDR    0x53    // ENS160默认I2C地址

// eCO2浓度等级阈值 (ppm)
#define ECO2_EXCELLENT   400
#define ECO2_GOOD       800
#define ECO2_MODERATE   1000
#define ECO2_POOR       1500

// 创建对象
DFRobot_ENS160_I2C ENS160(&Wire, I2C_ADDR);
Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  
  // 初始化ENS160
  while (NO_ERR != ENS160.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  
  // 设置传感器模式
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
  ENS160.setTempAndHum(25.0, 50.0);  // 设置校准温湿度

  // 初始化RGB灯带
  leds.begin();
  leds.show();  // 初始清空灯带
  leds.setBrightness(100);  // 中等亮度
}

void loop() {
  // 获取eCO2浓度
  uint16_t eco2 = ENS160.getECO2();
  Serial.print("eCO2 concentration: ");
  Serial.print(eco2);
  Serial.println(" ppm");

  // 根据空气质量控制LED
  updateAirQualityLED(eco2);

  delay(2000);  // 每2秒更新一次
}

// 空气质量可视化函数
void updateAirQualityLED(uint16_t eco2) {
  if (eco2 <= ECO2_GOOD) {
    // 优/良（绿色常亮）
    setAllLEDs(0, 255, 0);
  } 
  else if (eco2 <= ECO2_MODERATE) {
    // 一般（蓝色常亮）
    setAllLEDs(0, 0, 255);
  }
  else if (eco2 <= ECO2_POOR) {
    // 差（黄色常亮）
    setAllLEDs(255, 255, 0);
  }
  else {
    // 严重污染（红色常亮）
    setAllLEDs(255, 0, 0);
  }
}

// 设置所有LED颜色
void setAllLEDs(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}