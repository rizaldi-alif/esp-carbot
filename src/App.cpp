#include "App.h"

// Panggil setup() pada semua modul — **logika di dalam setup() tetap utuh**
void App::setup()
{
    Serial.println("[App] setup() - initializing modules...");

    wifiManager.setup();
    webServerManager.setup();
    sensorManager.setup();
    motorControls.setup();
    alarmManager.setup();
    modeSelection.setup();

    Serial.println("[App] setup() - modules initialized");
}

// startTasks membuat FreeRTOS tasks untuk tiap modul.
// Perhatian: kita TIDAK memodifikasi isi setup() / loop() modul.
// Kita hanya memanggil loop() modul di dalam task masing-masing.
void App::startTasks()
{
    // WiFi task → disarankan pada core 0 (agar tidak mengganggu core 1 user tasks)
    xTaskCreatePinnedToCore(
        wifiTask, "WifiTask", 4096, this, 1, NULL, 0);

    // Web server task → core 0 (web + wifi cenderung bekerja bersama)
    xTaskCreatePinnedToCore(
        webTask, "WebTask", 8192, this, 1, NULL, 0);

    // Sensor task → core 1
    xTaskCreatePinnedToCore(
        sensorTask, "SensorTask", 4096, this, 1, NULL, 1);

    // Motor control task → core 1
    xTaskCreatePinnedToCore(
        motorTask, "MotorTask", 4096, this, 1, NULL, 1);

    // Alarm task → core 1
    xTaskCreatePinnedToCore(
        alarmTask, "AlarmTask", 4096, this, 1, NULL, 1);

    // Mode selection task → core 1
    xTaskCreatePinnedToCore(
        modeTask, "ModeTask", 4096, this, 1, NULL, 1);

    Serial.println("[App] startTasks() - all tasks created");
}

/* ===== Task Implementations =====
   Setiap task memanggil modul.loop() secara periodik.
   Catatan penting: jika modul.loop() mengandung blocking long delay() maka
   tugas tersebut hanya akan tidur/busy di dalam modul — dan itu tidak ideal.
   Jika modul punya delay() panjang => sebaiknya ubah delay() menjadi non-blocking
   (vTaskDelay) di dalam modul, atau laporkan ke saya supaya saya bantu refactor.
*/

void App::wifiTask(void *param)
{
    App *self = static_cast<App *>(param);
    // loop forever
    for (;;)
    {
        self->wifiManager.loop();
        vTaskDelay(WIFI_PERIOD_MS);
    }
}

void App::webTask(void *param)
{
    App *self = static_cast<App *>(param);
    for (;;)
    {
        self->webServerManager.loop();
        vTaskDelay(WEB_PERIOD_MS);
    }
}

void App::sensorTask(void *param)
{
    App *self = static_cast<App *>(param);
    for (;;)
    {
        self->sensorManager.loop();
        vTaskDelay(SENSOR_PERIOD_MS);
    }
}

void App::motorTask(void *param)
{
    App *self = static_cast<App *>(param);
    for (;;)
    {
        self->motorControls.loop();
        vTaskDelay(MOTOR_PERIOD_MS);
    }
}

void App::alarmTask(void *param)
{
    App *self = static_cast<App *>(param);
    for (;;)
    {
        self->alarmManager.loop();
        vTaskDelay(ALARM_PERIOD_MS);
    }
}

void App::modeTask(void *param)
{
    App *self = static_cast<App *>(param);
    for (;;)
    {
        self->modeSelection.loop();
        vTaskDelay(MODE_PERIOD_MS);
    }
}
