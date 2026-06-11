#include "logger.h"
#include <chrono>

static unsigned long millis()
{
    using namespace std::chrono;
    return static_cast<unsigned long>(duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count());
}

FaultLog faultHistory[10];
int logIndex = 0;

void addFaultLog(int fault)
{
    faultHistory[logIndex].timestamp = millis();
    faultHistory[logIndex].faultType = fault;

    logIndex++;

    if(logIndex >= 10)
    {
        logIndex = 0;
    }
}