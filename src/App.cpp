#include "App.h"
#include <SPIFFS.h>

App::App()
    : rightMotor(16, 17, 3, 4),
      leftMotor(26, 27, 5, 6),
      motorControls(rightMotor, leftMotor),
      webServerManager(motorControls),
      sensorManager(22, 23),
      alarmManager(18, 2),
      lastSensorTime(0),
      sensorDelay(60), ssid("esp32_dev_2"), pass("password") {}

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
    tone(18, 5000);
    delay(1000);
    digitalWrite(2, LOW);
    noTone(18);

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
