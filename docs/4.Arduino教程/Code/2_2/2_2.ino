#include <DFRobot_ENS160.h>

#define I2C_COMMUNICATION  // I2C通信

#ifdef  I2C_COMMUNICATION
  DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);  // I2C地址默认为0x53
#else
  /**
   * Set up digital pin according to the on-board pin connected with SPI chip-select pin
   * csPin Available Pins. For example: ESP32&ESP8266(D3), m0(6)
   */
  uint8_t csPin = D3;
  DFRobot_ENS160_SPI ENS160(&SPI, csPin);
#endif


void setup(void)
{
  Serial.begin(115200);

  // 初始化传感器
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  ENS160.setPWRMode(ENS160_STANDARD_MODE);  // 设置电源模式为标准气体感应模式

  ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);  // 用户将当前环境温度和相对湿度写入ENS160，用于校准和补偿测量气体数据。

}

void loop()
{
  /**
   *   获取传感器工作状态
   */
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status : ");
  Serial.println(Status);

  /**
   *   获取空气质量指数
   */
  uint8_t AQI = ENS160.getAQI();
  Serial.print("Air quality index : ");
  Serial.println(AQI);

  /**
   *   获取TVOC浓度
   *   返回值范围：0-65000，单位：ppb
   */
  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  /**
   * 根据VOCs和氢气的检测数据计算CO2当量浓度
   * 返回值范围：400-65000，单位：ppm
   */
  uint16_t ECO2 = ENS160.getECO2();
  Serial.print("Carbon dioxide equivalent concentration : ");
  Serial.print(ECO2);
  Serial.println(" ppm");

  Serial.println();
  delay(1000);
}