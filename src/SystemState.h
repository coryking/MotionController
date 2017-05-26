//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_SYSTEMSTATE_H
#define MOTIONCONTROLLER_SYSTEMSTATE_H


#include <Arduino.h>
#include <functional>

#include "EventData.h"
#include "IntervalometerSettings.h"
#include "IntervalometerStateMachine.h"

typedef void (*voidFuncPtr)(void);

typedef std::function<void(void)> fp;

struct AlarmSettings : public EventData {
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
};


enum SystemStates {
    SYSTEM_IDLE,
    SYSTEM_WAIT_FOR_ALARM,
    SYSTEM_SHOOTING
};

class SystemState {
public:
    SystemState() {}

    SystemStates getCurrentState();

    void StartAlarm(AlarmSettings*, voidFuncPtr);
    void AlarmTriggered(IntervalometerSettings, IntervalometerStateMachine*);
    void BeginShooting(IntervalometerSettings, IntervalometerStateMachine*);

private:
    SystemStates currentState;

    void setState(SystemStates newState);

};

#endif //MOTIONCONTROLLER_SYSTEMSTATE_H
