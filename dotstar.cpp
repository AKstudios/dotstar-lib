//==========================================================================================================
// dotstar.cpp - Implements the interface to control DotStar LED strips
// Author: Akram S. Ali
// Updated: 7/25/2023
//==========================================================================================================

#include <Arduino.h>
#include "dotstar.h"
#include <string.h>
#include <SPI.h>

// Define pins connected to DotStar LED strip
const int DATA_PIN = 11; // DI Data pin (MOSI) 
const int CLOCK_PIN = 13; // CI Clock pin (SCK)

//----------------------------------------------------------------------------------------------------------
// init_hardware() - Set the pinmodes for the data and clock pins
//----------------------------------------------------------------------------------------------------------
static void init_hardware()
{
    pinMode(DATA_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    digitalWrite(DATA_PIN, LOW);
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// send_data() - This function sends data to the LED strip by SPI or bitbanging the signals
//----------------------------------------------------------------------------------------------------------
void CDotStar::send_data(uint8_t data)
{
    // Transfer data via SPI
    if (!m_bitbang_setting) SPI.transfer(data); return;

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
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// init() - Initialize the LED strip hardware with a particular number of LEDs taken as argument
//----------------------------------------------------------------------------------------------------------
void CDotStar::init(int led_count)
{
    // Deallocate the array to begin with in case init is called again
    delete[] m_leds;

    // Allocate the array with the specified number of LEDs
    m_leds = new led_t[led_count];

    // Set the max brightness (5-bit, 0-31)
    m_max_brightness = 30;

    // Save the number of LEDs
    m_led_count = led_count;

    // Clear the entire LED array to 0 to begin with
    memset(m_leds, 0, sizeof(led_t)*led_count);

    // Set the hardware GPIO pins of the LED strip
    if (m_bitbang_setting) init_hardware();
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// set_led() - Set the R, G, B values of a given LED and the blink setting
//----------------------------------------------------------------------------------------------------------
void CDotStar::set_led(int index, int R, int G, int B, bool blink)
{
    // Sanity check - make sure LED index falls within the acceptable range
    if (index < 0 || index >= m_led_count) return;

    // Create a reference to the LED in question
    led_t& this_led  = m_leds[index];

    // Fill in the attributes of this LED
    this_led.red     = R;
    this_led.green   = G;
    this_led.blue    = B;
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// show_led() - This function will set the R, G, B values of an LED and send data to the LED strip
//----------------------------------------------------------------------------------------------------------
void CDotStar::show_led(int index, int R, int G, int B, bool blink)
{
    set_led(index, R, G, B, blink);
    show();
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// set_brightness() - This function will set the brightness level of all LEDs
//----------------------------------------------------------------------------------------------------------
void CDotStar::set_brightness(int level)
{
    // The first 3 bits of the LED frame are always 111
    const uint8_t led_frame_start = 0b11100000;

    // Sanity check - make sure the level falls within the acceptable range
    if (level < 1 && level > 31) return;

    // Set the brightness of each LED one by one in the array
    for(int i=0; i<m_led_count; i++)
        m_leds[i].brightness = led_frame_start | level;
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// show() - This function will send data to the LED strip
//----------------------------------------------------------------------------------------------------------
void CDotStar::show()
{
    // Send start frame - 32 bits of zeros
    send_data(0x00);
    send_data(0x00);
    send_data(0x00);
    send_data(0x00);

    // Get a byte pointer to the start of the LED array
    uint8_t* ptr = (uint8_t*)m_leds;

    // Send data to the LED strip one by one by bitbanging the signals
    for (int i=0; i<sizeof(led_t)*m_led_count; i++)
        send_data(*ptr++);

    // Send end frame - at least 32 bits of 1s based on the number of LEDs in the strip
    for (int i = 0; i < ((m_led_count + 15) / 16); i++)
        send_data(0xFF);
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// set_blink_period() - This will set the blink period of an LED
//----------------------------------------------------------------------------------------------------------
void CDotStar::set_blink_period(int period_ms)
{
    // WIP
}
//----------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------
// clear() - This will clear the LED strip or set a single color to all LEDs
//----------------------------------------------------------------------------------------------------------
void CDotStar::clear(int R, int G, int B, bool do_show)
{
    for (int i=0; i<m_led_count; i++)
    {
        // Fill in the attributes of this LED
        m_leds[i].red   = R;
        m_leds[i].green = G;
        m_leds[i].blue  = B;
    }

    if (do_show) show();
}
//----------------------------------------------------------------------------------------------------------
