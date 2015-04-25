#include "sensors.h"



SensorValues sensor_values = { 50, 6.5, 25, 50};
DHT temperature_humidity_sensor(53, DHT22);

void init_sensors(){

}

SensorValues get_sensor_values(){
    return sensor_values;
}

void measure_pH(){

}

void measure_temperature(){
    sensor_values.temperature = temperature_humidity_sensor.readTemperature();
}

void measure_humidity(){
    sensor_values.humidity = temperature_humidity_sensor.readHumidity();
}

void measure_luminosity(){

}
