//
// Created by Cory King on 2/18/17.
//

#ifndef FISHTANKCONTROLLER_RTC_H
#define FISHTANKCONTROLLER_RTC_H

#include <Arduino.h>
#include <RTClib.h>
#include <TimeLib.h>

RTC_DS3231* globalRtc = NULL;


time_t syncRtcTime() {
    Serial.println("Sync RTC Time");

    if (globalRtc != NULL) {
        return globalRtc->now().unixtime();
    } else {
        return 0;
    }
}

RTC_DS3231 *setupRtc(Print *display) {
    RTC_DS3231* rtc = new RTC_DS3231();
    if (! rtc->begin()) {
        display->println("Couldn't find RTC");
        while (1);
    }

    if (rtc->lostPower()) {
        // following line sets the RTC to the date & time this sketch was compiled
        rtc->adjust(DateTime(F(__DATE__), F(__TIME__)));
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

        delay(2000);
    }
    display->print("Found RTC Device");

    setSyncProvider(syncRtcTime);

    return rtc;

}

#endif //FISHTANKCONTROLLER_RTC_H
