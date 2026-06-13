#include <Arduino.h>
#include "health.h"

String getBatteryHealth(float imbalance)
{
    if(imbalance < 3.0)
        return "HEALTHY";

    else if(imbalance < 7.0)
        return "MINOR IMBALANCE";

    else if(imbalance < 15.0)
        return "CRITICAL IMBALANCE";

    else
        return "PACK FAILURE";
}