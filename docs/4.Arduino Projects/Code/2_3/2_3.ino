#include <DFRobot_ENS160.h>
#include <Adafruit_NeoPixel.h>

// Hardware configuration
#define LED_PIN     4       // RGB control pin
#define LED_COUNT   4       // LED pixel number
#define I2C_ADDR    0x53    // ENS160 default I2C address

// eCO2 concentration level threshold (ppm)
#define ECO2_EXCELLENT   400
#define ECO2_GOOD       800
#define ECO2_MODERATE   1000
#define ECO2_POOR       1500

// Create an object
DFRobot_ENS160_I2C ENS160(&Wire, I2C_ADDR);
Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  
  // Initialize ENS160
  while (NO_ERR != ENS160.begin()) {
    Serial.println("Communication with device failed, please check connection");
    delay(3000);
  }
  Serial.println("Begin ok!");
  
  // Set the sensor mode
  ENS160.setPWRMode(ENS160_STANDARD_MODE);
  ENS160.setTempAndHum(25.0, 50.0);  // Set calibration temperature and humidity

  // Initialize the RGB strip
  leds.begin();
  leds.show();  // Initially clear the light strip
  leds.setBrightness(100);  // Medium brightness
}

void loop() {
  // Obtain the eCO2 concentration
  uint16_t eco2 = ENS160.getECO2();
  Serial.print("eCO2 concentration: ");
  Serial.print(eco2);
  Serial.println(" ppm");

  // Control leds based on air quality
  updateAirQualityLED(eco2);

  delay(2000);  // Updated every two seconds
}

// Air quality visualization function
void updateAirQualityLED(uint16_t eco2) {
  if (eco2 <= ECO2_GOOD) {
    // Great/Good (Green always on)
    setAllLEDs(0, 255, 0);
  } 
  else if (eco2 <= ECO2_MODERATE) {
    // Normal (blue always on)
    setAllLEDs(0, 0, 255);
  }
  else if (eco2 <= ECO2_POOR) {
    // bad（yellow always on)
    setAllLEDs(255, 255, 0);
  }
  else {
    // Severe（red always on)
    setAllLEDs(255, 0, 0);
  }
}

// Set all LED colors
void setAllLEDs(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < LED_COUNT; i++) {
    leds.setPixelColor(i, leds.Color(r, g, b));
  }
  leds.show();
}