//==========================================================================================================
// dotstar.h - Defines the interface to control DotStar LED strips
// Author: Akram S. Ali
// Updated: 7/21/2023
//==========================================================================================================
#pragma once

class CDotStar
{
    // This struct defines the parameters in an LED
    struct led_t {unsigned char brightness, blue, green, red;};

public:

    // constructor
    CDotStar() {m_leds = nullptr;}

    // Initialize the LED strip hardware
    void init(int led_count);

    // Set the R, G, B values of a given LED and the blink setting
    void set_led(int index, int R, int G, int B, bool blink=false);
    
    // This function will set the R, G, B values of an LED and send data to the LED strip
    void show_led(int index, int R, int G, int B, bool blink=false);
    
    // This function will send data to the LED strip
    void show();

    // This function will set the brightness level of all LEDs
    void set_brightness(int level);

    // This will set the blink period of an LED
    void set_blink_period(int period_ms);

    // This will clear the LED strip or set a single color to all LEDs
    void clear(int R=0, int G=0, int B=0, bool do_show=false);

protected:
    // Total number of LEDs in the strip
    int m_led_count;

    // Maximum brightness of all LEDs in the strip
    int m_max_brightness;

    // This holds the data for the LED strip
    led_t* m_leds;
};
//----------------------------------------------------------------------------------------------------------