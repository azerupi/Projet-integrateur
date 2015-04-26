#include "heating.h"

char target_temperature = 25;
bool heating_active = false;

void init_heating(){
    pinMode(heating, OUTPUT);
}

void raise_target_temperature(){
    target_temperature++;
    check_temperature();
}

void lower_target_temperature(){
    target_temperature--;
    check_temperature();
}

void check_temperature(){

    if(get_sensor_values().temperature < target_temperature && !heating_active){
        digitalWrite(heating, HIGH);
    }
    else if(get_sensor_values().temperature > target_temperature && heating_active){
        digitalWrite(heating, LOW);
    }

}
