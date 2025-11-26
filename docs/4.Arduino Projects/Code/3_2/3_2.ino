#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// OLED configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Share I2C reset
#define I2C_ADDRESS 0x3C  // Default address of SH1106

// Create a display object
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // Initialize the serial port
  Serial.begin(115200);
  
  // Initialize OLED
  if(!display.begin(I2C_ADDRESS, true)) {  // true is 128x64 resolution
    Serial.println("SH1106 initialization failed");
    while(1);  // Stuck and not continuing
  }
  
  // Clear the screen and set the text properties
  display.clearDisplay();
  display.setTextSize(1);      // text size
  display.setTextColor(SH110X_WHITE);  // Monochrome display
  display.setCursor(10, 25);   // Set the starting position (center)
  
  // show "Hello world!"
  display.println("Hello world!");
  
  // Update display
  display.display();
}

void loop() {
  // There is no need for looping content
  delay(1000);
}