//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_SYSTEMSTATE_H
#define MOTIONCONTROLLER_SYSTEMSTATE_H


#include <Arduino.h>
#include <AccelStepper.h>

#include "StateMachine.h"
#include "IntervalometerSettings.h"
#include "IntervalometerStateMachine.h"


typedef void (*voidFuncPtr)(void);

struct AlarmData : public EventData {
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};

struct TextData : public EventData {
    String text;
};

struct HomingData : public EventData {
    long sliderSteps;
};

class SystemState : public StateMachine {
public:
    SystemState();

    void HomeSlider();
    void HomingComplete(HomingData*);
    void StartAlarm(AlarmData*);
    void AlarmFired();
    void BeginShooting();
    void ShootingComplete();
    void Back();
    void SaveData(TextData*);
    void ShowConfiguration();

    void setStepper(AccelStepper *stepper){ this->stepper = stepper;}

    void run() {
        //if(settings != NULL)
        //    stateMachine.run();
    }

    IntervalometerSettings *getSettings() const {
        return settings;
    }

    bool isKeyboardActive() {
        return this->_isKeyboardActive;
    }

private:
    void NextStep();

    IntervalometerSettings *settings;
    IntervalometerStateMachine stateMachine;
    AccelStepper *stepper;

    bool _isKeyboardActive=false;
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States {
        ST_UNHOMED=0,
        ST_HOMING,
        ST_IDLE,
        ST_WAIT_FOR_ALARM,
        ST_SHOOTING,
        ST_CONFIG_INTERVALOMETER,
        ST_CONFIG_IV_FRAMES,
        ST_CONFIG_IV_SHUTTER_SPEED,
        ST_CONFIG_IV_INTERVAL,
        ST_CONFIG_ALARM,
        ST_CONFIG_ALARM_TIME,
        ST_CONFIG_ALARM_SETPOINT,
        ST_SAVE_IV_FRAMES,
        ST_SAVE_IV_SHUTTER_SPEED,
        ST_SAVE_IV_INTERVAL,
        ST_SAVE_ALARM_TIME,
        ST_SAVE_ALARM_SETPOINT,
        ST_SAVE_HOMING_DATA,
        ST_MAX_STATES
    };

    STATE_DECLARE(SystemState, Unhomed, NoEventData);
    STATE_DECLARE(SystemState, Homing, NoEventData);
    STATE_DECLARE(SystemState,  Idle,   NoEventData);
    STATE_DECLARE(SystemState,  WaitForAlarm,   AlarmData);
    EXIT_DECLARE(SystemState, ExitWaitForAlarm);
    STATE_DECLARE(SystemState,  Shooting,  NoEventData);
    GUARD_DECLARE(SystemState, HasShootingData,NoEventData);
    STATE_DECLARE(SystemState, ConfigIntervalometer, NoEventData);
    STATE_DECLARE(SystemState, ConfigIvFrames, NoEventData);
    STATE_DECLARE(SystemState, ConfigIvShutterSpeed, NoEventData);
    STATE_DECLARE(SystemState, ConfigIvInterval, NoEventData);
    STATE_DECLARE(SystemState, ConfigAlarm, NoEventData);
    STATE_DECLARE(SystemState, ConfigAlarmTime, NoEventData);
    STATE_DECLARE(SystemState, ConfigAlarmSetPoint, NoEventData);
    STATE_DECLARE(SystemState, SaveIvFrames, TextData);
    STATE_DECLARE(SystemState, SaveIvShutterSpeed, TextData);
    STATE_DECLARE(SystemState, SaveIvInterval, TextData);
    STATE_DECLARE(SystemState, SaveAlarmTime, TextData);
    STATE_DECLARE(SystemState, SaveAlarmSetPoint, TextData);
    STATE_DECLARE(SystemState, SaveHomingData, HomingData);
    ENTRY_DECLARE(SystemState, ConfigEntry, NoEventData);
    EXIT_DECLARE(SystemState, ConfigExit);
    ENTRY_DECLARE(SystemState, EnterIdle, NoEventData);
    EXIT_DECLARE(SystemState, ExitIdle);

    // state map to define state function order
    BEGIN_STATE_MAP_EX
        STATE_MAP_ENTRY_EX(&Unhomed)
        STATE_MAP_ENTRY_EX(&Homing)
        STATE_MAP_ENTRY_ALL_EX(&Idle, 0, &EnterIdle, &ExitIdle)
        STATE_MAP_ENTRY_ALL_EX(&WaitForAlarm, 0, 0, &ExitWaitForAlarm)
        STATE_MAP_ENTRY_ALL_EX(&Shooting, &HasShootingData, 0,0)
        STATE_MAP_ENTRY_EX(&ConfigIntervalometer)
        STATE_MAP_ENTRY_ALL_EX(&ConfigIvFrames, 0, &ConfigEntry, &ConfigExit)
        STATE_MAP_ENTRY_ALL_EX(&ConfigIvShutterSpeed, 0, &ConfigEntry, &ConfigExit)
        STATE_MAP_ENTRY_ALL_EX(&ConfigIvInterval, 0, &ConfigEntry, &ConfigExit)
        STATE_MAP_ENTRY_ALL_EX(&ConfigAlarm, 0, &ConfigEntry, &ConfigExit)
        STATE_MAP_ENTRY_ALL_EX(&ConfigAlarmTime, 0, &ConfigEntry, &ConfigExit)
        STATE_MAP_ENTRY_ALL_EX(&ConfigAlarmSetPoint, 0, &ConfigEntry, &ConfigExit)
        STATE_MAP_ENTRY_EX(&SaveIvFrames)
        STATE_MAP_ENTRY_EX(&SaveIvShutterSpeed)
        STATE_MAP_ENTRY_EX(&SaveIvInterval)
        STATE_MAP_ENTRY_EX(&SaveAlarmTime)
        STATE_MAP_ENTRY_EX(&SaveAlarmSetPoint)
        STATE_MAP_ENTRY_EX(&SaveHomingData)
    END_STATE_MAP_EX

};

#endif //MOTIONCONTROLLER_SYSTEMSTATE_H
