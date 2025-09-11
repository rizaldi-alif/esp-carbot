#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <MotorDriver.h>
#include <MotorControls.h>
#include <HCSR04.h>

#define PWM_A1_PIN 16
#define PWM_A2_PIN 17
#define PWM_B1_PIN 26
#define PWM_B2_PIN 27

#define BUZZER_PIN 18
#define LED_PIN 2
#define TRIG_PIN 22
#define ECHO_PIN 23

const char *ssid = "esp32_coba_3";
const char *pass = "password123";

int lastSensorTime = 0;
int sensorDelay = 60;
bool toClose = false;
int lastAlarmTime = 0;
int alarmDelay = 200;

#define SENSOR_SAMPLES 5 // untuk median
#define MEDIAN_HISTORY 5 // untuk average dari median

int distBuffer[SENSOR_SAMPLES];
int distIndex = 0;

int medianHistory[MEDIAN_HISTORY];
int medianIndex = 0;

int getMedian(int *arr, int size)
{
  int temp[size];
  memcpy(temp, arr, sizeof(int) * size);

  // bubble sort
  for (int i = 0; i < size - 1; i++)
  {
    for (int j = i + 1; j < size; j++)
    {
      if (temp[j] < temp[i])
      {
        int t = temp[i];
        temp[i] = temp[j];
        temp[j] = t;
      }
    }
  }

  if (size % 2 == 1)
    return temp[size / 2];
  else
    return (temp[size / 2 - 1] + temp[size / 2]) / 2;
}

int getAverage(int *arr, int size)
{
  long sum = 0;
  for (int i = 0; i < size; i++)
  {
    sum += arr[i];
  }
  return (int)(sum / size);
}

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
// Objek motor dengan channel unik
MotorDriver rightMotor(PWM_A1_PIN, PWM_A2_PIN, 3, 4);
MotorDriver leftMotor(PWM_B1_PIN, PWM_B2_PIN, 5, 6);
MotorControls motorControls(rightMotor, leftMotor);
HCSR04 ultrasonic(TRIG_PIN, ECHO_PIN);

void wifiInit()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, pass);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
}

void sendDistSensor(int distance)
{
  String json = "{\"distance\":" + String(distance) + "}";
  ws.textAll(json);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client,
             AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  if (type == WS_EVT_CONNECT)
  {
    Serial.printf("WebSocket client connected: %u\n", client->id());
  }
  else if (type == WS_EVT_DISCONNECT)
  {
    Serial.printf("WebSocket client disconnected: %u\n", client->id());
  }
  else if (type == WS_EVT_DATA)
  {
    if (len == 0)
    {
      Serial.println("Received empty data");
      return;
    }

    char *receivedData = (char *)data;

    if (receivedData[0] == 'S')
    {
      int rightMotorSpeed = 0, leftMotorSpeed = 0;

      if (sscanf(receivedData + 1, "%d,%d", &rightMotorSpeed, &leftMotorSpeed) == 2)
      {
        rightMotorSpeed = constrain(rightMotorSpeed, -100, 100);
        leftMotorSpeed = constrain(leftMotorSpeed, -100, 100);

        Serial.printf("Setting motor speeds - Left: %d, Right: %d\n",
                      leftMotorSpeed, rightMotorSpeed);
        motorControls.setSpeed(rightMotorSpeed, leftMotorSpeed);
      }
      else
      {
        Serial.println("Invalid motor speed format");
      }
    }
    else
    {
      Serial.println("Unknown command received");
    }
  }
}

void initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void initRoutes()
{
  server.serveStatic("/", SPIFFS, "/");
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (SPIFFS.exists("/html/index.html"))
              {
                request->send(SPIFFS, "/html/index.html", "text/html");
              }
              else
              {
                request->send(404, "text/plain", "File not found");
              } });
}

void setup()
{
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(115200);

  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS mount failed");
    return;
  }

  wifiInit();
  initWebSocket();
  initRoutes();
  server.begin();

  rightMotor.begin();
  leftMotor.begin();

  digitalWrite(LED_PIN, HIGH);
  tone(BUZZER_PIN, 5000);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  noTone(BUZZER_PIN);
  Serial.println("Setup complete");
}

void loop()
{
  if ((millis() - lastSensorTime) > sensorDelay)
  {
    lastSensorTime = millis();
    int rawDist = ultrasonic.dist();

    // simpan sampel untuk median
    distBuffer[distIndex] = rawDist;
    distIndex = (distIndex + 1) % SENSOR_SAMPLES;

    // hitung median
    int medianVal = getMedian(distBuffer, SENSOR_SAMPLES);

    // simpan median ke buffer history
    medianHistory[medianIndex] = medianVal;
    medianIndex = (medianIndex + 1) % MEDIAN_HISTORY;

    // hitung rata-rata dari median
    int distance = getAverage(medianHistory, MEDIAN_HISTORY);

    // kirim ke websocket
    sendDistSensor(distance);

    // logika alarm
    if (distance > 0 && distance < 20)
      toClose = true;
    else
      toClose = false;
  }

  if (toClose && (millis() - lastAlarmTime) > alarmDelay)
  {
    lastAlarmTime = millis();
    tone(BUZZER_PIN, 5000);
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, LOW);
  }

  motorControls.update();
  ws.cleanupClients();
}