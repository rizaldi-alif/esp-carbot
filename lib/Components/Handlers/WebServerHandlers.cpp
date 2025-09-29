#include "WebServerHandlers.h"

WebServerHandlers::WebServerHandlers(AsyncWebServer *server)
{
    this->server = server;
}

void WebServerHandlers::handleNotFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not Found");
}
