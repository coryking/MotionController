//
// Created by Cory King on 5/26/17.
//

#include <functional>
#include "SystemState.h"
#include "display.h"
#include "rtc.h"
#include "Pins.h"
#include "tasker.h"

FunctionTask taskPrintDateTime(printDateTimeTask, MsToTaskTime(500));


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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_FRAMES
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_SHUTTER_SPEED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_INTERVAL
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
        TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM) // ST_SAVE_ALARM_SETPOINT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
    END_TRANSITION_MAP(data)
}

void SystemState::AlarmFired() {
    Serial.println("Alarm Fired!");
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(ST_SHOOTING)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
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
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_CONFIG_INTERVALOMETER
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_CONFIG_IV_FRAMES
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_CONFIG_IV_SHUTTER_SPEED
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_CONFIG_IV_INTERVAL
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
    END_TRANSITION_MAP(NULL)
}


void SystemState::HomeSlider() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(ST_HOMING)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_HOMING)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
    END_TRANSITION_MAP(NULL)
}

void SystemState::HomingComplete(HomingData* data) {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(ST_SAVE_HOMING_DATA)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
    END_TRANSITION_MAP(data)
}

void SystemState::ShootingComplete() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,

    END_TRANSITION_MAP(NULL)
}

void SystemState::Back() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_FRAMES) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_SHUTTER_SPEED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_SHUTTER_SPEED) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(ST_CONFIG_ALARM_TIME) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,

    END_TRANSITION_MAP(NULL)
}


void SystemState::SaveData(TextData* data) {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(ST_SAVE_IV_FRAMES) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(ST_SAVE_IV_SHUTTER_SPEED) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(ST_SAVE_IV_INTERVAL) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(ST_SAVE_ALARM_TIME) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(ST_SAVE_ALARM_SETPOINT) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,

    END_TRANSITION_MAP(data)
}

void SystemState::ShowConfiguration() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_CONFIG_INTERVALOMETER)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(ST_CONFIG_INTERVALOMETER)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_FRAMES) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_SHUTTER_SPEED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(ST_CONFIG_INTERVALOMETER) // ST_ALARM
                    TRANSITION_MAP_ENTRY(ST_CONFIG_INTERVALOMETER) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(ST_CONFIG_INTERVALOMETER) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,

    END_TRANSITION_MAP(NULL)
}

void SystemState::NextStep() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_SHUTTER_SPEED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_IV_INTERVAL
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_TIME
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_SHUTTER_SPEED) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_INTERVAL) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(ST_CONFIG_ALARM) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(ST_CONFIG_ALARM_SETPOINT) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,

    END_TRANSITION_MAP(NULL)
}

STATE_DEFINE(SystemState,   Homing, NoEventData) {

}

STATE_DEFINE(SystemState, SaveHomingData, HomingData) {
    this->settings->setEndPosition(data->sliderSteps);
    InternalEvent(ST_IDLE);
}

STATE_DEFINE(SystemState, Unhomed, NoEventData) {

}

STATE_DEFINE(SystemState,   Idle,   NoEventData)
{
    Serial.println("Going idle.... whew....");
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
    attachAlarmInterupt();

}

GUARD_DEFINE(SystemState, HasShootingData, NoEventData) {
    return (this->settings != NULL);
}

EXIT_DEFINE(SystemState, ExitWaitForAlarm) {
    detachAlarmInterupt();
}

STATE_DEFINE(SystemState, ConfigIntervalometer, NoEventData) {
    InternalEvent(ST_CONFIG_IV_FRAMES); // Jump right over to first part of wizard....
}


STATE_DEFINE(SystemState, ConfigIvFrames, NoEventData) {
    showConfigIvFrames();
    callBufferCb(String(this->settings->getTotalFrames()));

}

STATE_DEFINE(SystemState, ConfigIvShutterSpeed, NoEventData) {
    showConfigIvShutterSpeed();
    callBufferCb(String(this->settings->getShutterSpeedMs() / 1000));

}

STATE_DEFINE(SystemState, ConfigIvInterval, NoEventData) {
    showConfigIvInterval();
    callBufferCb(String(this->settings->getIntervalMs() / 1000));
}

STATE_DEFINE(SystemState, ConfigAlarm, NoEventData) {
    InternalEvent(ST_CONFIG_ALARM_SETPOINT);
}

STATE_DEFINE(SystemState, ConfigAlarmTime, NoEventData) {

}

STATE_DEFINE(SystemState, ConfigAlarmSetPoint, NoEventData) {

}

STATE_DEFINE(SystemState, SaveIvFrames, TextData) {
    long frames = data->text.toInt();
    Serial.print("Frames: ");
    Serial.println(frames);
    this->settings->setTotalFrames(frames);
    this->NextStep();
}

STATE_DEFINE(SystemState, SaveIvShutterSpeed, TextData) {
    auto ss = data->text.toInt() * 1000;
    Serial.print("Shutter Speed: ");
    Serial.println(ss);
    this->settings->setShutterSpeedMs(ss);
    this->NextStep();
}

STATE_DEFINE(SystemState, SaveIvInterval, TextData) {
    long intervalMs = data->text.toInt() * 1000;
    Serial.print("Interval: ");
    Serial.println(intervalMs);
    this->settings->setIntervalMs(intervalMs);
    this->NextStep();
}

STATE_DEFINE(SystemState, SaveAlarmTime, TextData) {

}

STATE_DEFINE(SystemState, SaveAlarmSetPoint, TextData) {

}

ENTRY_DEFINE(SystemState, ConfigEntry, NoEventData) {
    this->_isKeyboardActive = true;
    this->invokeKeyboardActive();
}

EXIT_DEFINE(SystemState, ConfigExit) {
    this->_isKeyboardActive = false;
    this->invokeKeyboardInactive();
}

ENTRY_DEFINE(SystemState, EnterIdle, NoEventData) {
    taskManager.StartTask(&taskPrintDateTime);
}

EXIT_DEFINE(SystemState,ExitIdle) {
    taskManager.StopTask(&taskPrintDateTime);
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

    settings = new IntervalometerSettings();
}

