#include "lcd.h"

lcd_view current_view = BLANK;

void show_view(LiquidCrystal *lcd, lcd_view view){

    lcd->clear();
    lcd->setCursor(0,0);

    switch(view){
            case TEMPERATURE:
                lcd->print("Temperature:");
                lcd->setCursor(9,1);
                lcd->print(get_sensor_values().temperature);
                lcd->print((char)223);
                lcd->print("C");
                break;

            case HUMIDITY:
                lcd->print("Humidity:");
                lcd->setCursor(10,1);
                lcd->print(get_sensor_values().humidity);
                lcd->print("%");
                break;

            default:
                break;
    }

    current_view = view;
}


void next_view(LiquidCrystal *lcd){

    lcd_view next_view;

    switch(current_view){
        case TEMPERATURE:
            next_view = HUMIDITY;
            break;

        // Don't include last view -> default will be selected and view will go back to first

        default:
            next_view = TEMPERATURE;
            break;
    }

    show_view(lcd, next_view);

}
