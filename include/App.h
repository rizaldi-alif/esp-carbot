#pragma once
#include <WifiManager.h>
#include <WebServerManager.h>
#include <SensorManager.h>
#include <AlarmManager.h>
#include <MotorDriver.h>
#include <MotorControls.h>
#include <ModeSelection.h>

class App
{
public:
    App();
    void setup();
    void loop();

    WifiManager wifiManager;
    WebServerManager webServerManager;
    SensorManager sensorManager;
    MotorDriver rightMotor;
    MotorDriver leftMotor;
    MotorControls motorControls;
    ModeSelection modeSelection;

    unsigned long lastSensorTime;
    int sensorDelay;

private:
    TaskHandle_t motorTaskHandle;
    TaskHandle_t sensorTaskHandle;
    TaskHandle_t webServerTaskHandle;
    TaskHandle_t modeSelectionTaskHandle;

    char ssid[32];
    char pass[32];
};
