#include "sensors.h"



SensorValues sensor_values = { NULL, NULL, NULL, NULL, NULL};
DHT temperature_humidity_sensor(53, DHT22);

void init_sensors(){
    measure_temperature();
    measure_humidity();
    measure_pH();
}

SensorValues get_sensor_values(){
    return sensor_values;
}

void measure_pH(){

    // Code

    // Update measure time

    sensor_values.last_ph_update = get_time_now();

}

void measure_temperature(){
    sensor_values.temperature = temperature_humidity_sensor.readTemperature();
}

void measure_humidity(){
    sensor_values.humidity = temperature_humidity_sensor.readHumidity();
}

void measure_luminosity(){

}
