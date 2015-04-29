#include "buttons.h"


void init_buttons(LiquidCrystal *lcd){

    // Initialize Buttons
    pinMode(button_1, INPUT);
    pinMode(button_2, INPUT);
    pinMode(button_3, INPUT);
    pinMode(button_4, INPUT);
    pinMode(button_5, INPUT);
    pinMode(button_6, INPUT);

    if(digitalRead(button_1) == HIGH){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Error with");
        lcd->setCursor(0,1);
        lcd->print("Button 1");
        delay(2000);
    }

    if(digitalRead(button_2) == HIGH){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Error with");
        lcd->setCursor(0,1);
        lcd->print("Button 2");
        delay(2000);
    }

    if(digitalRead(button_3) == HIGH){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Error with");
        lcd->setCursor(0,1);
        lcd->print("Button 3");
        delay(2000);
    }

    if(digitalRead(button_4) == HIGH){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Error with");
        lcd->setCursor(0,1);
        lcd->print("Button 4");
        delay(2000);
    }

    if(digitalRead(button_5) == HIGH){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Error with");
        lcd->setCursor(0,1);
        lcd->print("Button 5");
        delay(2000);
    }

    if(digitalRead(button_6) == HIGH){
        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Error with");
        lcd->setCursor(0,1);
        lcd->print("Button 6");
        delay(2000);
    }

}

int check_button(char button, void (*callback)() ){

    bool invoke_callback = false;

    if(digitalRead(button) == HIGH){
        invoke_callback = true;
    }

    // This prevents the callback to be invoked repeatedly when the button is pressed
    int time_pressed = millis();

    if(invoke_callback){
        callback();
        while(digitalRead(button) == HIGH && millis() - time_pressed < 200){
            // Do nothing until the button is released or 500 ms have passed
        }
        return 1;
    }
    return 0;
}
