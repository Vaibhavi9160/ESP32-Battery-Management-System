#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "safety.h"
#include "display.h"
#include "hmi.h"

#include "fault_manager.h"
#include "logger.h"
#include <math.h>

#include "cloud_manager.h"

#define CELL1_PIN 34
#define CELL2_PIN 35
#define CELL3_PIN 32
#define CELL4_PIN 33

#define RELAY_PIN 18
#define BUZZER_PIN 19

LiquidCrystal_I2C lcd(0x27, 16, 2);

BatteryData battery;
RuntimeMode runtimeMode = MODE_NORMAL;
FaultType faultType = NO_FAULT;
RuntimeMode previousMode = MODE_NORMAL;

bool sensorDisconnectSent = false;
bool invalidReadingSent = false;
bool frozenADCSent = false;
bool relayMismatchSent = false;

bool relayfeedback = digitalRead(RELAY_PIN); 

SystemState currentState = NORMAL;

ScreenType currentScreen = BATTERY_SCREEN;

float previousVoltage[4] = {0};
unsigned long freezeStartTime = 0;

unsigned long lastReadTime = 0;
unsigned long recoveryStartTime = 0;
unsigned long lastScreenChange = 0;
unsigned long disconnectStartTime = 0;
const unsigned long RECOVERY_DELAY = 5000;
const unsigned long SCREEN_INTERVAL = 3000;

void setup()
{
    Serial.begin(115200);
    connectCloud();
    sendTelemetry("SYSTEM_START");

    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);

    lcd.init();
    lcd.backlight();

    lcd.setCursor(0,0);
    lcd.print("Battery Safety");

    lcd.setCursor(0,1);
    lcd.print("Task 3 HMI");

    delay(1500);

    lcd.clear();
}

