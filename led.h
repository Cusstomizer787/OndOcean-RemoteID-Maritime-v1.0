#pragma once

#include <stdint.h>
#include "board_config.h"

#ifdef WS2812_LED_PIN
#include <Adafruit_NeoPixel.h>
#endif

class Led {
public:
    enum class LedState {
        INIT=0,
        PFST_FAIL,
        ARM_FAIL,
        ARM_OK
    };

    void set_state(LedState _state) {
        state = _state;
    }
    void update(void);

private:
    void init(void);
    bool done_init;
    uint32_t last_led_trig_ms;
    LedState state;

#ifdef WS2812_LED_PIN
    uint32_t last_led_strip_ms;
    Adafruit_NeoPixel ledStrip{2, WS2812_LED_PIN, NEO_GRB + NEO_KHZ800}; //the BlueMark db210pro boards has two LEDs, therefore we need to use 2.
#endif
};

extern Led led;

// LED color definitions for compatibility
#define LED_COLOR_RED     0xFF0000
#define LED_COLOR_GREEN   0x00FF00
#define LED_COLOR_BLUE    0x0000FF
#define LED_COLOR_YELLOW  0xFFFF00
#define LED_COLOR_ORANGE  0xFF8000
#define LED_COLOR_OFF     0x000000

// Compatibility functions
void led_set_color(uint32_t color);
void led_init();
void led_update();
