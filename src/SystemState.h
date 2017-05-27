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

class SystemState : public StateMachine {
public:
    SystemState();

    void HomeSlider();
    void HomingComplete();
    void StartAlarm(AlarmData*);
    void AlarmFired();
    void BeginShooting();
    void ShootingComplete();

    void setStepper(AccelStepper *stepper){ this->stepper = stepper;}

    void run() {
        if(settings != NULL)
            stateMachine.run();
    }

    IntervalometerSettings *getSettings() const {
        return settings;
    }

    void setSettings(IntervalometerSettings *settings) {
        this->settings = settings;
    }


private:
    IntervalometerSettings *settings;
    IntervalometerStateMachine stateMachine;
    AccelStepper *stepper;
    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States {
        ST_UNHOMED=0,
        ST_HOMING,
        ST_IDLE,
        ST_WAIT_FOR_ALARM,
        ST_SHOOTING,
        ST_MAX_STATES
    };

    STATE_DECLARE(SystemState, Unhomed, NoEventData);
    STATE_DECLARE(SystemState, Homing, NoEventData);
    STATE_DECLARE(SystemState,  Idle,   NoEventData);
    STATE_DECLARE(SystemState,  WaitForAlarm,   AlarmData);
    EXIT_DECLARE(SystemState, ExitWaitForAlarm);
    STATE_DECLARE(SystemState,  Shooting,  NoEventData);
    GUARD_DECLARE(SystemState, HasShootingData,NoEventData)

    // state map to define state function order
    BEGIN_STATE_MAP_EX
        STATE_MAP_ENTRY_EX(&Unhomed)
        STATE_MAP_ENTRY_EX(&Homing)
        STATE_MAP_ENTRY_EX(&Idle)
        STATE_MAP_ENTRY_ALL_EX(&WaitForAlarm, 0, 0, &ExitWaitForAlarm)
        STATE_MAP_ENTRY_ALL_EX(&Shooting, &HasShootingData, 0,0)
    END_STATE_MAP_EX

};

#endif //MOTIONCONTROLLER_SYSTEMSTATE_H
