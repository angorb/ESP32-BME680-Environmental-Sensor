/**
 * @file leds.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILE_LEDS_H_
#define FILE_LEDS_H_

#include <FastLED.h>

#define LED_PIN 2 // Pin 2 on Thing Plus C is connected to WS2812 LED
#define COLOR_ORDER GRB
#define CHIPSET WS2812
#define NUM_LEDS 1
#define BRIGHTNESS 25

void initLeds(void);
void statusBlink(unsigned long color, uint16_t num);
void ledHappyDance(uint16_t num);

CRGB leds[NUM_LEDS];

void initLeds(void)
{
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void statusBlink(unsigned long color, uint16_t num)
{
    for (uint8_t i = 0; i < num; i++)
    {
        leds[0] = color;
        FastLED.show();
        delay(200);
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(200);
    }
}

void ledHappyDance(uint16_t num)
{
    // Happy Dance !
    statusBlink(CRGB::Red, num);
    statusBlink(CRGB::Orange, num);
    statusBlink(CRGB::Yellow, num);
    statusBlink(CRGB::Green, num);
    statusBlink(CRGB::Blue, num);
    statusBlink(CRGB::Indigo, num);
    statusBlink(CRGB::Violet, num);
}

// TODO replace this with statusBlink
void errLeds(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
}

#endif
