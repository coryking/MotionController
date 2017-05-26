//
// Created by Cory King on 5/26/17.
//

#ifndef MOTIONCONTROLLER_I2C_H
#define MOTIONCONTROLLER_I2C_H

#include <Arduino.h>
#include <Wire.h>
#include "Pins.h"


void setupI2C() {
    Wire.begin(SDA_PIN, SCL_PIN);
}

#endif //MOTIONCONTROLLER_I2C_H
