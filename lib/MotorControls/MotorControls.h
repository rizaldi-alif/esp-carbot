#ifndef MOTOR_CONTROLS_H
#define MOTOR_CONTROLS_H

#include <MotorDriver.h>

class MotorControls {
public:
    MotorControls(MotorDriver &rightMotor, MotorDriver &leftMotor);
    void setSpeed(int rightSpeed, int leftSpeed);  // target speed -100..100
    void update();                                 // panggil di loop() untuk ramp-up

private:
    MotorDriver &_rightMotor;
    MotorDriver &_leftMotor;

    int _maxSpeed;
    int _rangePwm;
    int _minPwm;

    int _currentRightSpeed;
    int _currentLeftSpeed;
    int _targetRightSpeed;
    int _targetLeftSpeed;
    int _rampStep;   // kecepatan perubahan tiap update()

    int calculatePwm(int speed) const;
};

#endif
