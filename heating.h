#ifndef _HEATING
#define _HEATING

#include "lcd.h"
#include "sensors.h"
#include "buttons.h"

const char heating = 0; // Has to change to the correct pin when we wire it

extern char target_temperature;
extern bool heating_active;
extern long int heating_time;

void init_heating();

void change_temperature_target();
void raise_target_temperature();
void lower_target_temperature();

void check_temperature();

#endif
