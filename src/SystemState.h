//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_SYSTEMSTATE_H
#define MOTIONCONTROLLER_SYSTEMSTATE_H


#include <Arduino.h>

#include "StateMachine.h"
#include "IntervalometerSettings.h"
#include "IntervalometerStateMachine.h"


typedef void (*voidFuncPtr)(void);

struct AlarmData : public EventData {
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    voidFuncPtr interruptServiceCallback;
};

struct ShootingData : public EventData {
    IntervalometerSettings *settings;
    IntervalometerStateMachine *stateMachine;
};


class SystemState : public StateMachine {
public:
    SystemState() : StateMachine(ST_MAX_STATES) {}

    void HomeSlider();
    void HomingComplete();
    void StartAlarm(AlarmData*);
    void BeginShooting(ShootingData*);
    void ShootingComplete();
private:


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
    STATE_DECLARE(SystemState,  Shooting,  ShootingData);

    // state map to define state function order
    BEGIN_STATE_MAP_EX
        STATE_MAP_ENTRY_EX(&Unhomed)
        STATE_MAP_ENTRY_EX(&Homing)
        STATE_MAP_ENTRY_EX(&Idle)
        STATE_MAP_ENTRY_ALL_EX(&WaitForAlarm, 0, 0, &ExitWaitForAlarm)
        STATE_MAP_ENTRY_EX(&Shooting)
    END_STATE_MAP_EX

};

#endif //MOTIONCONTROLLER_SYSTEMSTATE_H
