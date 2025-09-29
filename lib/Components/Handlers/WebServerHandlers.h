#ifndef WEBSERVERHANDLERS_H
#define WEBSERVERHANDLERS_H
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

class WebServerHandlers
{
public:
    WebServerHandlers(AsyncWebServer *server);
    static void handleNotFound(AsyncWebServerRequest *request);

private:
    AsyncWebServer *server;
};
#endif // WEBSERVERHANDLERS_H