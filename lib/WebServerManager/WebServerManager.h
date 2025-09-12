#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <MotorControls.h>

class WebServerManager
{
public:
    WebServerManager(MotorControls &motorControls);
    void begin();
    void sendDistance(int distance);
    void sendSSID(const char *ssid);
    void loop();

private:
    AsyncWebServer server;
    AsyncWebSocket ws;
    MotorControls &motorControls;

    void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                 AwsEventType type, void *arg, uint8_t *data, size_t len);
};