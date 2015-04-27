
/*
    ##################################################################
    ##################################################################
    ##                                                              ##
    ##      This is the arduino code of our 2015 school project:    ##
    ##                                                              ##
    ##                (AUTOMATED) HYDROPONIC SYSTEM                 ##
    ##                                                              ##
    ##                                                              ##
    ##      It has been written by:                                 ##
    ##                                  Julien Kirstein,            ##
    ##                                  Jonathan Petit,             ##
    ##                                  Pierre Santos,              ##
    ##                                  Dylan Schneider,            ##
    ##                          and     Mathieu David               ##
    ##                                                              ##
    ##################################################################
    ##################################################################
*/


// Arduino libraries
#include <Ethernet.h>
#include <SD.h>
#include <SPI.h>
#include <LiquidCrystal.h>
#include <DHT.h>
#include <Time.h>

// Our code in other files
#include "webserver.h"
#include "sensors.h"
#include "lcd.h"
#include "buttons.h"
#include "heating.h"
#include "time.h"

// LCD

// http://arduino.cc/en/Tutorial/LiquidCrystal
LiquidCrystal lcd(23, 25, 27, 29, 31, 33); // RS - ENABLE - D4 - D5 - D6 - D7
enum lcd_mode { INFO, MENU };
lcd_mode LCD_MODE = INFO;


// Motors for pH automation
//*/

// Ethernet

Webserver webserver = Webserver();




// Timers
long int sensor_time = 0;
long int lcd_time = 0;
long int heating_time = 0;






/*
    ##
    ##      Setup
    ########################
*/


void setup() {

    Serial.begin(9600);


    // Initialize LCD screen
    lcd.begin(16, 2); // Rows and columns


    // Print on the lcd
    lcd.setCursor(0,0);
    lcd.print("Initializing...");



    init_sensors();
    init_buttons();
    init_heating();


    // Analog pins do not have to be intitialized...

    // Init webserver
    webserver.begin(&lcd);

    // Sync time with Internet

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Syncing time...");
    if( !init_time() ){ // If time didn't sync from the internet, set it manually
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Unable to sync");
        delay(500);
        set_time_manually(&lcd, button_1, button_6, button_3);
    }


}






/*
    ##
    ##      Loop
    ########################
*/


void loop() {

    // Refresh temperature and humidity every 5 seconds

    if(millis() - sensor_time > 5000){
        measure_temperature();
        measure_humidity();

        sensor_time = millis();
    }



    // Update lcd

    if(LCD_MODE == INFO){                   // if in INFO mode, cycle trough the views every 3 seconds

        if(millis() - lcd_time > 3000){
            next_view(&lcd);

            lcd_time = millis();

        }
    }



    // check if temperature is equal to target_temperature

    if(millis() - heating_time > 10000){
        check_temperature();

        heating_time = millis();
    }


    // Check button states with function: void check_button(char button, void (*callback));
    // We have still to determine what the buttons are going to do...


    // Try to sync time with ntp server
    sync_time();

    // If there is a web request, process it
    webserver.process_request();

}
