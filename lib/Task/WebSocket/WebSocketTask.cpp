#include "WebSocketTask.h"

WebSocketsServer WebSocketTask::webSocket = WebSocketsServer(81);
WebSocketTask *WebSocketTask::instance = nullptr;

WebSocketTask::WebSocketTask(MotorControls &motorControls) : _motorControls(motorControls)
{
    _taskHandle = NULL;
}

WebSocketTask::~WebSocketTask()
{
    stopTask();
}

void WebSocketTask::startTask()
{
    if (_taskHandle == NULL)
    {
        instance = this; // Set the static instance pointer to this object
        xTaskCreate(
            webSocketTaskFunction,
            "WebSocketTask",
            _taskStackSize, // Stack size in words
            NULL,
            _taskPriority,
            &_taskHandle);
    }
}

void WebSocketTask::stopTask()
{
    if (_taskHandle != NULL)
    {
        vTaskDelete(_taskHandle);
        _taskHandle = NULL;
    }
}

TaskHandle_t WebSocketTask::getTaskHandle()
{
    return _taskHandle;
}

void WebSocketTask::onWebSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    WebSocketTask *self = WebSocketTask::instance;
    switch (type)
    {
    case WStype_CONNECTED:

        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("Client %u connected from %d.%d.%d.%d\n", num, ip[0], ip[1], ip[2], ip[3]);
        break;
    case WStype_DISCONNECTED:
        Serial.printf("Client %u disconnected\n", num);
        break;
    case WStype_TEXT:
        char *receivedData = (char *)payload;
        Serial.printf("Received data from client %u: %s\n", num, receivedData);
        // Proses data yang diterima dari klien
        // Misalnya, jika data adalah perintah untuk menggerakkan motor
        if (receivedData[0] == 'S')
        {
            char *params = receivedData + 1; // Lewati karakter 'S'
            int rightMotorSpeed = 0, leftMotorSpeed = 0;
            if (sscanf(params, "%d,%d", &rightMotorSpeed, &leftMotorSpeed) == 2)
            {
                self->_motorControls.setSpeed(rightMotorSpeed, leftMotorSpeed);
                Serial.printf("Set motor speeds: Right=%d, Left=%d\n", rightMotorSpeed, leftMotorSpeed);
            }
        }
        if (receivedData[0] == 'M')
        {
            char *params = receivedData + 1; // Lewati karakter 'M'
            int mode = atoi(params);
            Serial.printf("Set mode: %d\n", mode);
            // Set mode sesuai dengan nilai yang diterima
            // Misalnya, self->setMode(mode);
        }
        break;

    default:
        break;
    }
}