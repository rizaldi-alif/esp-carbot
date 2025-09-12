#include "MotorDriver.h"

MotorDriver::MotorDriver(int pin1, int pin2, int enablePin, int ch, int freq, int resolution)
    : _pin1(pin1), _pin2(pin2), _enablePin(enablePin), _ch(ch), _freq(freq), _resolution(resolution) {}

void MotorDriver::begin()
{
    ledcSetup(_ch, _freq, _resolution);
    ledcAttachPin(_enablePin, _ch);
    stop();
}

void MotorDriver::forward()
{
    digitalWrite(_pin1, HIGH);
    digitalWrite(_pin2, LOW);
}

void MotorDriver::backward()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, HIGH);
}

void MotorDriver::stop()
{
    digitalWrite(_pin1, LOW);
    digitalWrite(_pin2, LOW);
}

void MotorDriver::drive(int pwm)
{
    ledcWrite(_ch, pwm);
}