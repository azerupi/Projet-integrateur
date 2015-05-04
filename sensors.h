#ifndef _SENSORS
#define _SENSORS

#include <DHT.h>
#include "time.h"
#include "lcd.h"
#include "buttons.h"

struct SensorValues{
    float pH;
    float temperature;
    float humidity;
    Time last_ph_update;
};

// Sensor pins
//*
const char pH_sensor = A15;       // Analog pin

extern float offset;

// http://playground.arduino.cc/Main/DHTLib
extern DHT temperature_humidity_sensor; // Pin - DHT type


void init_sensors();

SensorValues get_sensor_values();

void measure_pH();
float pH();
void measure_temperature();
void measure_humidity();

void calibrate_pH();

void pH_menu();


#endif
