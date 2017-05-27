//
// Created by Cory King on 5/26/17.
//

#include <functional>
#include "SystemState.h"
#include "rtc.h"
#include "Pins.h"

using namespace std;

/*
        ST_UNHOMED=0,
        ST_HOMING,
        ST_IDLE,
        ST_WAIT_FOR_ALARM,
        ST_SHOOTING,
        ST_MAX_STATES
 */

void SystemState::StartAlarm(AlarmData *data) {
    Serial.println("Starting Alarm!");
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
        TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM)   // ST_IDLE
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_WAIT_FOR_ALARM
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
    END_TRANSITION_MAP(data)
}

void SystemState::AlarmFired() {
    Serial.println("Alarm Fired!");
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
    END_TRANSITION_MAP(NULL)
}

void SystemState::BeginShooting() {
    Serial.println("BeginShooting!");
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
        TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_IDLE
        TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_WAIT_FOR_ALARM
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
    END_TRANSITION_MAP(NULL)
}


void SystemState::HomeSlider() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(ST_HOMING)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_HOMING)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
    END_TRANSITION_MAP(NULL)
}

void SystemState::HomingComplete() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(ST_IDLE)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
    END_TRANSITION_MAP(NULL)
}

void SystemState::ShootingComplete() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING
    END_TRANSITION_MAP(NULL)
}


STATE_DEFINE(SystemState,   Homing, NoEventData) {

}

STATE_DEFINE(SystemState, Unhomed, NoEventData) {

}

STATE_DEFINE(SystemState,   Idle,   NoEventData)
{
    // Noop
}

STATE_DEFINE(SystemState,   Shooting,  NoEventData) {
    this->stateMachine.start(this->settings);
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

    globalRtc.LatchAlarmsTriggeredFlags();

}

GUARD_DEFINE(SystemState, HasShootingData, NoEventData) {
    return (this->settings != NULL);
}

EXIT_DEFINE(SystemState, ExitWaitForAlarm) {
    detachInterrupt(RTC_INTERUPT_PIN);
}

SystemState::SystemState() : StateMachine(ST_MAX_STATES) {
    stateMachine.setCloseShutterCb([](){
        Serial.println("Setting camera pin low!");
        digitalWrite(CAMERA_PIN, LOW);
    });

    stateMachine.setOpenShutterCb([](){
        Serial.println("Setting camera pin high!");
        digitalWrite(CAMERA_PIN, HIGH);
    });

    stateMachine.setMoveStepperToRelativeCallback([this](long moveTo){
        stepper->move(moveTo);
    });

    stateMachine.setMoveStepperToAbsolutePositionCb([this](long moveTo){
        stepper->moveTo(moveTo);
    });

    stateMachine.setStepperRunningCallback([this]() {
        return stepper->isRunning();
    });
}
