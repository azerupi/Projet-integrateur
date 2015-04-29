#include "light.h"


Light_mode light_mode = AUTO;

long int auto_time = 0;


Time wake_time = {6,0,0};   // 6:00 am
Time sleep_time = {22,0,0}; // 10:00 pm


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

        bool on = true;

        if(now.hour <= wake_time.hour){
            on = false;
            if(now.hour == wake_time.hour && now.minutes >= wake_time.minutes){
                on = true;
            }
        }
        else if(now.hour >= sleep_time.hour){
            on = false;
            if(now.hour == sleep_time.hour && now.minutes < sleep_time.minutes){
                on = true;
            }
        }

        digitalWrite(light, on? HIGH : LOW);

        auto_time = millis();
    }

}



void change_light_mode(Light_mode mode){

}
