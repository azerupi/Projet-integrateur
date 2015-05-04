#include "sensors.h"

SensorValues sensor_values = { 0, 0, 0, 0, 0};
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

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("Measuring pH...");

    delay(5000);

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


void calibrate_pH(){

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("Calibrating pH...");

    delay(5000);

}


void pH_menu(){
    LiquidCrystal *lcd = get_lcd();

    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print("(1) to Measure");
    lcd->setCursor(0,1);
    lcd->print("(2) to Calibrate");

    while(!check_button(button_1, measure_pH) && !check_button(button_2, calibrate_pH)){
        // Wait
    }


}
