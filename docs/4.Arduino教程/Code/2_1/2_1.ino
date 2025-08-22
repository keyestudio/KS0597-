#include <Adafruit_NeoPixel.h>

#define LED_PIN 4      // 控制信号引脚
#define LED_COUNT 4    // LED 灯珠数量

// 创建 NeoPixel 对象
Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  leds.begin();           // 初始化 LED 灯带
  leds.show();            // 关闭所有灯珠
  leds.setBrightness(50); // 设置亮度（0~255）
}

void loop() {
  // 设置第一个灯珠为红色
  leds.setPixelColor(0, 255, 0, 0);
  // 设置第二个灯珠为绿色
  leds.setPixelColor(1, 0, 255, 0);
  // 设置第三个灯珠为蓝色
  leds.setPixelColor(2, 0, 0, 255);
  // 设置第四个灯珠为白色
  leds.setPixelColor(3, 255, 255, 255);
  // 更新显示
  leds.show();
  delay(1000);

  // 关闭所有灯珠
  leds.clear();
  leds.show();
  delay(1000);
}