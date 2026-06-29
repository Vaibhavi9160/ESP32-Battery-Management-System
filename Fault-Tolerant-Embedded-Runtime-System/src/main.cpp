#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "safety.h"
#include "display.h"
#include "hmi.h"

#include "fault_manager.h"
#include "logger.h"
#include <math.h>

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

bool relayfeedback = true; 

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

        bool weakCell =
            detectWeakCell(battery);

        bool overVoltage =
            detectOverVoltage(battery);

        bool fluctuation =
            detectRapidFluctuation(battery);

            bool invalidreading = false;
            for(int i=0;i<4;i++)
            {
                if(battery.cellVoltage[i] < 0.1 || battery.cellVoltage[i] > 3.2)
                {
                    invalidreading = true;
                    break;
                }
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
    Serial.println("FROZEN ADC");
}
if(invalidreading && runtimeMode != MODE_FAILSAFE)
{runtimeMode = MODE_SHUTDOWN;
    
    Serial.println("SYSTEM SHUTDOWN");
    digitalWrite(RELAY_PIN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SYSTEM ");
    lcd.setCursor(0,1);
    lcd.print("SHUTDOWN");
}
{
    runtimeMode = MODE_DEGRADED;
    faultType = INVALID_READING;

    addFaultLog(INVALID_READING);

    Serial.println("FAULT DETECTED");
    Serial.println("INVALID READING");
}

bool sensorDisconnected = false;
for(int i=0;i<4;i++)
{
    if(battery.cellVoltage[i] < 0.30)
    {
        sensorDisconnected = true;
        break;
    }
}
Serial.print("Sensor Disconnected: ");
Serial.println(sensorDisconnected);

if(sensorDisconnected)
{
    if(disconnectStartTime == 0)
    {
        disconnectStartTime = millis();
    }
}
else
{
    disconnectStartTime = 0;
}

if(disconnectStartTime != 0 &&
   millis() - disconnectStartTime > 5000)
{
    runtimeMode = MODE_DEGRADED;
    faultType = SENSOR_DISCONNECTED;

    addFaultLog(SENSOR_DISCONNECTED);

    Serial.println("FAULT DETECTED");
    Serial.println("SENSOR DISCONNECTED");
}

            if(invalidreading)
            {
                runtimeMode = MODE_DEGRADED;
                faultType = INVALID_READING;

                addFaultLog(INVALID_READING);

                Serial.println("FAULT DETECTED");
                Serial.println("INVALID READING");
            }
            else
            {
                runtimeMode = MODE_NORMAL;
                faultType = NO_FAULT;
            }

            Serial.print("Runtime Mode: ");

            bool relaymismatch = false;
            bool relaycommand = digitalRead(RELAY_PIN);
            if(relaycommand != relayfeedback)
            {
                relaymismatch = true;
                runtimeMode = MODE_FAILSAFE;
                faultType = RELAY_MISMATCH; 
                addFaultLog(RELAY_MISMATCH);
            
            
            Serial.print("FAULT DETECTED: ");
            Serial.println("Relay Mismatch");
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