
class CLedManager
{
    struct led_t {unsigned char current, blue, green, red;};

public:

    // constructor
    CLedManager() {m_leds = nullptr;}

    void init(int led_count);
    void set_led(int index, int R, int G, int B, bool blink=false);
    void show_led(int index, int R, int G, int B, bool blink=false); // this is for ONE led. This will call set_led and show() automatically
    void set_current(int level);  // 0-31
    void set_blink_period(int period_ms);
    void show(); // this will send data to the LED strip

protected:
    int m_led_count;
    int m_max_brightness;
    led_t* m_leds;
};
