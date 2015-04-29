#include "light.h"


Light_mode light_mode = AUTO;

long int auto_time = -60000; // - 60 000 so that it updates auto mode on start


Time wake_time = {6,0,0};   // 6:00 am
Time sleep_time = {22,0,0}; // 10:00 pm


void init_light(){

    pinMode(light, OUTPUT);

    digitalWrite(light, LOW);

}



void light_cycle(){

    if(light_mode == OFF){
        digitalWrite(light, LOW);
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



void change_light_mode(){

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("Light mode");
    get_lcd()->setCursor(11,1);

    if(light_mode == AUTO){
        light_mode = ON;
        get_lcd()->print("ON");
    }
    else if(light_mode == ON){
        light_mode = OFF;
        get_lcd()->print("OFF");
    }
    else{
        light_mode = AUTO;
        get_lcd()->print("AUTO");
    }

    auto_time = -60000; // So that auto mode activates directly
    light_cycle();

    long int wait = millis();

    while(millis() - wait < 1500 ){
        // Wait 1500 ms
        if(millis() - wait > 300){
            if(check_button(button_2, change_light_mode)){
                break;
            }
        }
    }

}
