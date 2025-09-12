#include "MotorDriver.h"

MotorDriver::MotorDriver(int pin1, int pin2, int ch1, int ch2, int freq, int resolution)
    : _pin1(pin1), _pin2(pin2), _ch1(ch1), _ch2(ch2), _freq(freq), _resolution(resolution) {}

void MotorDriver::begin()
{
    ledcSetup(_ch1, _freq, _resolution);
    ledcSetup(_ch2, _freq, _resolution);
    ledcAttachPin(_pin1, _ch1);
    ledcAttachPin(_pin2, _ch2);
    stop();
}

void MotorDriver::forward(int pwm)
{
    ledcWrite(_ch1, pwm);
    ledcWrite(_ch2, 0);
}

void MotorDriver::backward(int pwm)
{
    ledcWrite(_ch1, 0);
    ledcWrite(_ch2, pwm);
}

void MotorDriver::stop()
{
    ledcWrite(_ch1, 0);
    ledcWrite(_ch2, 0);
}
