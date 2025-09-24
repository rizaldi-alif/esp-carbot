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

private:
    WifiManager wifiManager;
    WebServerManager webServerManager;
    SensorManager sensorManager;
    AlarmManager alarmManager;
    MotorDriver rightMotor;
    MotorDriver leftMotor;
    MotorControls motorControls;
    ModeSelection modeSelection;

    unsigned long lastSensorTime;
    int sensorDelay;
    char ssid[32];
    char pass[32];
};
