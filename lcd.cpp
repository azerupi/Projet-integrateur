#include "lcd.h"

lcd_view current_view = BLANK;

void show_view(LiquidCrystal *lcd, lcd_view view){

    lcd->clear();
    lcd->setCursor(0,0);

    switch(view){
            case TEMPERATURE:
                lcd->print("Temperature:");
                lcd->setCursor(1,13);
                lcd->print(get_sensor_values().temperature);
                break;

            case HUMIDITY:
                lcd->print("Humidity:");
                lcd->setCursor(1,13);
                lcd->print(get_sensor_values().humidity);
                break;

            default:
                break;
    }

    current_view = view;

}


void next_view(LiquidCrystal *lcd){

    lcd_view next_view;

    switch(view){
        case TEMPERATURE:
            next_view = HUMIDITY;

        // Don't include last view -> default will be selected and view will go back to first
        
        default:
            next_view = TEMPERATURE;
    }

    show_view(lcd, next_view);

}
