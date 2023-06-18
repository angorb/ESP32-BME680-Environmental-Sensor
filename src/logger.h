/**
 * @file logger.h
 * @author nick brogna (nick@wemakepretty.com)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILE_LOGGER_H_
#define FILE_LOGGER_H_

#include <SD.h>
#include <SPI.h>

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
String filesystemDataJson; // FIXME sloppy scope

char saveFilePath[8]; // filenames can be no longer than 8 chars
bool hasCardError;

File logFile;

void setupLogging(void);
void logSdCard(void);
String updateFilesystemData(void);
void listDir(fs::FS &fs, const char *dirname, uint8_t levels);
void createDir(fs::FS &fs, const char *path);
void removeDir(fs::FS &fs, const char *path);
void readFile(fs::FS &fs, const char *path);
void writeFile(fs::FS &fs, const char *path, const char *message);
void appendFile(fs::FS &fs, const char *path, const char *message);
void renameFile(fs::FS &fs, const char *path1, const char *path2);
void deleteFile(fs::FS &fs, const char *path);
void testFileIO(fs::FS &fs, const char *path);

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        Serial.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if (levels)
            {
                listDir(fs, file.path(), levels - 1);
            }
        }
        else
        {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char *path)
{
    Serial.printf("Creating Dir: %s\n", path);
    if (fs.mkdir(path))
    {
        Serial.println("Dir created");
    }
    else
    {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char *path)
{
    Serial.printf("Removing Dir: %s\n", path);
    if (fs.rmdir(path))
    {
        Serial.println("Dir removed");
    }
    else
    {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char *path)
{
    Serial.printf("Reading file: %s\n", path);

    File file = fs.open(path);
    if (!file)
    {
        Serial.println("Failed to open file for reading");
        return;
    }

    Serial.print("Read from file: ");
    while (file.available())
    {
        Serial.write(file.read());
    }
    file.close();
}

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

void renameFile(fs::FS &fs, const char *path1, const char *path2)
{
    Serial.printf("Renaming file %s to %s\n", path1, path2);
    if (fs.rename(path1, path2))
    {
        Serial.println("File renamed");
    }
    else
    {
        Serial.println("Rename failed");
    }
}

void deleteFile(fs::FS &fs, const char *path)
{
    Serial.printf("Deleting file: %s\n", path);
    if (fs.remove(path))
    {
        Serial.println("File deleted");
    }
    else
    {
        Serial.println("Delete failed");
    }
}

void testFileIO(fs::FS &fs, const char *path)
{
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file)
    {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len)
        {
            size_t toRead = len;
            if (toRead > 512)
            {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        Serial.printf("%u bytes read for %u ms\n", flen, end);
        file.close();
    }
    else
    {
        Serial.println("Failed to open file for reading");
    }

    file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        Serial.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for (i = 0; i < 2048; i++)
    {
        file.write(buf, 512);
    }
    end = millis() - start;
    Serial.printf("%u bytes written for %u ms\n", 2048 * 512, end);
    file.close();
}

void setupLogging()
{
    sprintf(saveFilePath, "/log.txt", getTime());

    // Initialize SD card
    hasCardError = false;
    if (!SD.begin(SD_CS))
    {
        Serial.println("Card Mount Failed");
        hasCardError = true;
        return;
    }
    uint8_t cardType = SD.cardType();
    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        hasCardError = true;

        return;
    }

    if (!hasCardError)
    {
        Serial.println("File doens't exist");
        Serial.println("Creating file...");

        writeFile(
            SD,
            saveFilePath,
            "iaq,iaq_accuracy,static_iaq,co2_equivalent,breath_voc_equivalent,temperature_raw,temperature_comp,pressure,humidity_raw,humidity_comp,gas,stab_status,run_in_status,time,uptime\n");

        Serial.println("Starting logging...");
    }
    else
    {
        Serial.println("Logging disabled.");
    }
}

void logSdCard()
{
    if (!hasCardError)
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
            String(millis() / 1000) +
            "\n";

        appendFile(
            SD,
            saveFilePath,
            logMessage.c_str());

        Serial.println('.');
    }
}

String updateFilesystemData(void)
{
    StaticJsonDocument<500> data; // TODO too big

    data["logging_enabled"] = (!hasCardError);

    if (!hasCardError)
    {
        data["filesystem"]["total_mb"] = SD.totalBytes() / (1024 * 1024);
        data["filesystem"]["used_mb"] = SD.usedBytes() / (1024 * 1024);
    }

    data["time"]["now"] = getTime();
    data["time"]["uptime"] = (millis() / 1000);

    String response;
    serializeJson(data, response);

    return response;
}

#endif
