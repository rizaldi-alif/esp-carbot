#pragma once
#include <Arduino.h>

// sertakan header modul seperti aslinya (jangan ubah file-file modul)
#include "WifiManager.h"
#include "WebServerManager.h"
#include "SensorManager.h"
#include "MotorControls.h"
#include "AlarmManager.h"
#include "ModeSelection.h"

// App bertanggung jawab: memanggil setup() modul,
// lalu membuat FreeRTOS task yang memanggil loop() tiap modul.
class App
{
public:
    App() = default;

    // Panggil setup dari semua modul (dipanggil dari main setup)
    void setup();

    // Membuat FreeRTOS tasks untuk tiap modul.
    // startTasks() dibuat terpisah jadi kita bisa memanggilnya dari task starter.
    void startTasks();

private:
    // Instances modul (SAMA seperti di repo-mu; tidak diubah isinya)
    WifiManager wifiManager;
    WebServerManager webServerManager;
    SensorManager sensorManager;
    MotorControls motorControls;
    AlarmManager alarmManager;
    ModeSelection modeSelection;

    // Task wrappers (static agar cocok untuk xTaskCreate)
    static void wifiTask(void *param);
    static void webTask(void *param);
    static void sensorTask(void *param);
    static void motorTask(void *param);
    static void alarmTask(void *param);
    static void modeTask(void *param);

    // Delay period (ms) yang dipakai di vTaskDelay tiap task.
    // Kamu boleh sesuaikan nanti jika perlu.
    static constexpr TickType_t WIFI_PERIOD_MS = pdMS_TO_TICKS(200);  // 5Hz
    static constexpr TickType_t WEB_PERIOD_MS = pdMS_TO_TICKS(50);    // 20Hz
    static constexpr TickType_t SENSOR_PERIOD_MS = pdMS_TO_TICKS(20); // 50Hz
    static constexpr TickType_t MOTOR_PERIOD_MS = pdMS_TO_TICKS(20);  // 50Hz
    static constexpr TickType_t ALARM_PERIOD_MS = pdMS_TO_TICKS(100); // 10Hz
    static constexpr TickType_t MODE_PERIOD_MS = pdMS_TO_TICKS(100);  // 10Hz
};
