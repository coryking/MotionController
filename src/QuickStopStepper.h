//
// Created by Cory King on 5/20/17.
//

#ifndef MOTIONCONTROLLER_QUICKSTOPSTEPPER_H
#define MOTIONCONTROLLER_QUICKSTOPSTEPPER_H

#include <AccelStepper.h>


class QuickStopStepper : public AccelStepper {
public:
    QuickStopStepper(uint8_t interface = AccelStepper::FULL4WIRE, uint8_t pin1 = 2, uint8_t pin2 = 3, uint8_t pin3 = 4, uint8_t pin4 = 5, bool enable = true) :
            AccelStepper(interface, pin1, pin2, pin3, pin4, enable) {

    }


    void quickStop() {
        if(speed() !=0.0) {
            move(0);
        }
    }
};


#endif //MOTIONCONTROLLER_QUICKSTOPSTEPPER_H
