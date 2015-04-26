
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


// Our code in other files
#include "webserver.h"
#include "sensors.h"
#include "lcd.h"
#include "buttons.h"


// Relays

const char heating = 2;
const char light = 1;


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

    // Initialize relays
    pinMode(heating, OUTPUT);
    pinMode(light, OUTPUT);

    // Analog pins do not have to be intitialized...

    // Init webserver
    webserver.begin(&lcd);

}




void loop() {

    // Refresh temperature and humidity every 5 seconds

    if(millis() - sensor_time > 5000){
        measure_temperature();
        measure_humidity();

        sensor_time = millis();
    }

    // update lcd
    if(LCD_MODE == INFO){
        if(millis() - lcd_time > 3000){

            next_view(&lcd);

            lcd_time = millis();

        }
    }

    // Check button states with function: void check_button(char button, void (*callback));
    // We have still to determine what the buttons are going to do...

    webserver.process_request();

}
