#include "dotstar.h"

#define NUM_LEDS 8

CDotStar strip;

void setup()
{
  Serial.begin(115200);
  strip.init(NUM_LEDS);
  strip.set_brightness(10);
}

void loop()
{
  strip.set_led(0, 0, 255, 0);
  strip.set_led(1, 0, 0, 255);
  strip.show();
  delay(1000);

  strip.set_led(2, 255, 0, 0);
  strip.set_led(3, 255, 0, 0);
  strip.show();
  delay(1000);
}
