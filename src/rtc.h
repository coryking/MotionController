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


/**
 * Parse a date-time string
 * @param dtString in format of yyyymmddhhmmss
 * @return parsed date time
 */
RtcDateTime parseDateTimeString(String dtString);
RtcDateTime parseTimeString(String dtString);

String toDateTimeString(const RtcDateTime &dt);
String toTimeString(const RtcDateTime &dt);

bool Alarmed();

#endif //FISHTANKCONTROLLER_RTC_H
