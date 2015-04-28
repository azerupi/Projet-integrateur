#ifndef _BUTTONS
#define _BUTTONS

#include <LiquidCrystal.h>

#include "Arduino.h"

// Buttons

const char button_1 = 41; // Most left button
const char button_2 = 43;
const char button_3 = 45;
const char button_4 = 47;
const char button_5 = 49;
const char button_6 = 51; // Most right button

void init_buttons(LiquidCrystal *lcd);

void check_button(char button, void (*callback));


#endif
