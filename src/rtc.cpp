//
// Created by Cory King on 5/26/17.
//

#include "rtc.h"
#include <RtcDS3231.h>
#include <TimeLib.h>

RTC_CLOCK globalRtc = RTC_CLOCK(Wire);

time_t syncRtcTime() {
    Serial.println("Sync RTC Time");
    return globalRtc.GetDateTime().Epoch32Time();
}


void setupRtc(Print *display) {
    globalRtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);

    if (! globalRtc.IsDateTimeValid()) {
        // following line sets the RTC to the date & time this sketch was compiled
        globalRtc.SetDateTime(compiled);
        // This line sets the RTC with an explicit date & time, for example to set
        // January 21, 2014 at 3am you would call:
        // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

        delay(2000);
    }

    if (! globalRtc.GetIsRunning()) {
        display->println("Couldn't find RTC");
        globalRtc.SetIsRunning(true);
    }

    RtcDateTime now = globalRtc.GetDateTime();
    if (now < compiled)
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        globalRtc.SetDateTime(compiled);
    }

    globalRtc.Enable32kHzPin(false);
    globalRtc.SetSquareWavePin(DS3231SquareWavePin_ModeAlarmOne);

    display->print("Found RTC Device");

    setSyncProvider(syncRtcTime);

}