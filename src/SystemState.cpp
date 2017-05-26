//
// Created by Cory King on 5/26/17.
//

#include <functional>
#include "SystemState.h"
#include "rtc.h"
#include "Pins.h"

using namespace std;

void SystemState::StartAlarm(AlarmData *data) {
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM)   // ST_IDLE
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_WAIT_FOR_ALARM
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WAIT_TO_FINISH_SHOOTING
    END_TRANSITION_MAP(data)
}

void SystemState::BeginShooting(ShootingData* data) {
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_IDLE
        TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_WAIT_FOR_ALARM
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WAIT_TO_FINISH_SHOOTING
    END_TRANSITION_MAP(data)
}

STATE_DEFINE(SystemState,   Idle,   NoEventData)
{
    // Noop
}

STATE_DEFINE(SystemState,   Shooting,  ShootingData) {
    data->stateMachine->start(data->settings);
    InternalEvent(ST_WAIT_TO_FINISH_SHOOTING);
}

STATE_DEFINE(SystemState, WaitToFinishShooting, ShootingData) {
    if(data->stateMachine->getCurrentState() == FINISHED)
        InternalEvent(ST_IDLE);
}

STATE_DEFINE(SystemState,   WaitForAlarm, AlarmData) {
    DS3231AlarmOne alarm1(
            data->day,
            data->hour,
            data->minute,
            data->second,
            DS3231AlarmOneControl_HoursMinutesSecondsMatch
    );
    globalRtc.SetAlarmOne(alarm1);


    attachInterrupt(RTC_INTERUPT_PIN, data->interruptServiceCallback, FALLING);

    globalRtc.LatchAlarmsTriggeredFlags();

}

EXIT_DEFINE(SystemState, ExitWaitForAlarm) {
    detachInterrupt(RTC_INTERUPT_PIN);
}