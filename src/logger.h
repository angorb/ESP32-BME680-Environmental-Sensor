/**
 * @file logger.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILE_LOGGER_H_
#define FILE_LOGGER_H_

#include "SD.h"

#include "leds.h"
#include "sensor.h"

/**
 *  GPIO 5: DATA 3/CS
 *  GPIO 19: DATA 0/POCI (or Peripheral's SDO)
 *  GPIO 18: CLK/SCK
 *  GPIO 23: CMD/PICO (or Peripheral's SDI)
 */
#define SD_CS 5

String logMessage;
char saveFilePath[19];
bool hasCardError;

void setupLogging(void);
void logSdCard(void);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);

void setupLogging()
{
    sprintf(saveFilePath, "/log_%u.txt", getTime());

    // Initialize SD card
    SD.begin(SD_CS);
    hasCardError = false;
    if (!SD.begin(SD_CS))
    {
        Serial.println("Card Mount Failed");
        hasCardError = false;
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        hasCardError = false;

        return;
    }
    Serial.println("Initializing SD card...");
    if (!SD.begin(SD_CS))
    {
        Serial.println("ERROR - SD card initialization failed!");
        hasCardError = false;

        return; // init failed
    }
    File file = SD.open(saveFilePath);
    if (!file)
    {
        Serial.println("File doens't exist");
        Serial.println("Creating file...");

        writeFile(
            SD,
            saveFilePath,
            "iaq,iaq_accuracy,static_iaq,co2_equivalent,breath_voc_equivalent,temperature_raw,temperature_comp,pressure,humidity_raw,humidity_comp,gas,stab_status,run_in_status,time,uptime\n");
    }
    else
    {
        Serial.println("File already exists");
    }
    file.close();
}

void logSdCard()
{
    logMessage =
        String(iaqSensor.iaq) + "," +
        String(iaqSensor.iaqAccuracy) + "," +
        String(iaqSensor.staticIaq) + "," +
        String(iaqSensor.co2Equivalent) + "," +
        String(iaqSensor.breathVocEquivalent) + "," +
        String(iaqSensor.rawTemperature) + "," +
        String(iaqSensor.temperature) + "," +
        String(iaqSensor.pressure) + "," +
        String(iaqSensor.rawHumidity) + "," +
        String(iaqSensor.humidity) + "," +
        String(iaqSensor.gasPercentage) + "," +
        String(iaqSensor.stabStatus) + "," +
        String(iaqSensor.runInStatus) + "," +
        String(getTime()) + "," +
        String(millis() / 1000);

    if (!hasCardError)
        appendFile(SD, saveFilePath, logMessage.c_str());
}

// Write to the SD card (DON'T MODIFY THIS FUNCTION)
void writeFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Writing file: %s\n", path);
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        Serial.println("File written");
    }
    else
    {
        Serial.println("Write failed");
    }
    file.close();
}
// Append data to the SD card (DON'T MODIFY THIS FUNCTION)
void appendFile(fs::FS &fs, const char *path, const char *message)
{
    Serial.printf("Appending to file: %s\n", path);
    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        Serial.println("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        Serial.println("Message appended");
    }
    else
    {
        Serial.println("Append failed");
    }
    file.close();
}

#endif
