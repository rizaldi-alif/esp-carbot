#include "MotorDriver.h"

MotorDriver::MotorDriver(int pin1, int pin2, int enablePin, int ch, int freq, int resolution)
    : _pin1(pin1), _pin2(pin2), _enablePin(enablePin), _ch(ch), _freq(freq), _resolution(resolution) {}

void MotorDriver::begin()
{
    pinMode(_pin1, OUTPUT);
    pinMode(_pin2, OUTPUT);
    pinMode(_enablePin, OUTPUT);
    ledcSetup(_ch, _freq, _resolution);
    ledcAttachPin(_enablePin, _ch);
    Serial.printf("MotorDriver initialized on pins %d, %d with enable pin %d on channel %d\n", _pin1, _pin2, _enablePin, _ch);
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
    ledcWrite(_ch, 0);
}

void MotorDriver::drive(int pwm)
{
    pwm = abs(pwm);
    pwm = constrain(pwm, 0, 1023);
    ledcWrite(_ch, pwm);
    Serial.printf("Driving motor on channel %d with PWM %d\n", _ch, pwm);
}
