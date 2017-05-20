#include "QuickStopStepper.h"
#include <Task.h>
#include <InputDebounce.h>

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]


#define DIR D1
#define STEP D2
#define LIMIT_SWITCH D6 // yellow wire
#define CAMERA_PIN D5 // green whire

#define BUFFER_STEPS 10

#define INFINITE_MOTION 100000

enum CalibrationState {
    NEEDS_CALIBRATION,
    FIND_UPPER,
    FOUND_UPPER,
    FIND_LOWER,
    FOUND_LOWER,
    MOVING_TO_ZERO,
    CALIBRATED
};

CalibrationState calibrationState = NEEDS_CALIBRATION;
long sliderDistanceSteps = 0;

// Define a stepper and the pins it will use
QuickStopStepper stepper(AccelStepper::DRIVER, STEP, DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

TaskManager taskManager;

static InputDebounce limitSwitch; // not enabled yet, setup has to be called later

void onEnableMotor(uint32_t deltaTime);
void onDisableMotor(uint32_t deltaTime);

FunctionTask enableMotorTask(onEnableMotor, MsToTaskTime(4000));
FunctionTask disableMotorTask(onDisableMotor, MsToTaskTime(4000));

void limitSwitch_pressedCallback() {
    switch(calibrationState) {
        case FIND_UPPER:
            Serial.print("Found upper bound!!!  Position is: ");
            Serial.print(stepper.currentPosition());
            Serial.println(".  Stopping...");
            stepper.quickStop();
            calibrationState = FOUND_UPPER;
            break;
        case FIND_LOWER:
            Serial.print("Found lower bound!!!  Position is: ");
            Serial.print(stepper.currentPosition());
            Serial.println(".  Stopping...");
            stepper.quickStop();
            calibrationState = FOUND_LOWER;

            break;
        default:
            Serial.println("Oh shit, yo!  We hit an end!!");
            Serial.print("Hit at ");
            Serial.print(stepper.currentPosition());
            Serial.println(" steps ");
            Serial.print(stepper.distanceToGo());
            Serial.println(" steps was remaining....  Gonna stop, fuck this shit....");

            stepper.quickStop();
    }
}

void doNothingCallback() {}
void doNothingDurationCallback(unsigned long duration) {}

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
            // If at the end of travel go to the other end
            if (stepper.distanceToGo() == 0) {
                Serial.print("Currently at: ");
                Serial.print(stepper.currentPosition());
                Serial.print(" moving to: ");

                long newPosition = random(0,sliderDistanceSteps);
                Serial.println(newPosition);
                stepper.moveTo(newPosition);

            }

            break;
        case NEEDS_CALIBRATION:
            Serial.println("Uncalibrated.  Gonna calibrate this shit....");
            stepper.moveTo(INFINITE_MOTION);
            calibrationState = FIND_UPPER;
            break;
        case FOUND_UPPER:
            if(!stepper.isRunning()) {
                Serial.print("Stopped.  Current Position: ");
                Serial.println(stepper.currentPosition());
                stepper.setCurrentPosition(0);
                stepper.moveTo(-INFINITE_MOTION);
                calibrationState = FIND_LOWER;
                Serial.println("Now looking for lower bound");
            }
            break;
        case FOUND_LOWER:
            if(!stepper.isRunning()) {
                Serial.print("Stopped.  Current Position: ");
                Serial.println(stepper.currentPosition());
                sliderDistanceSteps = abs(stepper.currentPosition()) - 2 * BUFFER_STEPS;
                Serial.print("Slider Distance is ");
                Serial.print(sliderDistanceSteps);
                Serial.println(" steps");
                stepper.setCurrentPosition(-BUFFER_STEPS);
                Serial.println("Moving to zero point...");
                stepper.moveTo(0);
                calibrationState = MOVING_TO_ZERO;
            }
            break;
        case MOVING_TO_ZERO:
            if(!stepper.isRunning()) {
                Serial.print("Stopped.  Current Position: ");
                Serial.println(stepper.currentPosition());
                Serial.print("Slider Distance is ");
                Serial.print(sliderDistanceSteps);
                Serial.println(" steps");
                Serial.println("Fully Calibrated and ready for action!");
                calibrationState = CALIBRATED;
            }
            break;
        default:
            break;
    }

    taskManager.Loop();
    stepper.run();
}

void onEnableMotor(uint32_t deltaTime) {
    stepper.enableOutputs();
    taskManager.StartTask(&disableMotorTask);
    taskManager.StopTask(&enableMotorTask);
}


void onDisableMotor(uint32_t deltaTime) {
    stepper.disableOutputs();
    taskManager.StopTask(&disableMotorTask);
    taskManager.StartTask(&enableMotorTask);
}
