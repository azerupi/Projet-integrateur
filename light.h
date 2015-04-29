#ifndef _LIGHT
#define _LIGHT

// To have access to saved sensor readings
#include "time.h"


enum Light_mode{
        AUTO,
        ON,
        OFF
};


const char light = 2; // Change when wired on Arduino
extern Light_mode light_mode;
extern long int auto_time;


void init_light();
void light_cycle();

void change_light_mode(Light_mode mode);

#endif
