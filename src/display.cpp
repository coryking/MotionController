//
// Created by Cory King on 6/3/17.
//

#include <Arduino.h>
#include "display.h"
#include "rtc.h"

hd44780_I2Cexp lcd(LCD_I2CADDR, I2Cexp_BOARD_SAINSMART);

void setupLcd() {
    lcd.begin(20,4);
    lcd.clear();
    lcd.print("Cory's Badass");
    lcd.setCursor(0,1);
    lcd.print("Motion Controller");
}

void printDateTimeTask(uint32_t deltaTime)
{
    RtcDateTime dt = globalRtc.GetDateTime();
    char datestring[20];

    snprintf_P(datestring,
               20,
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second() );
    lcd.setCursor(0,3);
    lcd.print(datestring);
}

void moveToInputArea() {
    lcd.setCursor(0,1);
    lcd.blink();
}

void showConfigIvFrames() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Number of Frames:");
    moveToInputArea();
}

void showConfigIvShutterSpeed() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutter Speed (sec):");
    moveToInputArea();
}

void showConfigIvInterval() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutter Speed (sec):");
    moveToInputArea();
}

void showConfigAlarmTime() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutter Speed (sec):");
    moveToInputArea();
}

void showConfigAlarmSetPoint() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Shutter Speed (sec):");
    moveToInputArea();
}

