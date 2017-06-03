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
#include "Pins.h"

extern hd44780_I2Cexp lcd;

void setupLcd();

void showConfigIvFrames();
void showConfigIvShutterSpeed();
void showConfigIvInterval();
void showConfigAlarmTime();
void showConfigAlarmSetPoint();

void showHomingScreen();
void showHomingLowerBound(long);
void showHomingUpperBound(long);
void showHomed();

void showShootingScreen(String action, long currentFrame, long framesRemaining);


String fillString(String str, uint8_t fillTo);

void printDateTimeTask(uint32_t deltaTime);

#endif //MOTIONCONTROLLER_LCD_H
