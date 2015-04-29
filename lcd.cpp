#include "lcd.h"

LiquidCrystal lcd(23, 25, 27, 29, 31, 33); // RS - ENABLE - D4 - D5 - D6
lcd_view current_view = BLANK;


void init_lcd(){
    // Initialize LCD screen
    lcd.begin(16, 2); // Rows and columns


    // Print on the lcd
    lcd.setCursor(0,0);
    lcd.print("Initializing...");

}

LiquidCrystal* get_lcd(){
    return &lcd;
}

void show_view(lcd_view view){

    lcd.clear();
    lcd.setCursor(0,0);

    switch(view){
            case TEMPERATURE:
                lcd.print("Temperature:");
                lcd.setCursor(9,1);
                lcd.print(get_sensor_values().temperature);
                lcd.print((char)223);
                lcd.print("C");
                break;

            case HUMIDITY:
                lcd.print("Humidity:");
                lcd.setCursor(10,1);
                lcd.print(get_sensor_values().humidity);
                lcd.print("%");
                break;

            case TIME:
                {
                    lcd.print("Time:");
                    lcd.setCursor(7,1);
                    String time_string = (get_time_now().hour < 10) ?  "0" : "" + get_time_now().hour + ':' + (get_time_now().minutes < 10) ? "0" : "" + get_time_now().minutes + ':' + (get_time_now().seconds < 10) ? "0" : "" + get_time_now().seconds;

                    lcd.print(time_string);
                }
                break;

            default:
                break;
    }

    current_view = view;
}


void next_view(){

    lcd_view next_view;

    switch(current_view){
        case TEMPERATURE:
            next_view = HUMIDITY;
            break;

        case HUMIDITY:
            next_view = TIME;
            break;

        // Don't include last view -> default will be selected and view will go back to first

        default:
            next_view = TEMPERATURE;
            break;
    }

    show_view(next_view);

}
