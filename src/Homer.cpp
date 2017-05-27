//
// Created by Cory King on 5/27/17.
//

#include "Homer.h"

void Homer::StartHoming() {
    BEGIN_TRANSITION_MAP
                    TRANSITION_MAP_ENTRY(ST_FINDING_UPPER) // ST_UNHOMED
                    TRANSITION_MAP_ENTRY(EVENT_IGNORED) // ST_FINDING_UPPER
                    TRANSITION_MAP_ENTRY(ST_FINDING_UPPER) // ST_FINDING_LOWER
                    TRANSITION_MAP_ENTRY(ST_FINDING_UPPER) // ST_MOVING_TO_ZERO
                    TRANSITION_MAP_ENTRY(ST_FINDING_UPPER) // ST_HOMED
                    TRANSITION_MAP_ENTRY(ST_FINDING_UPPER) // ST_EMERGENCY_STOP
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
    END_TRANSITION_MAP(NULL)
}

GUARD_DEFINE(Homer, HasStepperSet, NoEventData) {
    return (stepper != NULL);
}

STATE_DEFINE(Homer, Unhomed, NoEventData) {
    // noop
}

STATE_DEFINE(Homer, FindingUpper, NoEventData) {
    Serial.println("Uncalibrated.  Gonna calibrate this shit....");
    this->stepper->moveTo(INFINITE_MOTION);
}

STATE_DEFINE(Homer, FindingLower, NoEventData) {
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
}

STATE_DEFINE(Homer, Homed, NoEventData) {
    this->invokeSliderHomedCb(_sliderDistance);
}

void Homer::setSliderHomedCb(const Homer::SliderHomedCb &sliderSetCb) {
    Homer::sliderSetCb = sliderSetCb;
}

void Homer::setStepper(AccelStepper *stepper) {
    Homer::stepper = stepper;
}
