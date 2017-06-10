//
// Created by Cory King on 6/3/17.
//

#include <Arduino.h>
#include "display.h"
#include "rtc.h"

hd44780_I2Cexp lcd(LCD_I2CADDR, I2Cexp_BOARD_SAINSMART);

LcdState theLcdState;

void setupLcd() {
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.clear();
    lcd.print("Cory's Badass");
    lcd.setCursor(0, 1);
    lcd.print("Motion Controller");
}

void ICACHE_FLASH_ATTR printText(String text, uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
    lcd.print(text);
}

void ICACHE_FLASH_ATTR centerText(String text, uint8_t row) {
    auto pos = (LCD_COLS - text.length()) / 2;
    printText(text, pos, row);
}

void ICACHE_FLASH_ATTR rightAlign(String text, uint8_t row) {
    auto pos = (LCD_COLS - text.length());
    printText(text, pos, row);
}

void ICACHE_FLASH_ATTR printDateTimeTask(uint32_t deltaTime) {
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
               dt.Second());
    printText(datestring, 0, 3);
}

void ICACHE_FLASH_ATTR moveToInputArea() {
    lcd.setCursor(0, 1);
}

void ICACHE_FLASH_ATTR showConfigText(String text) {
    lcd.clear();
    printText(text, 0, 0);
    moveToInputArea();
}

void ICACHE_FLASH_ATTR showConfigIvFrames() {
    showConfigText("Number of Frames:");
}

void ICACHE_FLASH_ATTR showConfigIvShutterSpeed() {
    showConfigText("Shutter Speed (sec):");
}

void ICACHE_FLASH_ATTR showConfigIvInterval() {
    showConfigText("Interval (sec):");
}

void ICACHE_FLASH_ATTR showConfigAlarmTime() {
    lcd.clear();
    printText("Clock:", 0, 0);
    printText("(yyyymmddhhmmss)", 0, 3);
    moveToInputArea();
}

void ICACHE_FLASH_ATTR showConfigAlarmSetPoint() {
    lcd.clear();
    printText("Start Time:", 0, 0);
    printText("(hh:mm:ss)", 0, 3);
    moveToInputArea();
}

void ICACHE_FLASH_ATTR showHomingScreen() {
    lcd.clear();
    printText("Homing this shit...", 0, 0);
    printText("Finding lower bound", 0, 2);
}

void ICACHE_FLASH_ATTR showHomingLowerBound(long totalSteps) {
    lcd.clear();
    printText("Found Lower Bound!", 0, 0);
    lcd.setCursor(0, 1);
    lcd.print("Total Steps: ");
    lcd.print(totalSteps);
    printText("Moving to Zero...", 0, 2);

}

void ICACHE_FLASH_ATTR showHomingUpperBound(long currentPosition) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Found Upper Bound!");
    lcd.setCursor(0, 1);
    lcd.print("Position is: ");
    lcd.print(currentPosition);
    lcd.setCursor(0, 2);
    lcd.print("Finding Lower Bound");
}

void ICACHE_FLASH_ATTR showHomed() {
    lcd.clear();
}
void ICACHE_FLASH_ATTR showShootingFinished() {
    lcd.clear();
    centerText("Finished Shooting.", 1);
}
void ICACHE_FLASH_ATTR showShootingScreen(String action, String action2, long currentFrame, long framesRemaining) {
    lcd.clear();
    printText(action, 0, 0);
    printText(action2, 0, 1);
    lcd.setCursor(0, 2);
    lcd.print("Frame ");
    lcd.print(currentFrame + 1);
    lcd.print("/");
    lcd.print(framesRemaining);
}

void ICACHE_FLASH_ATTR showYesNowScreen(String question) {
    lcd.clear();
    centerText(question, 1);
    centerText("1 - yes, 2 - no", 2);
}

void ICACHE_FLASH_ATTR showWaitingForAlarmScreen(const RtcDateTime &startTime) {
    lcd.clear();
    centerText("Waiting to begin", 0);

    char datestring[20];

    snprintf_P(datestring,
               20,
               PSTR("%02u:%02u:%02u"),
               startTime.Hour(),
               startTime.Minute(),
               startTime.Second());
    centerText(datestring, 1);
    rightAlign("(C)ancel", 2);
}

void ICACHE_FLASH_ATTR showIdleMenu() {
    lcd.clear();
    printText("A - Set Interval", 0, 0);
    printText("B - Set Alarm", 0, 1);
    printText("C - Set Time", 0, 2);
}

void ICACHE_FLASH_ATTR displayTimeRemaining(long currentDuration, long interval) {
    long remaining = (interval - currentDuration) / 1000;
    lcd.setCursor(0, 3);
    lcd.print(remaining);
    lcd.print("s remaining");
}


const char *spaceString = "                    ";
String fillString(String str, uint8_t fillTo) {
    if (str.length() >= fillTo)
        return str;

    int toFill = fillTo - str.length();

    str += spaceString[20 - toFill];
    return str;
}

void ICACHE_FLASH_ATTR OnHandleLedTask(uint32_t) {
    auto val = analogRead(A0);

    analogWrite(LCD_PWM_PIN, val);

}
