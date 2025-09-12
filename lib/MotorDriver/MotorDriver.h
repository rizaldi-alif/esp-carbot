#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver
{
public:
    MotorDriver(int pin1, int pin2, int enablePin, int ch, int freq = 5000, int resolution = 8);
    void begin();
    void forward();
    void backward();
    void drive(int pwm);
    void stop();

private:
    int _pin1, _pin2;
    int _enablePin;
    int _ch;
    int _freq, _resolution;
};

#endif
