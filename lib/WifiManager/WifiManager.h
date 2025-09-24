#pragma once
#include <WiFi.h>

class WifiManager
{
public:
    void begin(const char *ssid, const char *pass);
};
