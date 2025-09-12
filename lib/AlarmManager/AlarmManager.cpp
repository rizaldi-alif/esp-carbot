#include "AlarmManager.h"

AlarmManager::AlarmManager(int buzzerPin, int ledPin)
    : buzzerPin(buzzerPin), ledPin(ledPin), lastAlarmTime(0), alarmDelay(200) {
    pinMode(buzzerPin, OUTPUT);
    pinMode(ledPin, OUTPUT);
}

void AlarmManager::trigger() {
    tone(buzzerPin, 5000);
    digitalWrite(ledPin, HIGH);
    delay(100);
    noTone(buzzerPin);
    digitalWrite(ledPin, LOW);
}

void AlarmManager::loop(bool condition) {
    if (condition && (millis() - lastAlarmTime) > alarmDelay) {
        lastAlarmTime = millis();
        trigger();
    }
}
