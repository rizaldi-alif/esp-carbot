#ifndef WEBSOCKET_TASK_H
#define WEBSOCKET_TASK_H

#include <Arduino.h>
#include <WebSocketsServer.h>
#include "MotorControls.h"

class WebSocketTask
{
public:
    WebSocketTask(MotorControls &motorControls);
    ~WebSocketTask();
    void startTask();
    void stopTask();
    void sendDataToClient(String &jsonData)
        TaskHandle_t
        getTaskHandle();

private:
    const uint32_t _taskStackSize = 8192; // Ukuran stack task dalam byte
    const UBaseType_t _taskPriority = 2;
    MotorControls &_motorControls;

    static void webSocketTaskFunction(void *parameter);
    static void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);

    static WebSocketTask *instance;
    static WebSocketsServer *webSocket;

    TaskHandle_t _taskHandle = nullptr;
};

#endif