#include "battery.h"

void calculateBatteryParameters(BatteryPack &pack)
{
    pack.averageVoltage = 0;

    for(int i=0;i<4;i++)
    {
        pack.averageVoltage += pack.cellVoltage[i];
    }

    pack.averageVoltage /= 4.0;

    pack.maxVoltage = pack.cellVoltage[0];
    pack.minVoltage = pack.cellVoltage[0];

    pack.strongestCell = 1;
    pack.weakestCell = 1;

    for(int i=1;i<4;i++)
    {
        if(pack.cellVoltage[i] > pack.maxVoltage)
        {
            pack.maxVoltage = pack.cellVoltage[i];
            pack.strongestCell = i + 1;
        }

        if(pack.cellVoltage[i] < pack.minVoltage)
        {
            pack.minVoltage = pack.cellVoltage[i];
            pack.weakestCell = i + 1;
        }
    }

    if(pack.averageVoltage > 0.1)
    {
        pack.imbalancePercent =
        ((pack.maxVoltage-pack.minVoltage)
        /pack.averageVoltage)*100.0;
    }
    else
    {
        pack.imbalancePercent = 0;
    }
}