/**
 * @file webserver.h
 * @author nick brogna (nick@wemakepretty.com)
 * @brief
 * @version 0.1
 * @date 2023-06-11
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef FILE_WEBSERVER_H_
#define FILE_WEBSERVER_H_

#include <ESPAsyncWebSrv.h>
#include "AsyncTCP.h"
#include "AsyncJson.h"

#include "leds.h"
#include "sensor.h"
#include "battery.h"

void setupWebServer(void);
void notFound(AsyncWebServerRequest *request);

AsyncWebServer server(80);

void setupWebServer()
{
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    // BEGIN WEB SERVER ROUTES
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", sensorDataJson); });
    server.on("/sensor", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", sensorDataJson); });
    server.on("/power", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", fuelGaugeJson); });
    server.on("/files", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", filesystemDataJson); });

    // ADD WEB SERVER EVENT HANDLERS
    server.onNotFound(notFound);

    // START WEB SERVER
    server.begin();
}

// 404 request handler
void notFound(AsyncWebServerRequest *request)
{
    if (request->method() == HTTP_OPTIONS)
    {
        request->send(200);
    }
    else
    {
        request->send(404, "application/json", "{\"message\":\"Not found\"}");
    }
    statusBlink(CRGB::OrangeRed, 2);
}

#endif
