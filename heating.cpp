#include "heating.h"

char target_temperature = 25;
bool heating_active = false;

long int resistors_time = 0;

void init_heating(){
    pinMode(heating, OUTPUT);
}


void change_temperature_target(){

        long int button_press = 0;
        bool target_temperature_changed = true;

        while(digitalRead(button_3) == LOW){

            if(digitalRead(button_1) == HIGH && millis() - button_press > 200){
                button_press = millis();
                lower_target_temperature();
                target_temperature_changed = true;
            }
            else if(digitalRead(button_2) == HIGH && millis() - button_press > 200){
                button_press = millis();
                raise_target_temperature();
                target_temperature_changed = true;
            }

            if(target_temperature > 30){
                target_temperature = 30;
            }

            if(target_temperature_changed){
                get_lcd()->clear();
                get_lcd()->setCursor(0,0);
                get_lcd()->print("Set target temp:");
                get_lcd()->setCursor(13,1);
                get_lcd()->print((int)target_temperature);
                get_lcd()->print((char)223);
                get_lcd()->print("C");
                target_temperature_changed = false;
            }

            delay(50);

        }
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

    if(get_sensor_values().temperature < target_temperature){

        if(millis() - resistors_time > 90000){
            digitalWrite(heating, HIGH);
            resistors_time = millis();
        }
        else if(millis() - resistors_time < 90000 && millis() - resistors_time > 60000){
            digitalWrite(heating, LOW);
        }

    }
    else{
        digitalWrite(heating, LOW);
    }

}
