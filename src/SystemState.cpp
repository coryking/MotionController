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

void ICACHE_FLASH_ATTR  SystemState::StartAlarm() {
    Serial.println(F("Starting Alarm!"));
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
        TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM) // ST_ASK_START_ALARM,
        TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM) // ST_ASK_START_SHOOTING,

    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::AlarmFired() {
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::BeginShooting() {
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
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_ASK_START_ALARM,
        TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::Run() {
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::HomeSlider() {
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::HomingComplete(HomingData* data) {
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(data)
}

void ICACHE_FLASH_ATTR  SystemState::Back() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN)   // ST_WAIT_FOR_ALARM
                    TRANSITION_MAP_ENTRY(CANNOT_HAPPEN) // ST_SHOOTING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_CONFIG_INTERVALOMETER
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_FRAMES) // ST_CONFIG_IV_FRAMES
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_CONFIG_IV_SHUTTER_SPEED
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}


void ICACHE_FLASH_ATTR  SystemState::SaveData(TextData* data) {
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(data)
}

void ICACHE_FLASH_ATTR  SystemState::ShowIntervalSettings() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_CONFIG_INTERVALOMETER)   // ST_IDLE
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}


void ICACHE_FLASH_ATTR  SystemState::GoIdle() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_IDLE
                    TRANSITION_MAP_ENTRY(ST_IDLE)   // ST_WAIT_FOR_ALARM
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SHOOTING_PAUSED,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}


void ICACHE_FLASH_ATTR  SystemState::ShowAlarmSettings() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_CONFIG_ALARM)   // ST_IDLE
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::ShowTimeSettings(){
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED)   // ST_HOMING
                    TRANSITION_MAP_ENTRY(ST_CONFIG_ALARM_TIME)   // ST_IDLE
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
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_ASK_START_SHOOTING,
    END_TRANSITION_MAP(NULL)
}

void ICACHE_FLASH_ATTR  SystemState::NextStep() {
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
                    TRANSITION_MAP_ENTRY(ST_ASK_START_SHOOTING) // ST_SAVE_IV_FRAMES,
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_INTERVAL) // ST_SAVE_IV_SHUTTER_SPEED,
                    TRANSITION_MAP_ENTRY(ST_CONFIG_IV_FRAMES) // ST_SAVE_IV_INTERVAL,
                    TRANSITION_MAP_ENTRY(ST_IDLE) // ST_SAVE_ALARM_TIME,
                    TRANSITION_MAP_ENTRY(ST_ASK_START_ALARM) // ST_SAVE_ALARM_SETPOINT
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SAVE_HOMING_DATA,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_FRAME,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_POSITIONING,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTER_WAIT,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_SHUTTLE,
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_SHOOTING_PAUSED,
                    TRANSITION_MAP_ENTRY(ST_WAIT_FOR_ALARM) // ST_ASK_START_ALARM,
                    TRANSITION_MAP_ENTRY(ST_SHOOTING) // ST_ASK_START_SHOOTING,
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
    showIdleMenu();
    // Noop
}

STATE_DEFINE(SystemState,   WaitForAlarm, NoEventData) {
    auto dt = this->settings->getStartTime();
    DS3231AlarmOne alarm1(
            dt.DayOfWeek(),
            dt.Hour(),
            dt.Minute(),
            dt.Second(),
            DS3231AlarmOneControl_HoursMinutesSecondsMatch
    );
    globalRtc.SetAlarmOne(alarm1);

    globalRtc.LatchAlarmsTriggeredFlags();
    showWaitingForAlarmScreen(dt);
}

GUARD_DEFINE(SystemState, HasShootingData, NoEventData) {
    return (this->settings != NULL);
}

EXIT_DEFINE(SystemState, ExitWaitForAlarm) {
}

STATE_DEFINE(SystemState, ConfigIntervalometer, NoEventData) {
    InternalEvent(ST_CONFIG_IV_SHUTTER_SPEED); // Jump right over to first part of wizard....
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
    // Make sure we start with a value that is sane...
    auto proposed_interval = max(this->settings->getShutterSpeedMs()+ (2 * 1000), this->settings->getIntervalMs());
    callBufferCb(String(proposed_interval / 1000));
}

STATE_DEFINE(SystemState, ConfigAlarm, NoEventData) {
    InternalEvent(ST_CONFIG_ALARM_SETPOINT);
}

