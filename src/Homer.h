//
// Created by Cory King on 5/27/17.
//

#ifndef MOTIONCONTROLLER_HOMER_H
#define MOTIONCONTROLLER_HOMER_H

#include <functional>
#include <Arduino.h>
#include <AccelStepper.h>

#include "StateMachine.h"
#include "Pins.h"

class Homer : public StateMachine {
public:
    typedef std::function<void(ulong sliderDistance)> SliderHomedCb;

    Homer() : StateMachine(ST_MAX_STATES) {}

    void StartHoming();
    void LimitSwitchPressed();
    void MotorStopped();

    bool isHoming() {
        return (this->GetCurrentState() != ST_HOMED || this->GetCurrentState() != ST_UNHOMED);
    }

    ulong getSliderDistance() const {
        return _sliderDistance;
    }

    void setStepper(AccelStepper *stepper);

    void setSliderHomedCb(const SliderHomedCb &sliderSetCb);

private:
    ulong _sliderDistance;
    SliderHomedCb sliderSetCb;

    AccelStepper* stepper;

    void invokeSliderHomedCb(ulong sliderDistance) {
        if(sliderSetCb) {
            sliderSetCb(_sliderDistance);
        }
    }

    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States {
        ST_UNHOMED=0,
        ST_FINDING_UPPER,
        ST_FINDING_LOWER,
        ST_MOVING_TO_ZERO,
        ST_HOMED,
        ST_EMERGENCY_STOP,
        ST_MAX_STATES
    };

    STATE_DECLARE(Homer, Unhomed, NoEventData);
    STATE_DECLARE(Homer, FindingUpper, NoEventData);
    STATE_DECLARE(Homer, FindingLower, NoEventData);
    STATE_DECLARE(Homer, MovingToZero, NoEventData);
    STATE_DECLARE(Homer, Homed, NoEventData);
    STATE_DECLARE(Homer, EmergencyStopped, NoEventData);
    GUARD_DECLARE(Homer, HasStepperSet, NoEventData);

    BEGIN_STATE_MAP_EX
        STATE_MAP_ENTRY_EX(&Unhomed)
        STATE_MAP_ENTRY_ALL_EX(&FindingUpper, &HasStepperSet,0,0)
        STATE_MAP_ENTRY_ALL_EX(&FindingLower, &HasStepperSet,0,0)
        STATE_MAP_ENTRY_ALL_EX(&MovingToZero, &HasStepperSet,0,0)
        STATE_MAP_ENTRY_EX(&Homed)
        STATE_MAP_ENTRY_EX(&EmergencyStopped)
    END_STATE_MAP_EX
};


#endif //MOTIONCONTROLLER_HOMER_H
