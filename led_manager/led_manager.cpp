
#include <Arduino.h>
#include "led_manager.h"
#include <string.h>

// Define pins connected to DotStar LED strip
const int DATA_PIN = 11; // DI Data pin (MOSI) 
const int CLOCK_PIN = 13; // CI Clock pin (SCK)

//----------------------------------------------------------------------------------------------------------
static void init_hardware()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    digitalWrite(DATA_PIN, LOW);
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// bitbang() - This function sends data to the LED strip by bitbanging the signals
//----------------------------------------------------------------------------------------------------------
static void bitbang(uint8_t data)
{
  // Send each bit of data (MSB first)
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(DATA_PIN, (data >> i) & 1);

    digitalWrite(CLOCK_PIN, HIGH);
    delayMicroseconds(1); // A small delay to stabilize the clock
    digitalWrite(CLOCK_PIN, LOW);
    delayMicroseconds(1); // A small delay to stabilize the clock
  }
}
//-------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
void CLedManager::init(int led_count)
{
    Serial.println("init -----");
    delete[] m_leds; // deallocate the array
    m_leds = new led_t[led_count]; // allocate the array
    m_max_brightness = 30; // set max brightness (0-31)
    m_led_count = led_count;

    memset(m_leds, 0, sizeof(led_t)*led_count); // clear the entire LED array to 0

    init_hardware();
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
void CLedManager::set_led(int index, int R, int G, int B, bool blink)
{
    Serial.println("set -----");
    // The first 3 bits of the LED frame are always 111
    const uint8_t led_frame_start = 0b11100000;

    // Sanity check 
    if (index < 0 || index >= m_led_count) return;

    // Create a reference to the LED in question
    led_t& this_led  = m_leds[index];

    // Fill in the attributes of this LED
    this_led.red     = R;
    this_led.green   = G;
    this_led.blue    = B;
}
//----------------------------------------------------------------------------------------------------------

void CLedManager::set_current(int level)
{
    // Sanity check
    if (level < 1 && level > 31) return;

    for(int i=0; i<m_led_count; i++)
        m_leds[i].current = level;
}


//----------------------------------------------------------------------------------------------------------
void CLedManager::show()
{
    // for (int i=0; i<24; i++)
    //     bitbang(0);
    

    Serial.println("show -----");

    // Send start frame
    bitbang(0);
    bitbang(0);
    bitbang(0);
    bitbang(0);

    // Get a byte pointer to the start of the LED array
    uint8_t* ptr = (uint8_t*)m_leds;

    for (int i=0; i<sizeof(led_t)*m_led_count; i++)
    {
        bitbang(*ptr++);
    }
        
    // Send end frame
    // for (int i=0; i<72; i++)
    //     bitbang(0xFF);
    bitbang(0xFF);
    bitbang(0xFF);
    bitbang(0xFF);
    bitbang(0xFF);

}
//----------------------------------------------------------------------------------------------------------