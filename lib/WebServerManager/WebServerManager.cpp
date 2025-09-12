#include "WebServerManager.h"

WebServerManager::WebServerManager(MotorControls& motorControls)
    : server(80), ws("/ws"), motorControls(motorControls) {}

void WebServerManager::begin() {
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client,
                      AwsEventType type, void *arg, uint8_t *data, size_t len) {
        this->onEvent(server, client, type, arg, data, len);
    });

    server.addHandler(&ws); 

    server.serveStatic("/", SPIFFS, "/");
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (SPIFFS.exists("/index.html")) {
            request->send(SPIFFS, "/index.html", "text/html");
        } else {
            request->send(404, "text/plain", "File not found");
        }
    });

    server.begin();
}

void WebServerManager::sendDistance(int distance) {
    String json = "{\"distance\":" + String(distance) + "}";
    ws.textAll(json);
}

void WebServerManager::sendSSID(const char* ssid) {
    String json = "{\"ssid\":\"" + String(ssid) + "\"}";
    ws.textAll(json);
}

void WebServerManager::loop() {
    ws.cleanupClients();
}

void WebServerManager::onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
                               AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (type == WS_EVT_CONNECT) {
        Serial.printf("WebSocket client connected: %u\n", client->id());
    } else if (type == WS_EVT_DISCONNECT) {
        Serial.printf("WebSocket client disconnected: %u\n", client->id());
    } else if (type == WS_EVT_DATA) {
        if (len == 0) return;
        char *receivedData = (char *)data;
            if (receivedData[0] == 'S') {
                int rightMotorSpeed = 0, leftMotorSpeed = 0;
            if (sscanf(receivedData + 1, "%d,%d", &rightMotorSpeed, &leftMotorSpeed) == 2) {
                rightMotorSpeed = constrain(rightMotorSpeed, -100, 100);
                leftMotorSpeed = constrain(leftMotorSpeed, -100, 100);
                motorControls.setSpeed(rightMotorSpeed, leftMotorSpeed);
                }
            }
    }
}
