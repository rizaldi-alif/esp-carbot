#include "ModeSelection.h"

ModeSelection::ModeSelection(SensorManager &sensors, MotorControls &motorControls) : sensors(sensors), motorControls(motorControls), _mode(0), _baseSpeed(30) {}

void ModeSelection::setMode(int mode)
{
    _mode = mode;
}

void ModeSelection::obstacleAvoidence()
{
    int dist = sensors.readDistance();
    int leftIR = sensors.readLeftIR();
    int rightIR = sensors.readRightIR();
    if (dist > 30 && rightIR != LOW && leftIR != LOW)
    {
        motorControls.setSpeed(_baseSpeed, _baseSpeed);
    }
    else if (dist > 30 && rightIR != LOW && leftIR == LOW)
    {
        motorControls.setSpeed(_baseSpeed / 2, _baseSpeed);
    }
    else if (dist > 30 && rightIR == LOW && leftIR != LOW)
    {
        motorControls.setSpeed(_baseSpeed, _baseSpeed / 2);
    }
    else if (dist <= 30 && rightIR != LOW && leftIR == LOW)
    {
        motorControls.setSpeed(_baseSpeed / 2, _baseSpeed);
    }
    else if (dist <= 30 && rightIR == LOW && leftIR != LOW)
    {
        motorControls.setSpeed(_baseSpeed, _baseSpeed / 2);
    }
    else if (dist <= 30 && rightIR == LOW && leftIR == LOW)
    {
        motorControls.setSpeed(0, 0);
        delayMicroseconds(10);
        motorControls.setSpeed(-_baseSpeed, -_baseSpeed);
    }
    else
    {
        motorControls.setSpeed(0, 0);
    }
}

void ModeSelection::humanFollowing()
{
    int dist = sensors.readDistance();
    if (dist > 30 && dist < 100)
    {
        motorControls.setSpeed(_baseSpeed, _baseSpeed);
    }
    else if (dist <= 30)
    {
        motorControls.setSpeed(0, 0);
    }
}

void ModeSelection::autoPatrol()
{
    int lIR = sensors.readLeftIR();
    int rIR = sensors.readRightIR();
    Serial.print("Left IR: ");
    Serial.print(lIR);
    Serial.print("| Right IR: ");
    Serial.println(rIR);
}

void ModeSelection::loop()
{
    switch (_mode)
    {
    case 2:
        obstacleAvoidence();
        break;
    case 3:
        humanFollowing();
        break;
    case 4:
        autoPatrol();
        break;

    default:
        break;
    }
}