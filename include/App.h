#pragma once
#include <WiFiManager.h>
#include <WebServerManager.h>
#include <SensorManager.h>
#include <AlarmManager.h>
#include <MotorDriver.h>
#include <MotorControls.h>

class App
{
public:
    App();
    void setup();
    void loop();

private:
    WiFiManager wifiManager;
    WebServerManager webServerManager;
    SensorManager sensorManager;
    AlarmManager alarmManager;
    MotorDriver rightMotor;
    MotorDriver leftMotor;
    MotorControls motorControls;

    unsigned long lastSensorTime;
    int sensorDelay;
    char ssid[32];
    char pass[32];
};
