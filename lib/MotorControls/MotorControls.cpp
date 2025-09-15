#include "MotorControls.h"

MotorControls::MotorControls(MotorDriver &rightMotor, MotorDriver &leftMotor)
    : _rightMotor(rightMotor), _leftMotor(leftMotor),
      _maxSpeed(100), _rangePwm(0), _minPwm(500),
      _currentRightSpeed(0), _currentLeftSpeed(0),
      _targetRightSpeed(0), _targetLeftSpeed(0),
      _rampStep(5) {
        _rangePwm = (1 << 10) - 1 - _minPwm; // Assuming 10-bit resolution
      }

int MotorControls::calculatePwm(int speed) const
{
    speed = abs(speed);
    speed = constrain(speed, 0, _maxSpeed); 
    float ratio = (float)speed / _maxSpeed;
    return (int)(_rangePwm * ratio + _minPwm);
}

void MotorControls::setSpeed(int rightSpeed, int leftSpeed)
{
    _targetRightSpeed = constrain(rightSpeed, -_maxSpeed, _maxSpeed);
    _targetLeftSpeed = constrain(leftSpeed, -_maxSpeed, _maxSpeed);
}

void MotorControls::update()
{
    // Smooth ramp-up/down for right motor
    if (_currentRightSpeed < _targetRightSpeed)
        _currentRightSpeed += _rampStep;
    else if (_currentRightSpeed > _targetRightSpeed)
        _currentRightSpeed -= _rampStep;

    // Smooth ramp-up/down for left motor
    if (_currentLeftSpeed < _targetLeftSpeed)
        _currentLeftSpeed += _rampStep;
    else if (_currentLeftSpeed > _targetLeftSpeed)
        _currentLeftSpeed -= _rampStep;

    // Apply to hardware
    if (_currentRightSpeed > 0)
    {
        _rightMotor.forward();
    }
    else if (_currentRightSpeed < 0)
    {
        _rightMotor.backward();
    }
    else
    {
        _rightMotor.stop();
    }

    if (_currentLeftSpeed > 0)
    {
        _leftMotor.forward();
    }
    else if (_currentLeftSpeed < 0)
    {
        _leftMotor.backward();
    }
    else
    {
        _leftMotor.stop();
    }

    _rightMotor.drive(calculatePwm(_currentRightSpeed));
    _leftMotor.drive(calculatePwm(_currentLeftSpeed));
}
