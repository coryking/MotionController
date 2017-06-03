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
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,

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
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_ALARM
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_ALARM_TIME
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_ALARM_SETPOINT
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_IV_FRAMES,
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_IV_SHUTTER_SPEED,
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_IV_INTERVAL,
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_ALARM_TIME,
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_ALARM_SETPOINT,
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_HOMING_DATA,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
    END_TRANSITION_MAP(NULL)
}

void SystemState::Run() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_IDLE
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
                    TRANSITION_MAP_ENTRY(ST_SHOOTING_FRAME) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(ST_SHOOTING_POSITIONING) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(ST_SHOOTING_SHUTTER) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(ST_SHOOTING_SHUTTER_WAIT) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(ST_SHOOTING_SHUTTLE) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(ST_SHOOTING_PAUSED) // ST_SHOOTING_PAUSED,
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,

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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,

    END_TRANSITION_MAP(data)
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
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
                    TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(ST_CONFIG_ALARM_SETPOINT) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
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

STATE_DEFINE(SystemState,   Shooting,  NoEventData) {
    this->currentFrame = 0;
    this->showShootingDisplay("Starting Timelapse.");
    this->stepper->moveTo(this->settings->getStartPosition());

    InternalEvent(ST_SHOOTING_POSITIONING);
}

STATE_DEFINE(SystemState, ShootingPositioning, NoEventData) {
    if(!this->stepper->isRunning()) {
        InternalEvent(ST_SHOOTING_FRAME);
    }
}

STATE_DEFINE(SystemState, ShootingFrame, NoEventData) {
    this->showShootingDisplay("Taking Picture");
    this->startMs = millis();
    //taskManager.StartTask(this->_showRemainingTime);
    Serial.println("Setting camera pin high!");
    digitalWrite(CAMERA_PIN, HIGH);
    InternalEvent(ST_SHOOTING_SHUTTER);
}


// Do not fucking go into some state when the god damn thing is moving....
GUARD_DEFINE(SystemState, IsNotMoving, NoEventData) {
    return !this->stepper->isRunning();
}

STATE_DEFINE(SystemState, ShootingShutter, NoEventData) {
    unsigned long currentDuration = millis() - startMs;
    if(currentDuration >= this->settings->getShutterTriggerDurationMs()) {
        Serial.println("Setting camera pin low!");
        digitalWrite(CAMERA_PIN, LOW);
        InternalEvent(ST_SHOOTING_SHUTTER_WAIT);
        this->showShootingDisplay("Waiting for shutter.");

    }
}

STATE_DEFINE(SystemState, ShootingShutterWait, NoEventData) {
    unsigned long currentDuration = millis() - startMs;
    if(currentDuration >= this->settings->getShutterSpeedMs()) {
        this->showShootingDisplay("Moving to next", "position...");
        this->stepper->move(this->settings->getRelativeStepsPerFrame());
        InternalEvent(ST_SHOOTING_SHUTTLE);
    }

}

STATE_DEFINE(SystemState, ShootingShuttle, NoEventData) {
    if(!this->stepper->isRunning()) {
        this->showShootingDisplay("Waiting for end", "of interval...");
        InternalEvent(ST_SHOOTING_PAUSED);
    }
}

STATE_DEFINE(SystemState, ShootingPaused, NoEventData) {
    unsigned long currentDuration = millis() - startMs;

    if(currentDuration >= this->settings->getIntervalMs()) {
        Serial.print("Moving out of paused... Requested duration was ");
        Serial.print(this->settings->getIntervalMs());
        Serial.print(" actual duration was ");
        Serial.println(currentDuration);
        this->currentFrame++;
        taskManager.StopTask(_showRemainingTime);
        if(this->currentFrame >= this->settings->getTotalFrames()) {
            showShootingFinished();

            InternalEvent(ST_IDLE);
        } else {
            InternalEvent(ST_SHOOTING_FRAME);
        }
    }
}

void SystemState::showTimeRemaining(uint32_t delta) {
    unsigned long currentDuration = millis() - this->startMs;
    displayTimeRemaining(currentDuration, this->settings->getIntervalMs());
}

SystemState::SystemState() : StateMachine(ST_MAX_STATES) {
    settings = new IntervalometerSettings();
    //_showRemainingTime = new ShutterTimeRemaining(this, MsToTaskTime(250));
}

