#include <SPIFFS.h>
#include "WebServerTask.h"
#include "Routes/WebServerRoutes.h"

AsyncWebServer WebServerTask::server = AsyncWebServer(80);

WebServerTask::WebServerTask()
{
    Serial.println("WebServerTask initialized");
    _taskHandle = NULL;
}

WebServerTask::~WebServerTask()
{
    Serial.println("WebServerTask cleanup...");
    stopTask();
}

void WebServerTask::startTask()
{
    Serial.println("Starting WebServerTask...");
    if (_taskHandle == NULL)
    {
        xTaskCreate(webServerTaskFunction, "WebServerTask", _taskStackSize, NULL, _taskPriority, &_taskHandle);
    }
}

void WebServerTask::stopTask()
{
    if (_taskHandle != NULL)
    {
        vTaskDelete(_taskHandle);
        _taskHandle = NULL;
        Serial.println("WebServerTask stopped.");
    }
}

void WebServerTask::webServerTaskFunction(void *parameter)
{
    WebServerTask *self = static_cast<WebServerTask *>(parameter);
    if (!SPIFFS.begin(true))
    {
        Serial.println("An error has occurred while mounting SPIFFS");
        return;
    }

    WebServerRoutes routes(&server);
    Serial.println("Setting up routes...");
    routes.setupRoutes();
    server.begin();
    Serial.println("WebServerTask started.");
    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(20)); // Adjust delay as needed
    }
}