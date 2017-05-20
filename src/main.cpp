#include <AccelStepper.h>
#include <InputDebounce.h>

#include <IntervalometerSettings.h>
#include <IntervalometerStateMachine.h>


#define BUTTON_DEBOUNCE_DELAY   20   // [ms]


#define DIR D1
#define STEP D2
#define LIMIT_SWITCH D6 // yellow wire
#define CAMERA_PIN D5 // green whire

#define BUFFER_STEPS 40 // "padding" for the limit switches...

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


static InputDebounce limitSwitch; // not enabled yet, setup has to be called later

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
    settings = new IntervalometerSettings(10, 0, sliderDistanceSteps, 2000, 500,100);
    stateMachine = new IntervalometerStateMachine(settings);

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

    randomSeed(analogRead(0));
    Serial.begin(9600);

    // Change these to suit your stepper if you want
    stepper.setEnablePin(D8);
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(400);
    //stepper.moveTo(5500);

    limitSwitch.registerCallbacks(limitSwitch_pressedCallback, doNothingCallback, doNothingDurationCallback);
    limitSwitch.setup(LIMIT_SWITCH, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);

    //taskManager.StartTask(&enableMotorTask);
    //taskManager.StartTask(&disableMotorTask);


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
                stateMachine->start();
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
