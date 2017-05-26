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

    void StartAlarm(AlarmData*);
    void BeginShooting(ShootingData*);
    void Poll(ShootingData*);
private:


    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States {
        ST_IDLE = 0,
        ST_WAIT_FOR_ALARM,
        ST_SHOOTING,
        ST_WAIT_TO_FINISH_SHOOTING,
        ST_MAX_STATES
    };

    STATE_DECLARE(SystemState,  Idle,   NoEventData);
    STATE_DECLARE(SystemState,  WaitForAlarm,   AlarmData);
    EXIT_DECLARE(SystemState, ExitWaitForAlarm);
    STATE_DECLARE(SystemState,  Shooting,  ShootingData);
    STATE_DECLARE(SystemState, WaitToFinishShooting, ShootingData);

    // state map to define state function order
    BEGIN_STATE_MAP_EX
        STATE_MAP_ENTRY_EX(&Idle)
        STATE_MAP_ENTRY_ALL_EX(&WaitForAlarm, 0, 0, &ExitWaitForAlarm)
        STATE_MAP_ENTRY_EX(&Shooting)
        STATE_MAP_ENTRY_EX(&WaitToFinishShooting)
    END_STATE_MAP_EX

};

#endif //MOTIONCONTROLLER_SYSTEMSTATE_H
