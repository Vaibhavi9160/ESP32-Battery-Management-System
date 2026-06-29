#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "display.h"

extern LiquidCrystal_I2C lcd;

void showNormal()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SYSTEM NORMAL");
}

void showFault(String message)
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FAULT");

    lcd.setCursor(0,1);
    lcd.print(message);
}