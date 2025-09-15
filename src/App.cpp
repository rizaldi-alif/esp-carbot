#include "App.h"
#include <SPIFFS.h>

App::App()
    : rightMotor(16, 17, 22, 5),
      leftMotor(18, 19, 23, 6),
      motorControls(rightMotor, leftMotor),
      webServerManager(motorControls),
      sensorManager(25, 26),
      alarmManager(27, 2),
      lastSensorTime(0),
      sensorDelay(60), ssid("Dor Bot V-2.1"), pass("12345678") {}

void App::setup()
{
    Serial.begin(115200);

    if (!SPIFFS.begin(true))
    {
        Serial.println("SPIFFS mount failed");
        return;
    }

    Serial.println("SPIFFS mounted successfully");

    wifiManager.begin(ssid, pass);
    webServerManager.begin();
    webServerManager.sendSSID(ssid);

    rightMotor.begin();
    leftMotor.begin();

    digitalWrite(2, HIGH);
    tone(27, 5000);
    delay(1000);
    digitalWrite(2, LOW);
    noTone(27);
    delay(100);
    digitalWrite(2, HIGH);
    tone(27, 5000);
    delay(100);
    digitalWrite(2, LOW);
    noTone(27);
    delay(100);
    digitalWrite(2, HIGH);
    tone(27, 5000);
    delay(100);
    digitalWrite(2, LOW);
    noTone(27);

    Serial.println("Setup complete");
}

void App::loop()
{
    if ((millis() - lastSensorTime) > sensorDelay)
    {
        lastSensorTime = millis();
        int distance = sensorManager.readDistance();
        webServerManager.sendDistance(distance);
        alarmManager.loop(distance > 0 && distance < 20);
    }

    motorControls.update();
    webServerManager.loop();
}
