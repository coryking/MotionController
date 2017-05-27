//
// Created by Cory King on 5/20/17.
//

#ifndef MOTIONCONTROLLER_INTERVALOMETERSTATEMACHINE_H
#define MOTIONCONTROLLER_INTERVALOMETERSTATEMACHINE_H

#include <Arduino.h>
#include <functional>

#include "IntervalometerSettings.h"

enum IntervalometerStates {
    IDLE,
    START,
    POSITIONING,
    SHUTTER,
    SHUTTER_WAIT,
    SHUTTLE,
    PAUSED,
    FINISHED
};

class IntervalometerStateMachine {
public:
    typedef std::function<bool()> StepperRunningCallbackFn;
    typedef std::function<void(long)> MoveStepperToPositionFn;
    typedef std::function<void()> InvokeShutterCallbackFn;

    IntervalometerStateMachine() {
        setCurrentState(IDLE);

    }

    const IntervalometerSettings *getSettings() const {
        return settings;
    }

    IntervalometerStates getCurrentState() const {
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

    void start(IntervalometerSettings *settings) {
        this->settings = settings;
        this->transitionToStart();
    }

    void run() {
        unsigned long currentTime = millis();
        unsigned long currentDuration = currentTime - startMs;
        bool isStopped = !getRunningCallback();

        switch(currentState) {
            case START:
                this->transitionToPositioning();
                break;
            case POSITIONING:
                if(isStopped) {
                    this->transitionToShutter();
                }
                break;
            case SHUTTER:
                if(currentDuration >= this->settings->getShutterTriggerDurationMs()) {
                    this->transitionToShutterWait();
                }
                break;
            case SHUTTER_WAIT:
                if(currentDuration >= this->settings->getShutterSpeedMs()) {
                    this->transitionToShuttle();
                }
                break;
            case SHUTTLE:
                if(isStopped) {
                    this->transitionToPaused();
                }
                break;
            case PAUSED:
                if(currentDuration >= this->settings->getIntervalMs()) {
                    Serial.print("Moving out of paused... Requested duration was ");
                    Serial.print(this->settings->getIntervalMs());
                    Serial.print(" actual duration was ");
                    Serial.println(currentDuration);
                    this->currentFrame++;

                    if(this->currentFrame >= this->settings->getTotalFrames()) {
                        this->transitionToFinished();
                    } else {
                        this->transitionToShutter();
                    }
                }
                break;
            case IDLE:
            case FINISHED:
            default:
                break;
        }
    }

protected:
    void setCurrentState(IntervalometerStates state) {
        Serial.print("Going to state: ");
        Serial.print(state);
        if(this->settings != NULL) {
            Serial.print(" --  ");
            Serial.print(this->settings->getTotalFrames() - currentFrame);
            Serial.println(" shots remaining");
        }
        currentState = state;
    }

private:
    IntervalometerSettings *settings = NULL;

    IntervalometerStates currentState;
    // when the shutter started
    unsigned long startMs;

    unsigned int currentFrame;

    StepperRunningCallbackFn getRunningCallback;
    MoveStepperToPositionFn moveStepperToRelativePositionCb;
    MoveStepperToPositionFn moveStepperToAbsolutePositionCb;

    InvokeShutterCallbackFn openShutterCb;
    InvokeShutterCallbackFn closeShutterCb;

    void transitionToStart() {
        this->currentFrame = 1;
        this->startMs = millis();
        this->setCurrentState(START);
    }

    void transitionToPositioning() {
        moveStepperToAbsolutePositionCb(settings->getStartPosition());
        this->setCurrentState(POSITIONING);
    }

    void transitionToShutter() {
        startMs = millis();
        this->openShutterCb();
        this->setCurrentState(SHUTTER);
    }

    void transitionToShutterWait() {
        this->closeShutterCb();
        this->setCurrentState(SHUTTER_WAIT);
    }

    void transitionToShuttle() {
        moveStepperToRelativePositionCb(settings->getRelativeStepsPerFrame());
        this->setCurrentState(SHUTTLE);
    }

    void transitionToPaused() {
        this->setCurrentState(PAUSED);
    }

    void transitionToFinished() {
        this->setCurrentState(FINISHED);
    }

};


#endif //MOTIONCONTROLLER_INTERVALOMETERSTATEMACHINE_H
