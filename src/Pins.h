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

#define LIMIT_SWITCH_PIN D7 // yellow wire
#define LIMIT_SWITCH_PULLUP true
#define LIMIT_SWITCH_INVERT true
#define DEBOUNCE_MS 20

#define CAMERA_PIN D8 // green wire

#define POT_PIN A0

#define KEYPAD_I2CADDR 0x22
#define LCD_I2CADDR 0x27


#define INFINITE_MOTION 100000
#define BUFFER_STEPS 50 // "padding" for the limit switches...

#define NORMAL_MOTOR_SPEED 1000
#define NORMAL_MOTOR_ACCEL 4000
#define HOMING_MOTOR_SPEED 2000
#define HOMING_MOTOR_ACCEL 2000
#define WIGGLE_MOTOR_SPEED 50


#endif //MOTIONCONTROLLER_PINS_H
