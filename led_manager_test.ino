#include "led_manager.h"

#define NUM_LEDS 144

CLedManager led_strip;

void setup()
{
  Serial.begin(115200);
  led_strip.init(NUM_LEDS);
  led_strip.set_current(10);
}

void loop()
{
  led_strip.set_led(0, 0, 255, 0);
  led_strip.set_led(1, 0, 0, 255);
  led_strip.show();
  delay(1000);

  led_strip.set_led(2, 255, 0, 0);
  led_strip.set_led(3, 255, 0, 0);
  led_strip.show();
  delay(1000);


}
