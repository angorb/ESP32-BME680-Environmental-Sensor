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
#include "webserver.h"

// CONFIG
#include "wifi_config.h"

#define SENSOR_DATA_TO_SERIAL false

// function declarations
void setup();
void loop();

void setup()
{
    Serial.begin(115200);

    initLeds();

    // connect to wifi network or blink error
    connectToLocalNetwork();

    setupWebServer();

    initBsecSensor();
}

void loop()
{
    if (iaqSensor.run())
    {
        sensorDataJson = updateSensorData();
        if (SENSOR_DATA_TO_SERIAL)
        {
            Serial.println(sensorDataJson);
            statusBlink(CRGB::SeaGreen, 1);
        }
        // logSdCard(); // FIXME
    }
    else
    {
        checkIaqSensorStatus();
    }
}

/**
 * Errors:
 * - No WiFi Available
 *
 */
