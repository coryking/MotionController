//
// Created by Cory King on 5/27/17.
//

#include "Homer.h"
#include "display.h"
#include "tasker.h"

void Homer::StartHoming() {
    Serial.println(F("Start Homing"));
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(ST_BEGIN_HOMING) // ST_UNHOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FINDING_UPPER
        TRANSITION_MAP_ENTRY(ST_BEGIN_HOMING) // ST_FINDING_LOWER
        TRANSITION_MAP_ENTRY(ST_BEGIN_HOMING) // ST_MOVING_TO_ZERO
        TRANSITION_MAP_ENTRY(ST_BEGIN_HOMING) // ST_HOMED
        TRANSITION_MAP_ENTRY(ST_BEGIN_HOMING) // ST_EMERGENCY_STOP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BEGIN_HOMING
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WIGGLE_STEPPER
    END_TRANSITION_MAP(NULL)
}

void Homer::LimitSwitchPressed() {
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_UNHOMED
        TRANSITION_MAP_ENTRY(ST_FINDING_LOWER) // ST_FINDING_UPPER
        TRANSITION_MAP_ENTRY(ST_MOVING_TO_ZERO) // ST_FINDING_LOWER
        TRANSITION_MAP_ENTRY(ST_EMERGENCY_STOP) // ST_MOVING_TO_ZERO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_HOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_EMERGENCY_STOP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BEGIN_HOMING
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WIGGLE_STEPPER
    END_TRANSITION_MAP(NULL)

}

void Homer::MotorStopped() {
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_UNHOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FINDING_UPPER
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FINDING_LOWER
        TRANSITION_MAP_ENTRY(ST_HOMED) // ST_MOVING_TO_ZERO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_HOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_EMERGENCY_STOP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BEGIN_HOMING
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_WIGGLE_STEPPER
    END_TRANSITION_MAP(NULL)
}


void Homer::Poll() {
    BEGIN_TRANSITION_MAP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_UNHOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FINDING_UPPER
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FINDING_LOWER
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_MOVING_TO_ZERO
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_HOMED
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_EMERGENCY_STOP
        TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_BEGIN_HOMING
        TRANSITION_MAP_ENTRY(ST_WIGGLE_STEPPER) // ST_WIGGLE_STEPPER
    END_TRANSITION_MAP(NULL)
}


GUARD_DEFINE(Homer, HasStepperSet, NoEventData) {
    return (stepper != NULL);
}

STATE_DEFINE(Homer, Unhomed, NoEventData) {

}

STATE_DEFINE(Homer, BeginHoming, NoEventData)
{
    resetWiggles();


    Serial.println(F("Yo...  gonna start homing!"));
    stepper->setAcceleration(HOMING_MOTOR_ACCEL);
    if(_limitSwitch->isPressed()) {
        Serial.println(F("Switch was pressed already!"));
        stepper->setMaxSpeed(WIGGLE_MOTOR_SPEED);
        InternalEvent(ST_WIGGLE_STEPPER);
    } else {
        stepper->setMaxSpeed(HOMING_MOTOR_SPEED);
        InternalEvent(ST_FINDING_UPPER);
    }
}


STATE_DEFINE(Homer, WiggleStepper, NoEventData) {
    if(_limitSwitch->isPressed()) {

        // if it is running... keep going...
        if(stepper->isRunning())
            return;

        if(timesWiggled > MAX_WIGGLES) {
            Serial.println(F("Wiggled too many times..."));
            InternalEvent(ST_EMERGENCY_STOP);
        } else {
            wiggleDirection *= -1;
            Serial.print(F("Wiggling stepper..."));
            Serial.println(wiggleDirection * wiggleSteps);
            delay(1000);
            yield();
            stepper->moveTo(wiggleDirection * wiggleSteps);
            if(wiggleDirection < 0) {
                wiggleSteps = wiggleSteps * WIGGLE_BUFFER_MULTIPLER;
            }
        }
    } else {
        Serial.print(F("We are free!  Moving a little more... "));
        stepper->moveTo(stepper->currentPosition());
        stepper->runToPosition(); // fucking block that shit....
        yield();
        delay(1000);
        Serial.println(wiggleDirection * BUFFER_STEPS);
        // move out even more just to give some kind of clearance...
        stepper->moveTo(wiggleDirection * 2 * BUFFER_STEPS);
        yield();
        stepper->runToPosition(); // fucking block that shit....
        delay(1000);
        yield();
        _limitSwitch->read();
        delay(50);
        _limitSwitch->read();
        if(_limitSwitch->isPressed()) {
            // fuck....  I think we need to just bail
            InternalEvent(ST_EMERGENCY_STOP);
        } else {
            InternalEvent(ST_BEGIN_HOMING);
        }
    }
}

STATE_DEFINE(Homer, FindingUpper, NoEventData) {
    Serial.println("Uncalibrated.  Gonna calibrate this shit....");
    showHomingScreen();
    this->stepper->moveTo(INFINITE_MOTION);
    this->invokeStartedHomingCb();
}

STATE_DEFINE(Homer, FindingLower, NoEventData) {
    showHomingUpperBound(stepper->currentPosition());
    Serial.print("Found upper bound!!!  Position is: ");
    Serial.print(stepper->currentPosition());
    Serial.println(".  Stopping...");
    stepper->setCurrentPosition(0);
    stepper->moveTo(-INFINITE_MOTION);
    Serial.println("Now looking for lower bound");

}

STATE_DEFINE(Homer, MovingToZero, NoEventData) {
    Serial.print("Found lower bound!!!  Position is: ");
    Serial.print(stepper->currentPosition());
    Serial.println(".  Stopping...");
    Serial.println(stepper->currentPosition());
    _sliderDistance = abs(stepper->currentPosition()) - 2 * BUFFER_STEPS;
    Serial.print("Slider Distance is ");
    Serial.print(_sliderDistance);
    Serial.println(" steps");
    stepper->setCurrentPosition(-BUFFER_STEPS);
    Serial.println("Moving to zero point...");
    stepper->moveTo(0);
    showHomingLowerBound(_sliderDistance);
}

STATE_DEFINE(Homer, Homed, NoEventData) {
    showHomed();
    stepper->setAcceleration(NORMAL_MOTOR_ACCEL);
    stepper->setMaxSpeed(NORMAL_MOTOR_SPEED);
    this->invokeSliderHomedCb(_sliderDistance);
}

STATE_DEFINE(Homer, EmergencyStopped, NoEventData) {
    Serial.println(F("Emergency Stopped!  This is bad!"));
    // no-op for now...
}

void Homer::setSliderHomedCb(const Homer::SliderHomedCb &sliderSetCb) {
    Homer::sliderSetCb = sliderSetCb;
}

void Homer::setStepper(AccelStepper *stepper) {
    Homer::stepper = stepper;
}

void Homer::setStartedHomingCb(const Homer::StateChangedCb &startedHomingCb) {
    Homer::startedHomingCb = startedHomingCb;
}

void Homer::resetWiggles() {
    timesWiggled = 0;
    wiggleDirection = -1;
    wiggleSteps = BUFFER_STEPS;
}