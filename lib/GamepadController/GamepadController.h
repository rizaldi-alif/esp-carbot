#pragma once
#include <Arduino.h>
#include <Bluepad32.h>

class GamepadController
{
public:
    GamepadController();
    void begin();
    void update();

private:
    ControllerPtr myController[BP32_MAX_CONTROLLERS];
    void onConnect(ControllerPtr ctrl);
    void onDisconnect(ControllerPtr ctrl);
};