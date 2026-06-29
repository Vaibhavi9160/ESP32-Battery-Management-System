#include <math.h>
#include "safety.h"

bool detectWeakCell(BatteryData &battery)
{
    for(int i = 0; i < 4; i++)
    {
        if(battery.cellVoltage[i] < 1.0)
        {
            return true;
        }
    }
    return false;
}

bool detectOverVoltage(BatteryData &battery)
{
    for(int i = 0; i < 4; i++)
    {
        if(battery.cellVoltage[i] > 3.0)
        {
            return true;
        }
    }
    return false;
}

bool detectRapidFluctuation(BatteryData &battery)
{
    static float previousVoltage[4] = {0};

    for(int i = 0; i < 4; i++)
    {
        if(fabs(battery.cellVoltage[i] -
                previousVoltage[i]) > 0.5)
        {
            previousVoltage[i] =
                battery.cellVoltage[i];

            return true;
        }

        previousVoltage[i] =
            battery.cellVoltage[i];
    }

    return false;
}