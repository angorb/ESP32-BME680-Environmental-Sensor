#ifndef FILE_WEBSERVER_H_
#define FILE_WEBSERVER_H_

#include <ESPAsyncWebSrv.h>
#include "AsyncTCP.h"
#include "AsyncJson.h"

#include "leds.h"
#include "sensor.h"

void setupWebServer(void);
void notFound(AsyncWebServerRequest *request);

AsyncWebServer server(80);

void setupWebServer()
{
    // BEGIN WEB SERVER ROUTES
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(200, "application/json", sensorDataJson); });

    // ADD WEB SERVER EVENT HANDLERS
    server.onNotFound(notFound);

    // START WEB SERVER
    server.begin();
}

// 404 request handler
void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "application/json", "{\"error\":\"404 Not found\"}");
    statusBlink(CRGB::OrangeRed, 2);
}

#endif
