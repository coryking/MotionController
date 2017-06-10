//
// Created by Cory King on 6/3/17.
//

#ifndef MOTIONCONTROLLER_LCD_H
#define MOTIONCONTROLLER_LCD_H

#include <Arduino.h>
#include <Wire.h>
#include <Task.h>
#include <hd44780.h> // include hd44780 library header file
#include <hd44780ioClass/hd44780_I2Cexp.h> // i/o expander/backpack class
#include <RtcDateTime.h>
#include "Pins.h"

#define LCD_ROWS 4
#define LCD_COLS 20

// how long after a keypress before we turn off the lcd?
#define LCD_TIMEOUT 60 * 1000

extern hd44780_I2Cexp lcd;

void setupLcd();

class LcdState : public Task {
public:
    LcdState() : Task(MsToTaskTime(500)) {}
    bool isOn() { return _isOn; }
    bool markLastKeypress(ulong lastTime) {
        last_keypress = lastTime;
        if(!_isOn) {
            Serial.println("LCD Going On");
            _isOn = true;
            lcd.on();
            return true;
        }
        return false;
    }
    bool handleLcdState() {
        ulong t_diff = millis() - last_keypress;
        if(_isOn && (t_diff > LCD_TIMEOUT)) {
            Serial.println("LCD Going Off");
            lcd.off();
            _isOn = false;
            return true;
        }
        return false;
    }

private:
    virtual void OnUpdate(uint32_t deltaTime) {
        this->handleLcdState();
    }

private:
    bool _isOn = true;
    ulong last_keypress;

};

extern LcdState theLcdState;

void showConfigIvFrames();
void showConfigIvShutterSpeed();
void showConfigIvInterval();
void showConfigAlarmTime();
void showConfigAlarmSetPoint();

void showHomingScreen();
void showHomingLowerBound(long);
void showHomingUpperBound(long);
void showHomed();
void showIdleMenu();

void showEstimatedDuration(ulong);

void showYesNowScreen(String question);
void showWaitingForAlarmScreen(const RtcDateTime &startTime);

void displayTimeRemaining(long currentDuration, long interval);
void showShootingFinished();
void showShootingScreen(String action, String action2, long currentFrame, long framesRemaining);

String fillString(String str, uint8_t fillTo);

void printDateTimeTask(uint32_t deltaTime);

void OnHandleLedTask(uint32_t);

#endif //MOTIONCONTROLLER_LCD_H
