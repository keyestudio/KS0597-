#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1  // Share I2C reset
#define I2C_ADDRESS 0x3C  // Default address of SH1106

// Create a display object
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int sensorPin = 18;
volatile int goalCount = 0;  // Use volatile to ensure interrupt safety
unsigned long lastDetectTime = 0;
const int debounceTime = 50; // eliminate jitter(ms)

void setup() {
  Serial.begin(115200);

  // Initialize OLED
  if(!display.begin(I2C_ADDRESS, true)) {  // true is 128x64 resolution
    Serial.println("SH1106 initialization failed");
    while(1);  // Stuck and not continuing
  }

  // Clear the screen and set the text properties
  display.clearDisplay();
  display.setTextSize(3);      // Text size
  display.setTextColor(SH110X_WHITE);
  
  pinMode(sensorPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin), detectCallback, FALLING);
}

void detectCallback() {
  if (millis() - lastDetectTime > debounceTime) {
    goalCount++;
    lastDetectTime = millis();
  }
}

void loop() {
  // display
  display.clearDisplay();
  display.setCursor(40, 20);  
  display.println(goalCount);
  display.display();
}