STATE_DEFINE(SystemState, ConfigAlarmTime, NoEventData) {
    showConfigAlarmTime();
    callBufferCb(globalRtc.GetDateTime());
}

STATE_DEFINE(SystemState, ConfigAlarmSetPoint, NoEventData) {
    showConfigAlarmSetPoint();
    RtcDateTime dt = this->settings->isStartTimeSet() ? this->settings->getStartTime() : globalRtc.GetDateTime();

    callBufferTimeCb(dt);
}

STATE_DEFINE(SystemState, SaveIvFrames, TextData) {
    long frames = data->text.toInt();
    Serial.print(F("Frames: "));
    Serial.println(frames);
    this->settings->setTotalFrames(frames);
    this->NextStep();
}

STATE_DEFINE(SystemState, SaveIvShutterSpeed, TextData) {
    auto ss = data->text.toInt() * 1000;
    Serial.print(F("Shutter Speed: "));
    Serial.println(ss);
    this->settings->setShutterSpeedMs(ss);
    this->NextStep();
}

STATE_DEFINE(SystemState, SaveIvInterval, TextData) {
    long intervalMs = data->text.toInt() * 1000;
    Serial.print(F("Interval: "));
    Serial.println(intervalMs);
    this->settings->setIntervalMs(intervalMs);
    this->NextStep();
}

STATE_DEFINE(SystemState, SaveAlarmTime, TextData) {
    RtcDateTime dt = parseDateTimeString(data->text);
    if(dt != NULL)
        globalRtc.SetDateTime(dt);

    this->NextStep();
}

STATE_DEFINE(SystemState, SaveAlarmSetPoint, TextData) {
    Serial.println(data->text);
    RtcDateTime dt = parseTimeString(data->text);
    Serial.println(dt.Hour());
    Serial.println(dt.Minute());
    Serial.println(dt.Second());
    this->settings->setStartTime(dt);
    this->NextStep();
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
    this->showShootingDisplay(F("Starting Timelapse."));
    this->stepper->moveTo(this->settings->getStartPosition());

    InternalEvent(ST_SHOOTING_POSITIONING);
}

STATE_DEFINE(SystemState, ShootingPositioning, NoEventData) {
    if(!this->stepper->isRunning()) {
        InternalEvent(ST_SHOOTING_FRAME);
    }
}

STATE_DEFINE(SystemState, ShootingFrame, NoEventData) {
    this->showShootingDisplay(F("Taking Picture"));
    this->startMs = millis();
    //taskManager.StartTask(this->_showRemainingTime);
    Serial.println(F("Setting camera pin high!"));
    digitalWrite(CAMERA_PIN, HIGH);
    InternalEvent(ST_SHOOTING_SHUTTER);
}

EXIT_DEFINE(SystemState, ExitShootingFrame) {
    Serial.println("Setting camera pin low!");
    digitalWrite(CAMERA_PIN, LOW);
}


// Do not fucking go into some state when the god damn thing is moving....
GUARD_DEFINE(SystemState, IsNotMoving, NoEventData) {
    return !this->stepper->isRunning();
}

STATE_DEFINE(SystemState, ShootingShutter, NoEventData) {
    unsigned long currentDuration = millis() - startMs;
    if(currentDuration >= this->settings->getShutterTriggerDurationMs()) {
        this->showShootingDisplay("Waiting for shutter.");
        InternalEvent(ST_SHOOTING_SHUTTER_WAIT);
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
        //taskManager.StopTask(_showRemainingTime);
        if(this->currentFrame >= this->settings->getTotalFrames()) {
            showShootingFinished();

            InternalEvent(ST_IDLE);
        } else {
            InternalEvent(ST_SHOOTING_FRAME);
        }
    }
}


STATE_DEFINE(SystemState, AskStartAlarm, NoEventData) {
    showYesNowScreen("Start Alarm?");
}

STATE_DEFINE(SystemState, AskStartShooting, NoEventData) {
    showYesNowScreen("Begin Shooting?");
}

void ICACHE_FLASH_ATTR  SystemState::showTimeRemaining(uint32_t delta) {
    unsigned long currentDuration = millis() - this->startMs;
    displayTimeRemaining(currentDuration, this->settings->getIntervalMs());
}

SystemState::SystemState() : StateMachine(ST_MAX_STATES) {
    settings = new IntervalometerSettings();
    //_showRemainingTime = new ShutterTimeRemaining(this, MsToTaskTime(250));
}

