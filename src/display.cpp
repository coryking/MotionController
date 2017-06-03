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
    lcd.print("Interval (sec):");
    moveToInputArea();
}

void showConfigAlarmTime() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Clock:");
    lcd.setCursor(3,0);
    lcd.print("(yyyymmddhhmmss)");
    moveToInputArea();
}

void showConfigAlarmSetPoint() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Start Time:");
    lcd.setCursor(3,0);
    lcd.print("(yyyymmddhhmmss)");
    moveToInputArea();
}

void showHomingScreen() {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Homing this shit...");
    lcd.setCursor(0,2);
    lcd.print("Finding lower bound");
}

void showHomingLowerBound(long totalSteps) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Found Lower Bound!");
    lcd.setCursor(0,1);
    lcd.print("Total Steps: ");
    lcd.print(totalSteps);
    lcd.setCursor(0,2);
    lcd.print("Moving to Zero...");

}

void showHomingUpperBound(long currentPosition) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Found Upper Bound!");
    lcd.setCursor(0,1);
    lcd.print("Position is: ");
    lcd.print(currentPosition);
    lcd.setCursor(0,2);
    lcd.print("Finding Lower Bound");
}

void showHomed() {
    lcd.clear();
}

void showShootingScreen(String action, String action2, long currentFrame, long framesRemaining) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(action);
    lcd.setCursor(0,1);
    lcd.print(action2);
    lcd.setCursor(0,2);
    lcd.print(currentFrame);
    lcd.print(" of ");
    lcd.print(framesRemaining);
}



const char *spaceString = "                    ";
String fillString(String str, uint8_t fillTo) {
    if(str.length() >= fillTo)
        return str;

    int toFill = fillTo - str.length();

    str += spaceString[20 - toFill];
    return str;
}