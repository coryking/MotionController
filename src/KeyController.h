//
// Created by Cory King on 6/3/17.
//

#ifndef MOTIONCONTROLLER_KEYPAD_H
#define MOTIONCONTROLLER_KEYPAD_H

#include <Keypad_MC17.h>
#include "Pins.h"

#define KEYPAD_COLS 4
#define KEYPAD_ROWS 4

extern Keypad_MC17 keypad;

struct TextLine_S {
    String text;
    uint8_t position;
    ulong lastmod;
};

typedef struct TextLine_S TextLine;

void setupKeypad();

#endif //MOTIONCONTROLLER_KEYPAD_H
