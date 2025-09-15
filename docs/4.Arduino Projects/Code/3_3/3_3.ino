#include <Wire.h>
#include <AHT20.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define I2C_ADDRESS 0x3C

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
AHT20 aht20;

// Last update time
unsigned long lastUpdate = 0; // Record last update time
const unsigned long updateInterval = 1000; // Update interval of 1 second

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  
  // Initialize the AHT20 sensor
  if(!aht20.begin()) {
    Serial.println("AHT20 not detected! Please check the wiring");
    while(1); // An error is reported only through the serial port
  }

  // Initialize OLED
  if(!display.begin(I2C_ADDRESS, true)) {
    Serial.println("OLED initialization failed");
    while(1); // An error is reported only through the serial port
  }

  // Clear the screen to display the initial information
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
    
    // Streamlined display updates
    display.clearDisplay();
    display.setTextSize(1);

    // Draw the title bar
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Hygrothermograph");
    display.drawLine(0, 10, SCREEN_WIDTH, 10, SH110X_WHITE);
    
    // Temperature display (optimized symbol display)
    display.setCursor(0, 25);
    display.print("T: ");
    display.print(temperature, 1);
    display.cp437(true);
    display.write(248); // °
    display.println("C");
    
    // Humidity display
    display.setCursor(0, 45);
    display.print("RH: ");
    display.print(humidity, 1);
    display.println("%");
    
    display.display();
  }
}