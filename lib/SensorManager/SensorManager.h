#pragma once
#include <HCSR04.h>

class SensorManager
{
public:
    SensorManager(int trigPin, int echoPin, int leftIR, int rightIR);
    void begin();
    int readDistance();
    int readLeftIR();
    int readRightIR();

private:
    HCSR04 ultrasonic;
    static const int SENSOR_SAMPLES = 5;
    static const int MEDIAN_HISTORY = 5;

    int distBuffer[SENSOR_SAMPLES];
    int distIndex;
    int medianHistory[MEDIAN_HISTORY];
    int medianIndex;

    int leftIRPin;
    int rightIRPin;

    int getMedian(int *arr, int size);
    int getAverage(int *arr, int size);
};
