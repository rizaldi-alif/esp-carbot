#include <Arduino.h>
#include "App.h"

App app;

// Task wrapper untuk App (hanya memanggil startTasks)
void appStarterTask(void *pvParameters)
{
  // panggil startTasks (akan membuat task untuk tiap modul)
  app.startTasks();

  // setelah startTasks selesai, tugas starter tidak perlu bekerja lagi
  vTaskDelete(nullptr);
}

void setup()
{
  Serial.begin(115200);
  delay(100);

  Serial.println("=== Starting App (FreeRTOS mode) ===");

  // Panggil setup semua modul (tetap memanggil setup() modul seperti semula)
  app.setup();

  // Buat task kecil untuk memulai pembuatan task modul agar lebih rapi
  xTaskCreatePinnedToCore(
      appStarterTask,
      "AppStarter",
      4096,
      NULL,
      1,
      NULL,
      0);
}

void loop()
{
  // kosong — semua kerja dilakukan oleh FreeRTOS tasks
  vTaskDelay(pdMS_TO_TICKS(1000));
}
