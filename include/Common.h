#pragma once

struct SensorData
{
    int x;
    int y;
    int battery;
};

enum CommandType
{
    CMD_NONE,
    CMD_SET_MODE,
    CMD_STOP,
    CMD_START
};

struct Command
{
    CommandType type;
    int value;
};
