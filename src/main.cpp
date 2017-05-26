#include <AccelStepper.h>
#include <InputDebounce.h>

#include <IntervalometerSettings.h>
#include <IntervalometerStateMachine.h>

#include "Pins.h"

#include "I2C.h"
#include "rtc.h"

#include "SystemState.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

#define BUFFER_STEPS 40 // "padding" for the limit switches...

#define TOTAL_SHOTS 300
#define INTERVAL 1000
#define TIME_PER_SHOT 250
#define TIME_FOR_SHUTTER_TRIGGER 200

#define INFINITE_MOTION 100000

enum CalibrationState {
    NEEDS_CALIBRATION,
    FIND_UPPER,
    FIND_LOWER,
    MOVING_TO_ZERO,
    CALIBRATED,
    EMERGENCY_STOP
};

CalibrationState calibrationState = NEEDS_CALIBRATION;
long sliderDistanceSteps = 0;

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

IntervalometerStateMachine *stateMachine = NULL;
IntervalometerSettings *settings = NULL;

SystemState systemState();

static InputDebounce limitSwitch; // not enabled yet, setup has to be called later

volatile bool didGetAlarm = false;

void alarm_callback() {
    didGetAlarm = true;
}

void limitSwitch_pressedCallback() {
    switch(calibrationState) {
        case FIND_UPPER:
            Serial.print("Found upper bound!!!  Position is: ");
            Serial.print(stepper.currentPosition());
            Serial.println(".  Stopping...");
            stepper.setCurrentPosition(0);
            stepper.moveTo(-INFINITE_MOTION);
            calibrationState = FIND_LOWER;
            Serial.println("Now looking for lower bound");

            break;
        case FIND_LOWER:
            Serial.print("Found lower bound!!!  Position is: ");
            Serial.print(stepper.currentPosition());
            Serial.println(".  Stopping...");
            Serial.println(stepper.currentPosition());
            sliderDistanceSteps = abs(stepper.currentPosition()) - 2 * BUFFER_STEPS;
            Serial.print("Slider Distance is ");
            Serial.print(sliderDistanceSteps);
            Serial.println(" steps");
            stepper.setCurrentPosition(-BUFFER_STEPS);
            Serial.println("Moving to zero point...");
            stepper.moveTo(0);
            calibrationState = MOVING_TO_ZERO;

            break;
        default:
            Serial.println("Oh shit, yo!  We hit an end!!");
            Serial.print("Hit at ");
            Serial.print(stepper.currentPosition());
            Serial.println(" steps ");
            Serial.print(stepper.distanceToGo());
            Serial.println(" steps was remaining....  Gonna stop, fuck this shit....");
            calibrationState = EMERGENCY_STOP;
    }
}

void doNothingCallback() {}
void doNothingDurationCallback(unsigned long duration) {}

void setupIntervalometer() {
    settings = new IntervalometerSettings(TOTAL_SHOTS, 0, sliderDistanceSteps, INTERVAL, TIME_PER_SHOT,TIME_FOR_SHUTTER_TRIGGER);
    stateMachine = new IntervalometerStateMachine();

    stateMachine->setCloseShutterCb([](){
        Serial.println("Setting camera pin low!");
        digitalWrite(CAMERA_PIN, LOW);
    });

    stateMachine->setOpenShutterCb([](){
        Serial.println("Setting camera pin high!");
        digitalWrite(CAMERA_PIN, HIGH);
    });

    stateMachine->setMoveStepperToRelativeCallback([](long moveTo){
        stepper.move(moveTo);
    });

    stateMachine->setMoveStepperToAbsolutePositionCb([](long moveTo){
        stepper.moveTo(moveTo);
    });

    stateMachine->setStepperRunningCallback([]() {
        return stepper.isRunning();
    });
}

void setup()
{

    Serial.begin(9600);

    randomSeed(analogRead(0));
    pinMode(CAMERA_PIN, OUTPUT);
    digitalWrite(CAMERA_PIN, LOW);

    setupI2C();
    setupRtc(&Serial);

    // Change these to suit your stepper if you want
    //stepper.setEnablePin(D8);
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(4000);

    limitSwitch.registerCallbacks(limitSwitch_pressedCallback, doNothingCallback, doNothingDurationCallback);
    limitSwitch.setup(LIMIT_SWITCH, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);


}

void loop()
{
    unsigned long now = millis();
    limitSwitch.process(now);

    switch(calibrationState) {
        case CALIBRATED:
            break;
        case NEEDS_CALIBRATION:
            Serial.println("Uncalibrated.  Gonna calibrate this shit....");
            stepper.moveTo(INFINITE_MOTION);
            calibrationState = FIND_UPPER;
            break;
        case MOVING_TO_ZERO:
            if(!stepper.isRunning()) {
                Serial.print("Stopped.  Current Position: ");
                Serial.println(stepper.currentPosition());
                Serial.print("Slider Distance is ");
                Serial.print(sliderDistanceSteps);
                Serial.println(" steps");
                Serial.println("Fully Calibrated and ready for action!");
                setupIntervalometer();
                calibrationState = CALIBRATED;
                stateMachine->start(settings);
            }
            break;
        default:
            break;
    }

    if(calibrationState != EMERGENCY_STOP) {
        stepper.run();
        if(stateMachine)
            stateMachine->run();
    }

}
