#ifndef BATTERY_H
#define BATTERY_H

struct BatteryPack
{
    float cellVoltage[4];
    float averageVoltage;
    float maxVoltage;
    float minVoltage;
    float imbalancePercent;

    int strongestCell;
    int weakestCell;
};

void calculateBatteryParameters(BatteryPack &pack);

#endif