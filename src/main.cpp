#include <AccelStepper.h>
#include <Button.h>

#include <IntervalometerSettings.h>
#include <IntervalometerStateMachine.h>

#include <Task.h>
#include "tasker.h"

#include "Pins.h"

#include "I2C.h"
#include "rtc.h"
#include "KeyController.h"
#include "display.h"

#include "SystemState.h"
#include "Homer.h"


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

Button limitSwitch(LIMIT_SWITCH_PIN, LIMIT_SWITCH_PULLUP, LIMIT_SWITCH_INVERT, DEBOUNCE_MS);

bool previousMotorRunState = false;
void handleMotorRunState();

void keypadEvent(KeypadEvent key);

TextData *getTextData();

TextLine tl;
ulong last_display;


void doNothingCallback() {}
void doNothingDurationCallback(unsigned long duration) {}



FunctionTask taskHandleLED(OnHandleLedTask, MsToTaskTime(50));

void OnCheckAlarmState(uint32_t deltaTime) {
    if(Alarmed()) {
        systemState.AlarmFired();
    }
}

FunctionTask taskCheckAlarmState(OnCheckAlarmState, MsToTaskTime(500));

void OnShowEstimatedDuration(uint32_t deltaTime)
{
    if(systemState.isShooting()) {
        auto estimatedDuration = systemState.getEstimatedDurationForTimelapse();
        showTimelapseTimeRemaining(estimatedDuration);
    }
}
FunctionTask taskShowEstimatedDuration(OnShowEstimatedDuration, MsToTaskTime(250)); // turn on the led in 400ms

void setup()
{
    Serial.begin(9600);
    setupI2C();
    setupLcd();

    // read limit switch...
    Serial.println(limitSwitch.read());
    delay(200);
    yield();
    Serial.println(limitSwitch.read());
    Serial.println("Hello World");
    randomSeed(analogRead(0));
    pinMode(CAMERA_PIN, OUTPUT);
    digitalWrite(CAMERA_PIN, LOW);
    pinMode(RTC_INTERUPT_PIN, INPUT_PULLUP);

    setupRtc(&Serial);
    setupKeypad();
    keypad.addEventListener(keypadEvent); // Add an event listener for this keypad


    // Change these to suit your stepper if you want
    stepper.setEnablePin(SLEEP_PIN);
    stepper.setMaxSpeed(1000);
    stepper.setAcceleration(4000);

    systemState.setStepper(&stepper);
    systemState.setKeyboardActiveCb([](){
        tl.text = "";
        tl.position=0;
        tl.lastmod=millis();
        lcd.blink();
    });
    systemState.setKeyboardInctiveCb([](){
        lcd.noBlink();
    });
    systemState.setKeyboardBufferCb([](TextLine textLine) {
        tl = textLine;
    });
    homer.setLimitSwitch(&limitSwitch);
    homer.setStepper(&stepper);
    homer.setSliderHomedCb([](ulong sliderDistance) {
        auto hd = new HomingData();
        hd->sliderSteps = sliderDistance;
        systemState.HomingComplete(hd);
/*        sliderDistanceSteps = sliderDistance;
        Serial.println("Slider Has been homed according to our callback!");

        auto alarmData = new AlarmData();
        auto rtcDate = globalRtc.GetDateTime();
        rtcDate+=10;
        alarmData->day=rtcDate.Day();
        alarmData->hour=rtcDate.Hour();
        alarmData->minute=rtcDate.Minute();
        alarmData->second=rtcDate.Second() % 30;
        systemState.StartAlarm(alarmData); */
    });

    taskManager.StartTask(&taskHandleLED);
    taskManager.StartTask(&theLcdState);
    taskManager.StartTask(&taskShowEstimatedDuration);
    taskManager.StartTask(&taskCheckAlarmState);

    homer.StartHoming();
    systemState.HomeSlider();
}

void loop()
{

    limitSwitch.read();
    if(limitSwitch.wasPressed()) {
        homer.LimitSwitchPressed();
    }
    homer.Poll();
    handleMotorRunState();

    if(!homer.isEmergencyStopped()) {
        stepper.run();
        systemState.Run();
    }

    if(systemState.isKeyboardActive() &&  tl.lastmod >= last_display) {
        lcd.setCursor(0,1);
        String txt = fillString(tl.text, 20);
        lcd.print(txt);
        lcd.setCursor(tl.position+1, 1);
        last_display = millis();
    }

    taskManager.Loop();
    keypad.getKey();
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

void saveKeypadData() {
    TextData *data = getTextData();
    systemState.SaveData(data);
}

TextData *getTextData() {
    auto str = tl.text;
    str.trim();
    auto data = new TextData;
    data->text = str;
    return data;
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key){
    KeyState state = keypad.getState();

    switch (state){
        case PRESSED:

            if(theLcdState.markLastKeypress(millis()))
                return;

            Serial.println(key);
            if(systemState.isKeyboardActive()) {
                switch(key) {
                    case 'D':
                        if (tl.text.length() > 0) {
                            tl.text.remove(tl.text.length() - 1);
                            tl.position = tl.text.length() - 1;
                            tl.lastmod = millis();
                        }
                        break;
                    case 'A':
                        saveKeypadData();
                        break;
                    case 'B':
                        saveKeypadData();
                        systemState.BeginShooting();
                        break;
                    case 'C':
                        systemState.Back();
                        break;
                    default:
                        tl.text = tl.text + key;
                        tl.position = tl.text.length() - 1;
                        tl.lastmod = millis();

                        if(systemState.showEstimatedDuration()) {
                            auto estDuration = systemState.getEstimatedDuration(getTextData());
                            showEstimatedDuration(estDuration);
                        }
                        break;
                }

            } else if (systemState.GetCurrentState() == SystemState::ST_IDLE) {
                switch(key) {
                    case 'A':
                        systemState.ShowIntervalSettings();
                        break;
                    case 'B':
                        systemState.ShowAlarmSettings();
                        break;
                    case 'C':
                        systemState.ShowTimeSettings();
                        break;
                    case 'D':
                        homer.StartHoming();
                        systemState.HomeSlider();
                        break;
                    default:
                        break;
                }
            } else if (systemState.GetCurrentState() == SystemState::ST_ASK_START_SHOOTING) {
                if(key == '1')
                    systemState.BeginShooting();
                else
                    systemState.GoIdle();
            } else if (systemState.GetCurrentState() == SystemState::ST_ASK_START_ALARM) {
                if(key == '1')
                    systemState.StartAlarm();
                else
                    systemState.GoIdle();
            } else {
                if(key=='C')
                    systemState.GoIdle();
            }
            break;
        default:
            break;
    }
}