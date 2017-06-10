//
// Created by Cory King on 5/26/17.
//

#include "rtc.h"
#include <RtcDS3231.h>
#include <TimeLib.h>
#include "Pins.h"

RTC_CLOCK globalRtc = RTC_CLOCK(Wire);

volatile bool didGetAlarm = false;

time_t syncRtcTime() {
    Serial.println("Sync RTC Time");
    return globalRtc.GetDateTime().Epoch32Time();
}


void ICACHE_FLASH_ATTR setupRtc(Print *display) {
    globalRtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    globalRtc.Enable32kHzPin(false);
    globalRtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);

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

    display->print("Found RTC Device");

    globalRtc.LatchAlarmsTriggeredFlags();

    setSyncProvider(syncRtcTime);

}

bool Alarmed() {
    bool wasAlarmed = false;
    //if (didGetAlarm)
    //{
        //wasAlarmed = true;
        //didGetAlarm = false;
        // this gives us which alarms triggered and
        // then allows for others to trigger again
        DS3231AlarmFlag flag = globalRtc.LatchAlarmsTriggeredFlags();

        if (flag & DS3231AlarmFlag_Alarm1)
        {
            wasAlarmed = true;
            Serial.println("alarm one triggered");
        }
    //}
    return wasAlarmed;
}

uint16_t getIntFromString(String str, uint startIndex, uint endIndex) {
    auto subst = str.substring(startIndex, endIndex);
    return subst.toInt();
}

RtcDateTime ICACHE_FLASH_ATTR parseDateTimeString(String dtString) {
    if(dtString.length() != 14)
        return NULL;

    RtcDateTime dt(
        getIntFromString(dtString,0,3),  // yyyy
        getIntFromString(dtString,4,5),  // mm
        getIntFromString(dtString,6,7),  // dd
        getIntFromString(dtString,8,9),  // hh
        getIntFromString(dtString,10,11),  // mm
        getIntFromString(dtString,12,13) // ss
    );

    return dt;
}


RtcDateTime ICACHE_FLASH_ATTR parseTimeString(String dtString) {
    auto now = globalRtc.GetDateTime();
    if(dtString.length() != 8)
        return now;

    RtcDateTime dt(
            now.Year(),
            now.Month(),
            now.Day(),
            getIntFromString(dtString,0,2),  // hh
            getIntFromString(dtString,3,5),  // mm
            getIntFromString(dtString,6,8) // ss
    );

    return dt;
}

String toDateTimeString(const RtcDateTime &dt) {
    char datestring[20];

    snprintf_P(datestring,
               20,
               PSTR("%04u%02u%02u%02u%02u%02u"),
               dt.Year(),
               dt.Month(),
               dt.Day(),
               dt.Hour(),
               dt.Minute(),
               dt.Second() );
    return String(datestring);
}


String toTimeString(const RtcDateTime &dt) {
    char datestring[20];

    snprintf_P(datestring,
               20,
               PSTR("%02u:%02u:%02u"),
               dt.Hour(),
               dt.Minute(),
               dt.Second() );
    return String(datestring);
}