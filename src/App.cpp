#include "App.h"
#include <SPIFFS.h>

App::App()
    : rightMotor(16, 17, 22, 5),
      leftMotor(18, 19, 23, 6),
      motorControls(rightMotor, leftMotor),
      sensorManager(25, 26, 35, 34, 27, 2),
      modeSelection(sensorManager, motorControls),
      webServerManager(motorControls, modeSelection),
      lastSensorTime(0),
      sensorDelay(60), ssid("Dev-v3.0"), pass("12345678") {}

void motorTask(void *pvParameters)
{
    App *app = static_cast<App *>(pvParameters);
    for (;;)
    {
        app->motorControls.update();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void sensorTask(void *pvParameters)
{
    App *app = static_cast<App *>(pvParameters);
    for (;;)
    {
        unsigned long currentMillis = millis();
        if (currentMillis - app->lastSensorTime >= app->sensorDelay)
        {
            app->sensorManager.loop();
            app->lastSensorTime = currentMillis;
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void webServerTask(void *pvParameters)
{
    App *app = static_cast<App *>(pvParameters);
    for (;;)
    {
        app->webServerManager.loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void modeSelectionTask(void *pvParameters)
{
    App *app = static_cast<App *>(pvParameters);
    for (;;)
    {
        app->modeSelection.loop();
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

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
    sensorManager.begin();

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

    xTaskCreatePinnedToCore(motorTask, "Motor Task", 4096, this, 1, &motorTaskHandle, 1);
    xTaskCreatePinnedToCore(sensorTask, "Sensor Task", 4096, this, 1, &sensorTaskHandle, 1);
    xTaskCreatePinnedToCore(webServerTask, "WebServer Task", 8192, this, 1, &webServerTaskHandle, 0);
    xTaskCreatePinnedToCore(modeSelectionTask, "Mode Selection Task", 4096, this, 1, &modeSelectionTaskHandle, 1);
}

void App::loop()
{
    webServerManager.loop();
    modeSelection.loop();
}
