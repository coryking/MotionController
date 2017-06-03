//
// Created by Cory King on 2/18/17.
//

#ifndef FISHTANKCONTROLLER_RTC_H
#define FISHTANKCONTROLLER_RTC_H

#include <Arduino.h>
#include <RtcDS3231.h>
#include <Wire.h>

typedef RtcDS3231<TwoWire> RTC_CLOCK;


extern RTC_CLOCK globalRtc;

void setupRtc(Print *display);

extern volatile bool didGetAlarm;

void attachAlarmInterupt();
void detachAlarmInterupt();

void alarm_callback();

bool Alarmed();

#endif //FISHTANKCONTROLLER_RTC_H
