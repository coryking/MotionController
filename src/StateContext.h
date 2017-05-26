//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_STATECONTEXT_H
#define MOTIONCONTROLLER_STATECONTEXT_H

#include <functional>
#include "IntervalometerStateMachine.h"

class StateContext;

class AbstractState {
    virtual void run(StateContext & caller) = 0;
};

class StartState : public AbstractState {
    private
    void
};

class StateContext {
public:

    StateContext(IntervalometerSettings *settings) : settings(settings) {

    }

    typedef std::function<bool()> StepperRunningCallbackFn;
    typedef std::function<void(long)> MoveStepperToPositionFn;
    typedef std::function<void()> InvokeShutterCallbackFn;


    const IntervalometerSettings *getSettings() const {
        return settings;
    }

    AbstractState * getCurrentState() const {
        return currentState;
    }

    unsigned int getCurrentFrame() const {
        return currentFrame;
    }

    void setStepperRunningCallback(const StepperRunningCallbackFn &getRunningCallback) {
        IntervalometerStateMachine::getRunningCallback = getRunningCallback;
    }

    void setMoveStepperToRelativeCallback(const MoveStepperToPositionFn &moveStepperToRelativePositionCb) {
        IntervalometerStateMachine::moveStepperToRelativePositionCb = moveStepperToRelativePositionCb;
    }

    void setMoveStepperToAbsolutePositionCb(const MoveStepperToPositionFn &moveStepperToAbsolutePositionCb) {
        IntervalometerStateMachine::moveStepperToAbsolutePositionCb = moveStepperToAbsolutePositionCb;
    }

    void setOpenShutterCb(const InvokeShutterCallbackFn &openShutterCb) {
        IntervalometerStateMachine::openShutterCb = openShutterCb;
    }

    void setCloseShutterCb(const InvokeShutterCallbackFn &closeShutterCb) {
        IntervalometerStateMachine::closeShutterCb = closeShutterCb;
    }

private:
    IntervalometerSettings *settings;

    unsigned int currentFrame;
    // when the shutter started
    unsigned long startMs;

    AbstractState * currentState;

    StepperRunningCallbackFn getRunningCallback;
    MoveStepperToPositionFn moveStepperToRelativePositionCb;
    MoveStepperToPositionFn moveStepperToAbsolutePositionCb;

    InvokeShutterCallbackFn openShutterCb;
    InvokeShutterCallbackFn closeShutterCb;


};


#endif //MOTIONCONTROLLER_STATECONTEXT_H
