//
// Created by Cory King on 5/27/17.
//

#ifndef MOTIONCONTROLLER_HOMER_H
#define MOTIONCONTROLLER_HOMER_H

#include <functional>
#include <Arduino.h>
#include <AccelStepper.h>
#include <Button.h>

#include "StateMachine.h"
#include "Pins.h"

// max number of times we wiggle before bailing out...
#define MAX_WIGGLES 6
#define WIGGLE_BUFFER_MULTIPLER 1.1

class Homer : public StateMachine {
public:
    typedef std::function<void(ulong sliderDistance)> SliderHomedCb;
    typedef std::function<void()> StateChangedCb;

    Homer() : StateMachine(ST_MAX_STATES) {
        resetWiggles();
    }

    void StartHoming();
    void LimitSwitchPressed();
    void MotorStopped();
    void Poll();

    bool isHoming() {
        return (this->GetCurrentState() != ST_HOMED || this->GetCurrentState() != ST_UNHOMED);
    }

    bool isEmergencyStopped() {
        return (this->GetCurrentState() == ST_EMERGENCY_STOP);
    }

    ulong getSliderDistance() const {
        return _sliderDistance;
    }

    void setLimitSwitch(Button *limitSwitch) {
        this->_limitSwitch = limitSwitch;
    }

    void setStepper(AccelStepper *stepper);

    void setSliderHomedCb(const SliderHomedCb &sliderSetCb);

    void setStartedHomingCb(const StateChangedCb &startedHomingCb);

private:
    ulong _sliderDistance;
    SliderHomedCb sliderSetCb;
    StateChangedCb startedHomingCb;
    Button* _limitSwitch;
    uint timesWiggled;
    int wiggleDirection = -1;
    int wiggleSteps = BUFFER_STEPS;

    AccelStepper* stepper;

    // state enumeration order must match the order of state
    // method entries in the state map
    enum E_States {
        ST_UNHOMED=0,
        ST_FINDING_UPPER,
        ST_FINDING_LOWER,
        ST_MOVING_TO_ZERO,
        ST_HOMED,
        ST_EMERGENCY_STOP,
        ST_BEGIN_HOMING,
        ST_WIGGLE_STEPPER,
        ST_MAX_STATES
    };

    STATE_DECLARE(Homer, Unhomed, NoEventData);
    STATE_DECLARE(Homer, FindingUpper, NoEventData);
    STATE_DECLARE(Homer, FindingLower, NoEventData);
    STATE_DECLARE(Homer, MovingToZero, NoEventData);
    STATE_DECLARE(Homer, Homed, NoEventData);
    STATE_DECLARE(Homer, EmergencyStopped, NoEventData);
    STATE_DECLARE(Homer, BeginHoming, NoEventData);
    STATE_DECLARE(Homer, WiggleStepper, NoEventData);
    GUARD_DECLARE(Homer, HasStepperSet, NoEventData);


    BEGIN_STATE_MAP_EX
        STATE_MAP_ENTRY_EX(&Unhomed)
        STATE_MAP_ENTRY_ALL_EX(&FindingUpper, &HasStepperSet,0,0)
        STATE_MAP_ENTRY_ALL_EX(&FindingLower, &HasStepperSet,0,0)
        STATE_MAP_ENTRY_ALL_EX(&MovingToZero, &HasStepperSet,0,0)
        STATE_MAP_ENTRY_EX(&Homed)
        STATE_MAP_ENTRY_EX(&EmergencyStopped)
        STATE_MAP_ENTRY_EX(&BeginHoming)
        STATE_MAP_ENTRY_EX(&WiggleStepper)
    END_STATE_MAP_EX

    void invokeSliderHomedCb(ulong sliderDistance) {
        if(sliderSetCb) {
            sliderSetCb(_sliderDistance);
        }
    }

    void invokeStartedHomingCb() {
        if(startedHomingCb) {
            startedHomingCb();
        }
    }

    void resetWiggles();
};


#endif //MOTIONCONTROLLER_HOMER_H
