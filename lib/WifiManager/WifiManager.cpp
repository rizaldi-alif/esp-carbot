#include "WifiManager.h"

void WifiManager::begin(const char* ssid, const char* pass) {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(ssid, pass);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
}
