//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_PINS_H
#define MOTIONCONTROLLER_PINS_H

#define DIR_PIN D0
#define STEP_PIN D1
#define SLEEP_PIN D2

#define RTC_INTERUPT_PIN D3

#define LCD_PWM_PIN D4

#define SDA_PIN D5
#define SCL_PIN D6

#define LIMIT_SWITCH D7 // yellow wire
#define CAMERA_PIN D8 // green wire

#define POT_PIN A0


#define INFINITE_MOTION 100000
#define BUFFER_STEPS 40 // "padding" for the limit switches...


#endif //MOTIONCONTROLLER_PINS_H
