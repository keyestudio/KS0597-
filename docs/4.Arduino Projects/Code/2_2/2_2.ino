#include <DFRobot_ENS160.h>

#define I2C_COMMUNICATION  // I2C communication

#ifdef  I2C_COMMUNICATION
  DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ 0x53);  // the default address is 0x53
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

  // Initialize the sensor
  while( NO_ERR != ENS160.begin() ){
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  ENS160.setPWRMode(ENS160_STANDARD_MODE);  // Set the power mode to standard gas sensing mode

  ENS160.setTempAndHum(/*temperature=*/25.0, /*humidity=*/50.0);  // Write the current ambient temperature and relative humidity into ENS160 for calibrating and compensating for the measured gas data.

}

void loop()
{
  /**
   *   Obtain the working status of the sensor
   */
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status : ");
  Serial.println(Status);

  /**
   *   Obtain the air quality index
   */
  uint8_t AQI = ENS160.getAQI();
  Serial.print("Air quality index : ");
  Serial.println(AQI);

  /**
   *   Obtain the TVOC concentration
   *   Return value: 0-65000, unit: ppb
   */
  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("Concentration of total volatile organic compounds : ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  /**
   * Calculate the CO2 equivalent concentration based on the detection data of VOCs and hydrogen
   * Return value: 400-65000, unit: ppm
   */
  uint16_t ECO2 = ENS160.getECO2();
  Serial.print("Carbon dioxide equivalent concentration : ");
  Serial.print(ECO2);
  Serial.println(" ppm");

  Serial.println();
  delay(1000);
}