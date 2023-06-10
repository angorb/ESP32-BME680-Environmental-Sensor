/**
 * @file sensor.h
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILE_SENSOR_H_
#define FILE_SENSOR_H_

#include "bsec.h"
#include "ArduinoJson.h"
#include "network.h"
#include "leds.h"

#ifndef BSEC_SENSOR_I2C
#define BSEC_SENSOR_I2C 0x77
#endif

Bsec iaqSensor;

void initBsecSensor(void);
void checkIaqSensorStatus(void);
void errLeds(void);
String getBsecSensorInfo(void);

void initBsecSensor(void)
{
    pinMode(LED_BUILTIN, OUTPUT);
    iaqSensor.begin(BSEC_SENSOR_I2C, Wire);
    checkIaqSensorStatus();

    bsec_virtual_sensor_t sensorList[13] = {
        BSEC_OUTPUT_IAQ,
        BSEC_OUTPUT_STATIC_IAQ,
        BSEC_OUTPUT_CO2_EQUIVALENT,
        BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
        BSEC_OUTPUT_RAW_TEMPERATURE,
        BSEC_OUTPUT_RAW_PRESSURE,
        BSEC_OUTPUT_RAW_HUMIDITY,
        BSEC_OUTPUT_RAW_GAS,
        BSEC_OUTPUT_STABILIZATION_STATUS,
        BSEC_OUTPUT_RUN_IN_STATUS,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
        BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
        BSEC_OUTPUT_GAS_PERCENTAGE};

    iaqSensor.updateSubscription(sensorList, 13, BSEC_SAMPLE_RATE_LP);
    checkIaqSensorStatus();
}

void checkIaqSensorStatus(void)
{
    if (iaqSensor.bsecStatus != BSEC_OK)
    {
        if (iaqSensor.bsecStatus < BSEC_OK)
        {
            Serial.println("BSEC error code : " + String(iaqSensor.bsecStatus));
            for (;;)
                errLeds(); /* Halt in case of failure */
        }
        else
        {
            Serial.println("BSEC warning code : " + String(iaqSensor.bsecStatus));
        }
    }

    if (iaqSensor.bme68xStatus != BME68X_OK)
    {
        if (iaqSensor.bme68xStatus < BME68X_OK)
        {
            Serial.println("BME68X error code : " + String(iaqSensor.bme68xStatus));
            for (;;)
                errLeds(); /* Halt in case of failure */
        }
        else
        {
            Serial.println("BME68X warning code : " + String(iaqSensor.bme68xStatus));
        }
    }
}

String getBsecSensorInfo(void)
{
    StaticJsonDocument<500> data;

    data["iaq"] = iaqSensor.iaq;
    data["iaq_accuracy"] = iaqSensor.iaqAccuracy;
    data["static_iaq"] = iaqSensor.staticIaq;
    data["co2_equivalent"] = iaqSensor.co2Equivalent;
    data["breath_voc_equivalent"] = iaqSensor.breathVocEquivalent;
    data["temperature"]["raw"] = iaqSensor.rawTemperature;
    data["temperature"]["comp"] = iaqSensor.temperature;
    data["pressure"] = iaqSensor.pressure;
    data["humidity"]["raw"] = iaqSensor.rawHumidity;
    data["humidity"]["comp"] = iaqSensor.humidity;
    data["gas"] = iaqSensor.gasPercentage;
    data["status"]["stab"] = iaqSensor.stabStatus;
    data["status"]["run_in"] = iaqSensor.runInStatus;
    data["time"]["now"] = getTime();
    data["time"]["uptime"] = (millis() / 1000);

    String response;
    serializeJson(data, response);

    return response;
}

#endif
