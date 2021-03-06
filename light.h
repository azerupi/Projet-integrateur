#ifndef _LIGHT
#define _LIGHT

#include "buttons.h"
#include "lcd.h"
// To have access to saved sensor readings
#include "time.h"


enum Light_mode{
        AUTO,
        ON,
        OFF
};


const char light = 38;
extern Light_mode light_mode;
extern long int auto_time;

extern Time wake_time;
extern Time sleep_time;


void init_light();
void light_cycle();

void change_light_mode();

#endif
