#include <Adafruit_NeoPixel.h>

#define LED_PIN 4      // control signal pin
#define LED_COUNT 4    // LED bead number

// Create NeoPixel object
Adafruit_NeoPixel leds(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  leds.begin();           // initialize the LED strip
  leds.show();            // turn off all led
  leds.setBrightness(50); // set brightness（0~255）
}

void loop() {
  // Set the first LED bead to red
  leds.setPixelColor(0, 255, 0, 0);
  // Set the second LED bead to green
  leds.setPixelColor(1, 0, 255, 0);
  // Set the third LED bead to blue
  leds.setPixelColor(2, 0, 0, 255);
  // Set the forth LED bead to white
  leds.setPixelColor(3, 255, 255, 255);
  // refresh
  leds.show();
  delay(1000);

  // turn off all led
  leds.clear();
  leds.show();
  delay(1000);
}