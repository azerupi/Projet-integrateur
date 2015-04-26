
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


// Relays

const char heating = 47;
const char light = 45;

// Buttons

const char button_1 = 41; // Most left button
const char button_2 = 43;
const char button_3 = 45;
const char button_4 = 47;
const char button_5 = 49;
const char button_6 = 51; // Most right button


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

    // Initialize temperature-humidity sensor
    init_sensors();


    // Initialize relays
    pinMode(heating, OUTPUT);
    pinMode(light, OUTPUT);

    // Analog pins do not have to be intitialized...
    //*/

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

    webserver.process_request();

}
