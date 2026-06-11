#ifndef LOGGER_H
#define LOGGER_H

struct FaultLog
{
    unsigned long timestamp;
    int faultType;
};

extern FaultLog faultHistory[10];
extern int logIndex;

void addFaultLog(int fault);

#endif