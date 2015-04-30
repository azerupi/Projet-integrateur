#ifndef _SENSORS
#define _SENSORS

#include <DHT.h>
#include "time.h"

struct SensorValues{
    float luminosity;
    float pH;
    float temperature;
    float humidity;
    Time last_ph_update;
};

// Sensor pins
//*
const char photoresistor = A0;   // Analog pin
const char pH_sensor = A2;       // Analog pin

// http://playground.arduino.cc/Main/DHTLib
extern DHT temperature_humidity_sensor; // Pin - DHT type


void init_sensors();

SensorValues get_sensor_values();

void measure_pH();
void measure_temperature();
void measure_humidity();
void measure_luminosity();


#endif
