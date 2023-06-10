/**
 * @file network.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILE_NETWORK_H_
#define FILE_NETWORK_H_

#include <WiFi.h>
#include "esp_wifi.h"
#include "wifi_config.h"
#include "leds.h"

unsigned long getTime(void);
void connectToLocalNetwork(void);

const char *wifi_network_ssid = NETWORK_SSID;
const char *wifi_network_password = NETWORK_PASSWORD;

const char *ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;
const int daylightOffset_sec = 3600;
unsigned long startupTime = 0;

void connectToLocalNetwork(void)
{
    // START WIFI
    WiFi.mode(WIFI_AP_STA);

    WiFi.begin(wifi_network_ssid, wifi_network_password);
    Serial.println("\n[*] Connecting to WiFi Network");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        statusBlink(CRGB::Red, 2);
        delay(250);
    }

    // NTP time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    startupTime = getTime();
}

unsigned long getTime(void)
{
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("ERROR: Failed to obtain time");
        statusBlink(CRGB::Red, 3);
        return (0);
    }
    time(&now);

    return now;
}

#endif
