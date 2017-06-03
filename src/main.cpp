#include <AccelStepper.h>
#include <InputDebounce.h>

#include <IntervalometerSettings.h>
#include <IntervalometerStateMachine.h>

#include "Pins.h"

#include "I2C.h"
#include "rtc.h"
#include "KeyController.h"

#include "SystemState.h"
#include "Homer.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

#define TOTAL_SHOTS 2
#define INTERVAL 1000
#define TIME_PER_SHOT 250
#define TIME_FOR_SHUTTER_TRIGGER 200


long sliderDistanceSteps = 0;

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

//IntervalometerStateMachine *ivStateMachine = NULL;
//IntervalometerSettings *ivSettings = NULL;

SystemState systemState;
Homer homer;

static InputDebounce limitSwitch; // not enabled yet, setup has to be called later

bool previousMotorRunState = false;
void handleMotorRunState();

void limitSwitch_pressedCallback() {
    homer.LimitSwitchPressed();
}

void doNothingCallback() {}
void doNothingDurationCallback(unsigned long duration) {}

void setup()
{

    Serial.begin(9600);
    Serial.println("Hello World");
    randomSeed(analogRead(0));
    pinMode(CAMERA_PIN, OUTPUT);
    digitalWrite(CAMERA_PIN, LOW);

    setupI2C();
    setupRtc(&Serial);

    limitSwitch.registerCallbacks(limitSwitch_pressedCallback, doNothingCallback, doNothingDurationCallback);
    limitSwitch.setup(LIMIT_SWITCH, BUTTON_DEBOUNCE_DELAY, InputDebounce::PIM_INT_PULL_UP_RES);


    // Change these to suit your stepper if you want
    //stepper.setEnablePin(D8);
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(4000);

    systemState.setStepper(&stepper);
    homer.setStepper(&stepper);
    homer.setSliderHomedCb([](ulong sliderDistance) {
        systemState.HomingComplete();
        sliderDistanceSteps = sliderDistance;
        Serial.println("Slider Has been homed according to our callback!");

        IntervalometerSettings *ivSettings = new IntervalometerSettings(TOTAL_SHOTS, 0, sliderDistanceSteps, INTERVAL, TIME_PER_SHOT,TIME_FOR_SHUTTER_TRIGGER);
        systemState.setSettings(ivSettings);

        auto alarmData = new AlarmData();
        auto rtcDate = globalRtc.GetDateTime();
        rtcDate+=10;
        alarmData->day=rtcDate.Day();
        alarmData->hour=rtcDate.Hour();
        alarmData->minute=rtcDate.Minute();
        alarmData->second=rtcDate.Second() % 30;
        systemState.StartAlarm(alarmData);
    });

    homer.StartHoming();
    systemState.HomeSlider();
}

void loop()
{
    unsigned long now = millis();
    limitSwitch.process(now);
    handleMotorRunState();

    if(Alarmed()) {
        systemState.AlarmFired();
    }

    if(!homer.isEmergencyStopped()) {
        stepper.run();
        systemState.run();
    }

}

void handleMotorRunState() {
    if(previousMotorRunState != stepper.isRunning()) {
        if(stepper.isRunning()) {
            // signal that the stepper has started to move
        } else {
            // signal that the stepper has stopped
            homer.MotorStopped();
        }
    }
    previousMotorRunState = stepper.isRunning();
}