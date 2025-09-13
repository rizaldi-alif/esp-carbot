#pragma once
#include <Arduino.h>

class AlarmManager
{
public:
    AlarmManager(int buzzerPin, int ledPin);
    void trigger();
    void loop(bool condition);

private:
    int buzzerPin;
    int ledPin;
    unsigned long lastAlarmTime;
    int alarmDelay;
};
