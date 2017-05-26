//
// Created by Cory King on 5/26/17.
//

#include <functional>
#include "SystemState.h"
#include "rtc.h"
#include "Pins.h"

void SystemState::StartAlarm(AlarmSettings *alarmSettings, voidFuncPtr cb) {
    DS3231AlarmOne alarm1(
            alarmSettings->day,
            alarmSettings->hour,
            alarmSettings->minute,
            alarmSettings->second,
            DS3231AlarmOneControl_HoursMinutesSecondsMatch
    );
    globalRtc.SetAlarmOne(alarm1);


    attachInterrupt(RTC_INTERUPT_PIN, cb, FALLING);

    globalRtc.LatchAlarmsTriggeredFlags();

    this->setState(SYSTEM_WAIT_FOR_ALARM);
}

void SystemState::BeginShooting(IntervalometerSettings intervalometerSettings,  IntervalometerStateMachine* intervalometerStateMachine) {
    intervalometerStateMachine->start(&intervalometerSettings);
    this->setState(SYSTEM_SHOOTING);
}

void SystemState::AlarmTriggered(IntervalometerSettings intervalometerSettings, IntervalometerStateMachine * intervalometerStateMachine) {
    detachInterrupt(RTC_INTERUPT_PIN);

    if(this->getCurrentState() == SYSTEM_WAIT_FOR_ALARM) {
        this->BeginShooting(intervalometerSettings, intervalometerStateMachine);
    }
}

void SystemState::setState(SystemStates newState) {
    this->currentState = newState;
}

SystemStates SystemState::getCurrentState() {
    return this->currentState;
}
