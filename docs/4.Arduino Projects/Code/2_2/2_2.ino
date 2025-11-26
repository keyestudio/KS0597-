#include <DFRobot_ENS160.h>
#include <Wire.h>

// For I2C communication, the default address is 0x53
DFRobot_ENS160_I2C ENS160(&Wire, 0x53);

void setup(void)
{
  Serial.begin(115200);

  // Initialize the sensor
  while(NO_ERR != ENS160.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");

  // Set the power mode to standard gas sensing mode
  ENS160.setPWRMode(ENS160_STANDARD_MODE);

  // Set the ambient temperature and humidity for calibration (sample: 25°C, 50%RH)
  ENS160.setTempAndHum(25.0, 50.0);
}

void loop()
{
  // Obtain the working status of the sensor
  uint8_t Status = ENS160.getENS160Status();
  Serial.print("Sensor operating status: ");
  Serial.println(Status);

  // Obtain the air quality index(1-5)
  uint8_t AQI = ENS160.getAQI();
  Serial.print("Air quality index: ");
  Serial.println(AQI);

  // Obtain the TVOC concentration(0-65000 ppb)
  uint16_t TVOC = ENS160.getTVOC();
  Serial.print("TVOC concentration: ");
  Serial.print(TVOC);
  Serial.println(" ppb");

  // Obtain the equivalent CO2 concentration(400-65000 ppm)
  uint16_t ECO2 = ENS160.getECO2();
  Serial.print("eCO2 concentration: ");
  Serial.print(ECO2);
  Serial.println(" ppm");

  Serial.println();
  delay(1000);
}