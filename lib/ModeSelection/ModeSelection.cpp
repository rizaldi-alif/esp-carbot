#include "ModeSelection.h"
SemaphoreHandle_t modeChangeSemaphore = xSemaphoreCreateBinary();

ModeSelection::ModeSelection(SensorManager &sensors, MotorControls &motorControls) : sensors(sensors), motorControls(motorControls), _mode(0), _baseSpeed(30) {}

void ModeSelection::setMode(int mode)
{
    _mode = mode;
    xSemaphoreGive(modeChangeSemaphore);
}

void ModeSelection::obstacleAvoidence()
{
    int dist = sensors.readDistance();

    Serial.print("Mode: Obstacle Avoidance | ");
    Serial.print("Distance: ");
    Serial.print(dist);

    if (dist <= 30)
    {
        int rightDist, leftDist;

        // stop motors if obstacle is too close
        motorControls.setSpeed(0, 0);
        delay(200);

        // turn right
        motorControls.setSpeed(_baseSpeed, -_baseSpeed);
        delay(300);
        motorControls.setSpeed(0, 0);
        delay(500);
        rightDist = sensors.readDistance();

        // turn left
        motorControls.setSpeed(-_baseSpeed, _baseSpeed);
        delay(600);
        motorControls.setSpeed(0, 0);
        delay(500);
        leftDist = sensors.readDistance();

        // decide direction based on distance readings
        Serial.print(" | Right Dist: ");
        Serial.print(rightDist);
        Serial.print(" | Left Dist: ");
        Serial.println(leftDist);

        if (rightDist > leftDist)
        {
            // turn right
            motorControls.setSpeed(_baseSpeed, -_baseSpeed);
            delay(300);
        }
        else if (rightDist < leftDist)
        {
            // turn left
            motorControls.setSpeed(-_baseSpeed, _baseSpeed);
            delay(300);
        }
    }
    else
    {
        motorControls.setSpeed(_baseSpeed, _baseSpeed);
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
    while (true)
    {
        if (xSemaphoreTake(modeChangeSemaphore, portMAX_DELAY))
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
    }
}