#ifndef _LCD
#define _LCD

#include <LiquidCrystal.h>

// To have access to saved sensor readings
#include "sensors.h"
#include "time.h"


// http://arduino.cc/en/Tutorial/LiquidCrystal
extern LiquidCrystal lcd;

enum lcd_view { BLANK, TEMPERATURE, HUMIDITY, PH, TIME };
extern lcd_view current_view;

void init_lcd();

LiquidCrystal* get_lcd();

void show_view(lcd_view view);
void next_view();

#endif
