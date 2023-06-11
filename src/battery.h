#ifndef FILE_BATTERY_H_
#define FILE_BATTERY_H_

#include <SparkFun_MAX1704x_Fuel_Gauge_Arduino_Library.h>
#include "ArduinoJson.h"

#include "network.h"

void initFuelGauge(void);

SFE_MAX1704X lipo(MAX1704X_MAX17048);

String fuelGaugeJson;

double voltage = 0; // Variable to keep track of LiPo voltage
double soc = 0;     // Variable to keep track of LiPo state-of-charge (SOC)
double change_rate = 0;
bool alert; // Variable to keep track of whether alert has been triggered

void initFuelGauge(void)
{
    // FUEL GAUGE
    Wire.begin();
    lipo.enableDebugging(); // Uncomment this line to enable helpful debug messages on Serial
    if (lipo.begin())
    {
        lipo.quickStart();
        lipo.setThreshold(20); // Set alert threshold to 20%.
    }
}

String updateFuelGaugeData(void)
{
    StaticJsonDocument<500> data; // TODO too big

    data["power"]["voltage"] = lipo.getVoltage();
    data["power"]["alert"] = lipo.getAlert();
    data["power"]["change_rate"] = lipo.getChangeRate();
    data["power"]["version"] = lipo.getVersion();
    data["power"]["id"] = lipo.getID();
    data["power"]["compensation"] = lipo.getCompensation();
    data["power"]["soc"] = lipo.getSOC();
    data["power"]["status"] = lipo.getStatus();
    data["power"]["threshold"] = lipo.getThreshold();
    data["power"]["valrt"]["max"] = lipo.getVALRTMax();
    data["power"]["valrt"]["min"] = lipo.getVALRTMin();
    data["power"]["hibrt"]["act_thr"] = lipo.getHIBRTActThr();
    data["power"]["hibrt"]["hib_thr"] = lipo.getHIBRTHibThr();

    data["time"]["now"] = getTime();
    data["time"]["uptime"] = (millis() / 1000);

    String response;
    serializeJson(data, response);

    return response;
}

#endif
