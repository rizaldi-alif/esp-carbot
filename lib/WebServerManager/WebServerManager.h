#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <MotorControls.h>
#include <ModeSelection.h>

class WebServerManager
{
public:
    WebServerManager(MotorControls &motorControls, ModeSelection &modeSelection);
    void setup();
    void loop();

private:
    AsyncWebServer server;
    AsyncWebSocket ws;
    MotorControls &motorControls;
    ModeSelection &modeSelection;

    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                 AwsEventType type, void *arg, uint8_t *data, size_t len);
    void sendSpeed(int rightMotorSpeed, int leftMotorSpeed);
};