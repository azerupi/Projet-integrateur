
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
#include "buttons.h"
#include "sensors.h"
#include "lcd.h"
#include "heating.h"
#include "webserver.h"
#include "time.h"
#include "light.h"


// Motors for pH automation
//*/

// Ethernet

Webserver webserver = Webserver();




// Timers
long int sensor_time = 0;
long int lcd_time = 0;






/*
    ##
    ##      Setup
    ########################
*/


void setup() {

    Serial.begin(9600);

    init_lcd();
    init_buttons(&lcd);


    // Init webserver
    webserver.begin(get_lcd());

    // Sync time with Internet

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("Syncing time...");
    if(!init_time()){ // If time didn't sync from the internet, set it manually
        get_lcd()->clear();
        get_lcd()->setCursor(0,0);
        get_lcd()->print("Unable to sync");
        delay(500);
        set_time_manually();
    }

    init_sensors(); // Init sensors after time (because pH depends on it)
    init_light();
    init_heating();

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

        // check also if heating is needed
        check_temperature();
    }


    // Update lcd

    if(millis() - lcd_time > 3000){
        next_view();
        lcd_time = millis();
    }


    // Check button states with function: void check_button(char button, void (*callback)());

    if( check_button(button_1, change_temperature_target) ||
        check_button(button_2, change_light_mode) ||
        check_button(button_3, pH_menu) ||
        check_button(button_4, set_time_manually)
         || check_button(button_5, webserver_reconnect )
        ){

            next_view();
            lcd_time = millis();
    }

    if( digitalRead(button_6) == LOW ){
        long int time_pressed = millis();
        while(digitalRead(button_6) == LOW && millis() - time_pressed < 500){

        }
        next_view();
        lcd_time = millis();
    }


    // Light cycle
    light_cycle();


    // Try to sync time with ntp server
    sync_time();


    // If there is a web request, process it
    webserver.process();

}





void webserver_reconnect(){
    webserver.begin(get_lcd());
}
