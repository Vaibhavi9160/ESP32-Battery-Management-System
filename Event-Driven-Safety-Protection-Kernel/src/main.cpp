#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "safety.h"
#include "display.h"

#define CELL1_PIN 34
#define CELL2_PIN 35
#define CELL3_PIN 32
#define CELL4_PIN 33

#define RELAY_PIN 18
#define BUZZER_PIN 19

LiquidCrystal_I2C lcd(0x27, 16, 2);

BatteryData battery;

SystemState currentState = NORMAL;

unsigned long lastReadTime = 0;
unsigned long recoveryStartTime = 0;

const unsigned long RECOVERY_DELAY = 5000;

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
    lcd.print("System Ready");
}

void loop()
{
    unsigned long currentTime = millis();

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
                    showNormal();
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

                    showNormal();
                }

                break;

            default:
                break;
        }
    }
}