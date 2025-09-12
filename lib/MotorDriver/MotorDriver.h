#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <Arduino.h>

class MotorDriver
{
public:
    MotorDriver(int pin1, int pin2, int ch1, int ch2, int freq = 5000, int resolution = 8);
    void begin();
    void forward(int pwm);
    void backward(int pwm);
    void stop();

private:
    int _pin1, _pin2;
    int _ch1, _ch2;
    int _freq, _resolution;
};

#endif
