#ifndef CLOUD_MANAGER_H
#define CLOUD_MANAGER_H

#include <Arduino.h>

void connectCloud();
void sendTelemetry(String eventName);
void checkConnection();

void sendDashboardData(
    float c1,
    float c2,
    float c3,
    float c4,
    String runtime,
    String risk,
    String diagnostics,
    String recommendation
);

#endif