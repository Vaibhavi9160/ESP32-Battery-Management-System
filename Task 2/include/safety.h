#ifndef SAFETY_H
#define SAFETY_H

enum SystemState
{
    NORMAL,
    WARNING,
    FAULT,
    RECOVERY
};

struct BatteryData
{
    float cellVoltage[4];
};

bool detectWeakCell(BatteryData &battery);
bool detectOverVoltage(BatteryData &battery);
bool detectRapidFluctuation(BatteryData &battery);

#endif