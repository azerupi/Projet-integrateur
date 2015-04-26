#include "buttons.h"


void init_buttons(){

    // Initialize Buttons
    pinMode(button_1, INPUT);
    pinMode(button_2, INPUT);
    pinMode(button_3, INPUT);
    pinMode(button_4, INPUT);
    pinMode(button_5, INPUT);
    pinMode(button_6, INPUT);

}

void check_button(char button, void (*callback)()){

    bool invoke_callback = false;

    if(digitalRead(button) == LOW){
        invoke_callback = true;
    }

    // This prevents the callback to be invoked repeatedly when the button is pressed
    int time_pressed = millis();
    while(digitalRead(button) == LOW && millis() - time_pressed < 500){

        // Do nothing until the button is released or 500 ms have passed
    }

    callback();

}
