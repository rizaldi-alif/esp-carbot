#pragma once
#include <MotorControls.h>
#include <SensorManager.h>

class ModeSelection
{
public:
    ModeSelection(SensorManager &sensors, MotorControls &motorControls);
    void setMode(int mode);
    void loop();

private:
    SensorManager &sensors;
    MotorControls &motorControls;
    void obstacleAvoidence();
    void humanFollowing();
    void autoPatrol();
    int _mode;
    int _baseSpeed;
};