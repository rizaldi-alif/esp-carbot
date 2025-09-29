#include "WebServerRoutes.h"
WebServerRoutes::WebServerRoutes(AsyncWebServer *server)
{
    this->server = server;
}

void WebServerRoutes::setupRoutes()
{
    server->serveStatic("/", SPIFFS, "/").setDefaultFile("index.html");

    server->on("/", HTTP_GET, [](AsyncWebServerRequest *request)
               {
        if (SPIFFS.exists("/index.html")) {
            request->send(SPIFFS, "/index.html", "text/html");
        } else {
            request->send(404, "text/plain", "File not found");
        } });

    server->onNotFound([this](AsyncWebServerRequest *request)
                       { handlers.handleNotFound(request); });
}