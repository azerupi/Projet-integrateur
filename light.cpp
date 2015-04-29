#include "light.h"


Light_mode light_mode = AUTO;

long int auto_time = 0;



void init_light(){
    pinMode(light, OUTPUT);
    digitalWrite(light, LOW);
}



void light_cycle(){

    if(light_mode == OFF){
        return;
    }
    else if(light_mode == ON){
        digitalWrite(light, HIGH);
        return;
    }

    // If the function has not returned yet, it means that the lights are in AUTO mode

    // Check only every minute...

    if(millis() - auto_time > 60000){

        Time now = get_time_now();
        if(now.hour < 6 || now.hour >= 22){
            digitalWrite(light, LOW);
        }
        else{
            digitalWrite(light, HIGH);
        }

        auto_time = millis();
    }

}



void change_light_mode(Light_mode mode){

}
