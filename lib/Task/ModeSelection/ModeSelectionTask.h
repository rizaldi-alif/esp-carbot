#ifndef MODE_SELECTION_TASK_H
#define MODE_SELECTION_TASK_H

#include <Arduino.h>
#include "MotorControls.h"

class ModeSelectionTask
{
public:
    ModeSelectionTask(MotorControls &motorControls);
    ~ModeSelectionTask();

    void startTask();
    void stopTask();

private:
    static void modeSelectionTaskFunction(void *parameter);
    MotorControls &_motorControls;
    TaskHandle_t _taskHandle;
    const uint32_t _taskStackSize = 2048;
    const UBaseType_t _taskPriority = 1;
};

#endif // MODE_SELECTION_TASK_H