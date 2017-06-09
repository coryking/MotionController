//
// Created by Cory King on 5/20/17.
//

#ifndef MOTIONCONTROLLER_INTERVALOMETERSTATEMACHINE_H
#define MOTIONCONTROLLER_INTERVALOMETERSTATEMACHINE_H

#include <Arduino.h>
#include <functional>

#include "IntervalometerSettings.h"

enum IntervalometerStates {
    IS_IDLE,
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
        setCurrentState(IS_IDLE);

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

    void ICACHE_FLASH_ATTR setStepperRunningCallback(const StepperRunningCallbackFn &getRunningCallback) {
        IntervalometerStateMachine::getRunningCallback = getRunningCallback;
    }

    void ICACHE_FLASH_ATTR  setMoveStepperToRelativeCallback(const MoveStepperToPositionFn &moveStepperToRelativePositionCb) {
        IntervalometerStateMachine::moveStepperToRelativePositionCb = moveStepperToRelativePositionCb;
    }

    void ICACHE_FLASH_ATTR  setMoveStepperToAbsolutePositionCb(const MoveStepperToPositionFn &moveStepperToAbsolutePositionCb) {
        IntervalometerStateMachine::moveStepperToAbsolutePositionCb = moveStepperToAbsolutePositionCb;
    }

    void ICACHE_FLASH_ATTR  setOpenShutterCb(const InvokeShutterCallbackFn &openShutterCb) {
        IntervalometerStateMachine::openShutterCb = openShutterCb;
    }

    void ICACHE_FLASH_ATTR  setCloseShutterCb(const InvokeShutterCallbackFn &closeShutterCb) {
        IntervalometerStateMachine::closeShutterCb = closeShutterCb;
    }

    void ICACHE_FLASH_ATTR  start(IntervalometerSettings *settings) {
        this->settings = settings;
        this->transitionToStart();
    }

    void ICACHE_FLASH_ATTR  run() {
        unsigned long currentTime = millis();
        unsigned long currentDuration = currentTime - startMs;
        bool isStopped = !invokeGetRunningCallback();

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
            case IS_IDLE:
            case FINISHED:
            default:
                break;
        }
    }

protected:
    void ICACHE_FLASH_ATTR  setCurrentState(IntervalometerStates state) {
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

    bool invokeGetRunningCallback() {
        if(getRunningCallback) {
            return getRunningCallback();
        } else {
            return false;
        }
    }

    void ICACHE_FLASH_ATTR  transitionToStart() {
        this->currentFrame = 1;
        this->startMs = millis();
        this->setCurrentState(START);
    }

    void ICACHE_FLASH_ATTR  transitionToPositioning() {
        moveStepperToAbsolutePositionCb(settings->getStartPosition());
        this->setCurrentState(POSITIONING);
    }

    void ICACHE_FLASH_ATTR  transitionToShutter() {
        startMs = millis();
        this->openShutterCb();
        this->setCurrentState(SHUTTER);
    }

    void ICACHE_FLASH_ATTR  transitionToShutterWait() {
        this->closeShutterCb();
        this->setCurrentState(SHUTTER_WAIT);
    }

    void ICACHE_FLASH_ATTR  transitionToShuttle() {
        moveStepperToRelativePositionCb(settings->getRelativeStepsPerFrame());
        this->setCurrentState(SHUTTLE);
    }

    void ICACHE_FLASH_ATTR  transitionToPaused() {
        this->setCurrentState(PAUSED);
    }

    void ICACHE_FLASH_ATTR  transitionToFinished() {
        this->setCurrentState(FINISHED);
    }

};


#endif //MOTIONCONTROLLER_INTERVALOMETERSTATEMACHINE_H
