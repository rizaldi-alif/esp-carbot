#include "SensorManager.h"
#include <Arduino.h>
#include <string.h>

SensorManager::SensorManager(int trigPin, int echoPin, int leftIR, int rightIR)
    : ultrasonic(trigPin, echoPin), distIndex(0), medianIndex(0), leftIRPin(leftIR), rightIRPin(rightIR) {}

void SensorManager::setup()
{
    pinMode(leftIRPin, INPUT);
    pinMode(rightIRPin, INPUT);
}

int SensorManager::getMedian(int *arr, int size)
{
    int temp[size];
    memcpy(temp, arr, sizeof(int) * size);
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
    return (size % 2 == 1) ? temp[size / 2]
                           : (temp[size / 2 - 1] + temp[size / 2]) / 2;
}

int SensorManager::getAverage(int *arr, int size)
{
    long sum = 0;
    for (int i = 0; i < size; i++)
        sum += arr[i];
    return (int)(sum / size);
}

int SensorManager::readDistance()
{
    int rawDist = ultrasonic.dist();
    distBuffer[distIndex] = rawDist;
    distIndex = (distIndex + 1) % SENSOR_SAMPLES;

    int medianVal = getMedian(distBuffer, SENSOR_SAMPLES);

    medianHistory[medianIndex] = medianVal;
    medianIndex = (medianIndex + 1) % MEDIAN_HISTORY;

    return getAverage(medianHistory, MEDIAN_HISTORY);
}

int SensorManager::readLeftIR()
{
    return digitalRead(leftIRPin);
}

int SensorManager::readRightIR()
{
    return digitalRead(rightIRPin);
}