void loop()
{
    checkConnection();
    unsigned long currentTime = millis();

    if(currentState == NORMAL)
    {
        if(currentTime - lastScreenChange >= SCREEN_INTERVAL)
        {
            lastScreenChange = currentTime;

            currentScreen =
                (ScreenType)((currentScreen + 1) % 4);
        }
    }

    if(currentTime - lastReadTime >= 500)
    {
        lastReadTime = currentTime;

        battery.cellVoltage[0] =
            analogRead(CELL1_PIN) * 3.3 / 4095.0;

        battery.cellVoltage[1] =
            analogRead(CELL2_PIN) * 3.3 / 4095.0;

        battery.cellVoltage[2] =
            analogRead(CELL3_PIN) * 3.3 / 4095.0;

        battery.cellVoltage[3] =
            analogRead(CELL4_PIN) * 3.3 / 4095.0;

        float averageVoltage =
        (
            battery.cellVoltage[0] +
            battery.cellVoltage[1] +
            battery.cellVoltage[2] +
            battery.cellVoltage[3]
        ) / 4.0;

        float maxVoltage = battery.cellVoltage[0];
        float minVoltage = battery.cellVoltage[0];

        for(int i=1;i<4;i++)
        {
            if(battery.cellVoltage[i] > maxVoltage)
                maxVoltage = battery.cellVoltage[i];

            if(battery.cellVoltage[i] < minVoltage)
                minVoltage = battery.cellVoltage[i];
        }

        float imbalance =
            ((maxVoltage - minVoltage) /
            averageVoltage) * 100.0;

        Serial.print("C1=");
        Serial.print(battery.cellVoltage[0]);

        Serial.print(" C2=");
        Serial.print(battery.cellVoltage[1]);

        Serial.print(" C3=");
        Serial.print(battery.cellVoltage[2]);

        Serial.print(" C4=");
        Serial.println(battery.cellVoltage[3]);

        String runtime = "NORMAL";
        String risk = "LOW";
        String diagnostics = "ALL SYSTEMS OK";
        String recommendation = "Continue Operation";

        sendDashboardData(
            battery.cellVoltage[0],
            battery.cellVoltage[1],
            battery.cellVoltage[2],
            battery.cellVoltage[3],
            runtime,
            risk,
            diagnostics,
            recommendation
        );

    
        Serial.println(runtime);
        Serial.println(risk);
        Serial.println(diagnostics);
        Serial.println(recommendation);

        bool weakCell =
            detectWeakCell(battery);

        bool overVoltage =
            detectOverVoltage(battery);

        bool fluctuation =
            detectRapidFluctuation(battery);

        bool invalidReading = false;
        if(invalidReading)
        {
            if(!invalidReadingSent)
            {
                sendTelemetry("INVALID_READING");
                invalidReadingSent = true;
            }
        }
        else
        {
            invalidReadingSent = false;
        }

        bool frozenADC = true;

for(int i = 0; i < 4; i++)
{
    if(fabs(battery.cellVoltage[i] -
            previousVoltage[i]) > 0.01)
    {
        frozenADC = false;
    }
} 
for(int i = 0; i < 4; i++)
{
    previousVoltage[i] =
        battery.cellVoltage[i];
}
if(frozenADC)
{
    if(freezeStartTime == 0)
    {
        freezeStartTime = millis();
    }
}
else
{
    freezeStartTime = 0;
}

if(freezeStartTime != 0 &&
   millis() - freezeStartTime > 10000)
{
    runtimeMode = MODE_FAILSAFE;
    faultType = FROZEN_ADC;

    addFaultLog(FROZEN_ADC);

    Serial.println("FAULT DETECTED");
    sendTelemetry("FROZEN ADC");
}
if(invalidReading)
{
    if(runtimeMode != MODE_FAILSAFE)
    {
        runtimeMode = MODE_SHUTDOWN;

        Serial.println("SYSTEM SHUTDOWN");
        digitalWrite(RELAY_PIN, LOW);
        digitalWrite(BUZZER_PIN, HIGH);

        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("SYSTEM ");
        lcd.setCursor(0,1);
        lcd.print("SHUTDOWN");
    }
    else
    {
        runtimeMode = MODE_DEGRADED;
    }

    faultType = INVALID_READING;
    addFaultLog(INVALID_READING);

    Serial.println("FAULT DETECTED");
    sendTelemetry("INVALID READING");
}
if(runtimeMode != previousMode)
{
    sendTelemetry("MODE_CHANGED");

    previousMode = runtimeMode;
}
bool sensorDisconnectSent = false;
bool invalidReadingSent = false;
bool frozenADCSent = false;
bool relayMismatchSent = false;

bool sensorDisconnected = false;

for(int i=0;i<4;i++)
{
    if(battery.cellVoltage[i] < 0.30)
    {
        sensorDisconnected = true;
        break;
    }
}

if(sensorDisconnected)
{
    if(!sensorDisconnectSent)
    {
        sendTelemetry("SENSOR_DISCONNECTED");
        sensorDisconnectSent = true;
    }
}
else
{
    sensorDisconnectSent = false;
}

Serial.print("Sensor Disconnected: ");
Serial.println(sensorDisconnected);
if(runtimeMode != previousMode)
{
   switch(runtimeMode)
{
    case MODE_NORMAL:
        Serial.println("NORMAL");
        //Blynk.virtualWrite(V6, "NORMAL");
        break;

    case MODE_DEGRADED:
        Serial.println("DEGRADED");
        //Blynk.virtualWrite(V6, "DEGRADED");
        break;

    case MODE_FAILSAFE:
        Serial.println("FAILSAFE");
        break;

    case MODE_SHUTDOWN:
        Serial.println("SHUTDOWN");
        //Blynk.virtualWrite(V6, "SHUTDOWN");
        break;
}
    if(runtimeMode == MODE_NORMAL)
{
    //Blynk.virtualWrite(V7, "LOW");
}
else if(runtimeMode == MODE_DEGRADED)
{
    //Blynk.virtualWrite(V7, "MEDIUM");
}
else
{
}

if(faultType == NO_FAULT)
{
    //Blynk.virtualWrite(V8, "ALL SYSTEMS OK");
}
else
{
    //Blynk.virtualWrite(V8, "FAULT DETECTED");
}

if(runtimeMode == MODE_NORMAL)
{
    //Blynk.virtualWrite(V9, "Continue Operation");
}
else if(runtimeMode == MODE_DEGRADED)
{
    //Blynk.virtualWrite(V9, "Schedule Maintenance");
}
else
{
    //Blynk.virtualWrite(V9, "Inspect Battery Immediately");
}


    previousMode = runtimeMode;
}
switch(runtimeMode)
{
    case MODE_NORMAL:
        Serial.println("NORMAL");
        break;

    case MODE_DEGRADED:
        Serial.println("DEGRADED");
        break;

    case MODE_FAILSAFE:
        Serial.println("FAILSAFE");
        break;

    case MODE_SHUTDOWN:
        Serial.println("SHUTDOWN");
        break;
}

        switch(currentState)
        {
            case NORMAL:

                if(weakCell)
                {
                    currentState = FAULT;

                    digitalWrite(RELAY_PIN, LOW);
                    digitalWrite(BUZZER_PIN, HIGH);

                    showFault("WEAK CELL");
                }
                else if(overVoltage)
                {
                    currentState = FAULT;

                    digitalWrite(RELAY_PIN, LOW);
                    digitalWrite(BUZZER_PIN, HIGH);

                    showFault("OVERVOLTAGE");
                }
                else if(fluctuation)
                {
                    currentState = FAULT;

                    digitalWrite(RELAY_PIN, LOW);
                    digitalWrite(BUZZER_PIN, HIGH);

                    showFault("INSTABILITY");
                }
                else
                {
                    switch(currentScreen)
                    {
                        case BATTERY_SCREEN:

                            lcd.clear();

                            lcd.setCursor(0,0);
                            lcd.print("C1:");
                            lcd.print(battery.cellVoltage[0],1);

                            lcd.print(" C2:");
                            lcd.print(battery.cellVoltage[1],1);

                            lcd.setCursor(0,1);

                            lcd.print("C3:");
                            lcd.print(battery.cellVoltage[2],1);

                            lcd.print(" C4:");
                            lcd.print(battery.cellVoltage[3],1);

                            break;

                        case ANALYTICS_SCREEN:

                            lcd.clear();

                            lcd.setCursor(0,0);
                            lcd.print("AVG:");
                            lcd.print(averageVoltage,1);

                            lcd.setCursor(0,1);
                            lcd.print("IMB:");
                            lcd.print(imbalance,1);
                            lcd.print("%");

                            break;

                        case STATUS_SCREEN:

                            lcd.clear();

                            lcd.setCursor(0,0);
                            lcd.print("Relay:ON");

                            lcd.setCursor(0,1);
                            lcd.print("State:NORMAL");

                            break;

                        case DIAGNOSTIC_SCREEN:

                            lcd.clear();

                            lcd.setCursor(0,0);
                            lcd.print("Weak:NO");

                            lcd.setCursor(0,1);
                            lcd.print("OverV:NO");

                            break;
                    }
                }

                break;

            case FAULT:

                if(!weakCell &&
                   !overVoltage &&
                   !fluctuation)
                {
                    currentState = RECOVERY;

                    recoveryStartTime = millis();

                    lcd.clear();

                    lcd.setCursor(0,0);
                    lcd.print("RECOVERY");

                    lcd.setCursor(0,1);
                    lcd.print("WAIT 5 SEC");
                }

                break;

            case RECOVERY:

                if(millis() -
                   recoveryStartTime >=
                   RECOVERY_DELAY)
                {
                    currentState = NORMAL;

                    digitalWrite(RELAY_PIN, HIGH);
                    digitalWrite(BUZZER_PIN, LOW);
                }

                break;

            default:
                break;
        }
    }
}
