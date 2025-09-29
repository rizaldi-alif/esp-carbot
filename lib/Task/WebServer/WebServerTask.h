#ifndef WEBSERVERTASK_H
#define WEBSERVERTASK_H

#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>

class WebServerTask
{
public:
    WebServerTask();
    ~WebServerTask();
    void startTask();
    void stopTask();

private:
    static void webServerTaskFunction(void *parameter);
    static const uint16_t _taskStackSize = 8192; // Stack size in
    static const UBaseType_t _taskPriority = 1;  // Task priority
    static AsyncWebServer server;
    TaskHandle_t _taskHandle;
};
#endif // WEBSERVERTASK_H