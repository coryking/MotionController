//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_SYSTEMSTATE_H
#define MOTIONCONTROLLER_SYSTEMSTATE_H


#include <Arduino.h>
#include <AccelStepper.h>
#include <functional>
#include <Task.h>
#include "FunctionalTask.h"
#include "KeyController.h"

#include "StateMachine.h"
#include "IntervalometerSettings.h"
#include "IntervalometerStateMachine.h"
#include "display.h"


typedef void (*voidFuncPtr)(void);

struct TextData : public EventData {
    String text;
};

struct HomingData : public EventData {
    long sliderSteps;
};

class SystemState : public StateMachine {
public:
    typedef std::function<void()> KeyboardActiveFn;
    typedef std::function<void(TextLine)> SetKeyboardBufferFn;
    SystemState();

    void HomeSlider();
    void HomingComplete(HomingData*);
    void StartAlarm();
    void AlarmFired();
    void BeginShooting();
    void Back();
    void Run();
    void SaveData(TextData*);
    void ShowIntervalSettings();
    void ShowAlarmSettings();
    void ShowTimeSettings();
    void GoIdle();

    void setStepper(AccelStepper *stepper){ this->stepper = stepper;}

    void setKeyboardActiveCb(const KeyboardActiveFn &cb) {
        this->_activeCb = cb;
    }

    void setKeyboardInctiveCb(const KeyboardActiveFn &cb) {
        this->_inactiveCb = cb;
    }

    void setKeyboardBufferCb(const SetKeyboardBufferFn &cb) {
        this->_setBufferCb = cb;
    }

    IntervalometerSettings *getSettings() const {
        return settings;
    }

    bool isShooting() {
        return (
            GetCurrentState() == SystemState::ST_SHOOTING_FRAME ||
            GetCurrentState() == SystemState::ST_SHOOTING_PAUSED ||
            GetCurrentState() == SystemState::ST_SHOOTING_SHUTTER ||
            GetCurrentState() == SystemState::ST_SHOOTING_SHUTTER_WAIT ||
            GetCurrentState() == SystemState::ST_SHOOTING_POSITIONING ||
            GetCurrentState() == SystemState::ST_SHOOTING_SHUTTLE ||
            GetCurrentState() == SystemState::ST_SHOOTING
        );
    }

    uint getFramesRemaining() {
        if (isShooting()) {
            return this->settings->getTotalFrames() - currentFrame;
        } else {
            return 0;
        }
    }

    bool isKeyboardActive() {
        return this->_isKeyboardActive;
    }

    bool showEstimatedDuration() {
        return (this->GetCurrentState() == ST_CONFIG_IV_FRAMES);
    }

    uint getEstimatedDuration(TextData* data) {
        long frames = data->text.toInt();
        return getEstimatedDuration(frames);
    }
    uint getEstimatedDuration(long remainingFrames) {
        if(remainingFrames > 0) {
            return this->settings->estimateDuration(remainingFrames);
        } else {
            return 0;
        }
    }

    uint getEstimatedDurationForTimelapse() {
        if(isShooting()) {
            auto remainingFullMs = this->settings->estimateDurationMs(getFramesRemaining() - 1);
            unsigned long currentDuration = millis() - startMs;
            auto remainingOnFrame = this->settings->getIntervalMs() - currentDuration;
            return (remainingFullMs + remainingOnFrame) / 1000;
        } else {
            return 0;
        }
    }

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
        ST_SHOOTING_FRAME,
        ST_SHOOTING_POSITIONING,
        ST_SHOOTING_SHUTTER,
        ST_SHOOTING_SHUTTER_WAIT,
        ST_SHOOTING_SHUTTLE,
        ST_SHOOTING_PAUSED,
        ST_ASK_START_ALARM,
        ST_ASK_START_SHOOTING,
        ST_MAX_STATES
    };

private:
    void NextStep();

    IntervalometerSettings *settings;
    AccelStepper *stepper;
    KeyboardActiveFn _activeCb;
    KeyboardActiveFn _inactiveCb;
    SetKeyboardBufferFn _setBufferCb;

    // when the shutter started
    unsigned long startMs;

    unsigned int currentFrame;

    void invokeKeyboardActive() {
        if(this->_activeCb)
            this->_activeCb();
    }
    void invokeKeyboardInactive() {
        if(this->_inactiveCb)
            this->_inactiveCb();
    }

    void callBufferCb(const RtcDateTime &dt) {
        callBufferCb(toDateTimeString(dt));
    }
    void callBufferTimeCb(const RtcDateTime &dt) {
        callBufferCb(toTimeString(dt));
    }

    void callBufferCb(String data) {
        if(this->_setBufferCb)
        {
            data.trim();
            TextLine tl;
            tl.text = data;
            tl.position = data.length();
            tl.lastmod = millis();
            this->_setBufferCb(tl);

        }
    }

    void showShootingDisplay(String action) {
        this->showShootingDisplay(action, "");
    }
    void showShootingDisplay(String action, String action2) {
        showShootingScreen(action, action2, currentFrame, this->settings->getTotalFrames());
    }
    bool _isKeyboardActive=false;

    STATE_DECLARE(SystemState, Unhomed, NoEventData);
    STATE_DECLARE(SystemState, Homing, NoEventData);
    STATE_DECLARE(SystemState,  Idle,   NoEventData);
    STATE_DECLARE(SystemState,  WaitForAlarm,   NoEventData);
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
    STATE_DECLARE(SystemState, ShootingFrame, NoEventData);
    EXIT_DECLARE(SystemState, ExitShootingFrame);
    STATE_DECLARE(SystemState, ShootingPositioning, NoEventData);
    STATE_DECLARE(SystemState, ShootingShutter, NoEventData);
    STATE_DECLARE(SystemState, ShootingShutterWait, NoEventData);
    STATE_DECLARE(SystemState, ShootingShuttle, NoEventData);
    STATE_DECLARE(SystemState, ShootingPaused, NoEventData);
    GUARD_DECLARE(SystemState, IsNotMoving, NoEventData);
    STATE_DECLARE(SystemState, AskStartAlarm, NoEventData);
    STATE_DECLARE(SystemState, AskStartShooting, NoEventData);

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
        STATE_MAP_ENTRY_ALL_EX(&ShootingFrame, 0, 0, &ExitShootingFrame)
        STATE_MAP_ENTRY_EX(&ShootingPositioning)
        STATE_MAP_ENTRY_ALL_EX(&ShootingShutter, &IsNotMoving, 0,0)
        STATE_MAP_ENTRY_EX(&ShootingShutterWait)
        STATE_MAP_ENTRY_EX(&ShootingShuttle)
        STATE_MAP_ENTRY_EX(&ShootingPaused)
        STATE_MAP_ENTRY_EX(&AskStartAlarm)
        STATE_MAP_ENTRY_EX(&AskStartShooting)
    END_STATE_MAP_EX

};


#endif //MOTIONCONTROLLER_SYSTEMSTATE_H
