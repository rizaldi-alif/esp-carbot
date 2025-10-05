#pragma once
#include <Arduino.h>
#include <Bluepad32.h>

class ControllerSelection
{
public:
    ControllerSelection();
    void begin();
    void update();
    void setLEDs(uint8_t r, uint8_t g, uint8_t b);
    void setRumble(uint8_t lowFreq, uint8_t highFreq);
    bool isConnected();
    String getName();
    String getAddress();
    int getBatteryLevel(); // 0-100, -1 if not supported
    int getBatteryState(); // 0=discharging, 1=charging, 2=full, -1 if not supported
private:
    BP32 *gamepad = nullptr;
    void selectController();
    void onConnect(BP32 *gp);
    void onDisconnect(BP32 *gp);
    static void staticOnConnect(BP32 *gp, void *context);
    static void staticOnDisconnect(BP32 *gp, void *context);
    bool connected = false;
};