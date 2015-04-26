#ifndef _HEATING
#define _HEATING

#include "sensors.h"

const char heating = 0; // Has to change to the correct pin when we wire it

extern char target_temperature;
extern bool heating_active;

void init_heating();

void raise_target_temperature();
void lower_target_temperature();

void check_temperature();

#endif
