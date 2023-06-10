/**
 * @file main.cpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <Arduino.h>

#include "network.h"
#include "leds.h"
#include "logger.h"
#include "sensor.h"

// CONFIG
#include "wifi_config.h"

// function declarations
void setup();
void loop();

void setup()
{
    Serial.begin(115200);
    initLeds();

    // connect to wifi network or blink error
    connectToLocalNetwork();

    initBsecSensor();
}

void loop()
{
    if (iaqSensor.run())
    {
        Serial.println(getBsecSensorInfo());
        // logSdCard(); // FIXME
        statusBlink(CRGB::SeaGreen, 1);
    }
    else
    {
        checkIaqSensorStatus();
    }

    delay(1000);
}

/**
 * Errors:
 * - No WiFi Available
 *
 */
