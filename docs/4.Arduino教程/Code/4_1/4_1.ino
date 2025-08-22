#include <Wire.h>
#include <AHT20.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DFRobot_ENS160.h>
#include <Adafruit_NeoPixel.h>

// 硬件配置
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_ADDRESS 0x3C
#define LED_PIN     4       // RGB灯控制引脚
#define LED_COUNT   4       // LED灯珠数量
#define I2C_ADDR    0x53    // ENS160默认I2C地址

// eCO2浓度等级阈值 (ppm)
#define ECO2_MODERATE   1000
#define ECO2_POOR       1500

// 创建对象
AHT20 aht20;
DFRobot_ENS160_I2C ENS160(&Wire, I2C_ADDR);
Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// 上次更新时间
unsigned long lastUpdate = 0;
const unsigned long updateInterval = 2000; // 2秒更新间隔

const int lightSensorPin = 34; // 光敏电阻传感器


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

  // 初始化ENS160
  while (NO_ERR != ENS160.begin()) {
    Serial.println("ENS160初始化失败,请检查接线!");
    delay(3000); // 仅通过串口报错
  }
  
  // 设置传感器模式
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
  ENS160.setTempAndHum(25.0, 50.0);  // 设置校准温湿度

  // 初始化RGB灯带
  leds.begin();
  leds.show();  // 初始清空灯带
  leds.setBrightness(100);  // 中等亮度
}

void loop() {
  if(millis() - lastUpdate >= updateInterval) {
    lastUpdate = millis();
    
    float temperature = aht20.getTemperature();
    float humidity = aht20.getHumidity();

    int illum = analogRead(lightSensorPin); // 光照值

    // 获取eCO2浓度
    uint16_t eco2 = ENS160.getECO2();
    Serial.print("eCO2 concentration: ");
    Serial.print(eco2);
    Serial.println(" ppm");
    
    // 显示更新
    display.clearDisplay();
    display.setTextSize(1);
    
    // 更新显示
    updateDisplay(temperature, humidity, illum, eco2);

    if(illum < 1500){
      setAllLEDs(0, 255, 0);
    }
    else{
      setAllLEDs(0, 0, 0);
    }

    if(temperature > 30){
      setAllLEDs(255, 0, 0);
    }
    else{
      setAllLEDs(0, 0, 0);
    }

    if(humidity > 60){
      setAllLEDs(0, 0, 255);
    }
    else{
      setAllLEDs(0, 0, 0);
    }

    if(eco2 > 1000){
      setAllLEDs(255, 255, 0);
    }
    else{
      setAllLEDs(0, 0, 0);
    }
  }
}

// 数值显示
void updateDisplay(float temp, float humi, float illum, float eco2) {
  // 温度显示
  display.setCursor(0, 12);
  display.print("T: ");
  display.print(temp, 1);
  display.cp437(true);
  display.write(248); // °符号
  display.println("C");
  
  // 湿度显示
  display.setCursor(0, 24);
  display.print("RH: ");
  display.print(humi, 1);
  display.println("%");

  // 空气质量显示
  display.setCursor(0, 36);
  display.print("eco2: ");
  display.print(eco2, 1);
  display.println(" ppm");

  // 光照值显示
  display.setCursor(0, 48);
  display.print("E: ");
  display.print(illum, 1);
  display.println(" Lux");
  
  display.display();
}

// 设置所有LED颜色
void setAllLEDs(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}