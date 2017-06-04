//
// Created by Cory King on 6/3/17.
//

#include <Arduino.h>
#include "KeyController.h"
#include <Keypad.h>


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0',':','D'}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6, 7}; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad_MC17 keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS, KEYPAD_I2CADDR);


void setupKeypad() {
    keypad.begin( );
}
