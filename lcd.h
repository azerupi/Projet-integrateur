#ifndef _LCD
#define _LCD

#include <LiquidCrystal.h>

// To have access to saved sensor readings
#include "sensors.h"

enum lcd_view { BLANK, TEMPERATURE, HUMIDITY, PH };

extern lcd_view current_view;

void show_view(LiquidCrystal *lcd, lcd_view view);
void next_view(LiquidCrystal *lcd);

#endif
