#pragma once
#include <WiFi.h>

class WiFiManager
{
public:
    void begin(const char *ssid, const char *pass);
};
