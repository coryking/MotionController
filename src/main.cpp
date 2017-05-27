#include <AccelStepper.h>
#include <InputDebounce.h>

#include <IntervalometerSettings.h>
#include <IntervalometerStateMachine.h>

#include "Pins.h"

#include "I2C.h"
#include "rtc.h"

#include "SystemState.h"
#include "Homer.h"

#define BUTTON_DEBOUNCE_DELAY   20   // [ms]

#define TOTAL_SHOTS 300
#define INTERVAL 1000
#define TIME_PER_SHOT 250
#define TIME_FOR_SHUTTER_TRIGGER 200


long sliderDistanceSteps = 0;

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, STEP, DIR); // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

//IntervalometerStateMachine *ivStateMachine = NULL;
//IntervalometerSettings *ivSettings = NULL;

SystemState systemState;
Homer homer;

static InputDebounce limitSwitch; // not enabled yet, setup has to be called later

volatile bool didGetAlarm = false;
bool previousMotorRunState = false;
void handleMotorRunState();

void alarm_callback() {
    didGetAlarm = true;
}

void limitSwitch_pressedCallback() {
    homer.LimitSwitchPressed();
}

void doNothingCallback() {}
void doNothingDurationCallback(unsigned long duration) {}

void setup()
{

    Serial.begin(9600);

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
    digitalWrite(RTC_INTERUPT_PIN, HIGH);
    pinMode(RTC_INTERUPT_PIN, INPUT_PULLUP);
    attachInterrupt(RTC_INTERUPT_PIN, alarm_callback, FALLING);

}

void loop()
{
    unsigned long now = millis();
    limitSwitch.process(now);
    handleMotorRunState();

    if(didGetAlarm) {
        systemState.AlarmFired();
        didGetAlarm = false;
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