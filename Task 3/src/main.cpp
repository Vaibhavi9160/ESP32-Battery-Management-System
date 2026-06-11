#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "safety.h"
#include "display.h"
#include "hmi.h"

#define CELL1_PIN 34
#define CELL2_PIN 35
#define CELL3_PIN 32
#define CELL4_PIN 33

#define RELAY_PIN 18
#define BUZZER_PIN 19

LiquidCrystal_I2C lcd(0x27, 16, 2);

BatteryData battery;

SystemState currentState = NORMAL;

ScreenType currentScreen = BATTERY_SCREEN;

unsigned long lastReadTime = 0;
unsigned long recoveryStartTime = 0;
unsigned long lastScreenChange = 0;

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