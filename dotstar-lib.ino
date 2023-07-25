#include "dotstar.h"

#include <SPI.h>


#define NUM_LEDS 8

CDotStar strip;

//----------------------------------------------------------------------------------------------------------
void setup()
{
  Serial.begin(115200);
  strip.init(NUM_LEDS);
  strip.set_brightness(10);

  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));

  // SPI_test();

}
//----------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------
void SPI_test()
{
  SPI.transfer('A');
  SPI.transfer('B');
  SPI.transfer('C');
}
//----------------------------------------------------------------------------------------------------------



void loop()
{
  // SPI_test();
  strip.clear();
  strip.set_led(0, 255, 0, 0);
  strip.set_led(1, 255, 0, 0);
  strip.show();
  delay(1000);

  strip.clear();
  strip.set_led(2, 0, 0, 255);
  strip.set_led(3, 0, 0, 255);
  strip.show();
  delay(1000);

  // strip.clear();
  // delay(1000);

  // strip.clear(255,255,0, true);
  // delay(1000);
}
