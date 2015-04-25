
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


// Relays

const char heating = 47;
const char light = 45;


// LCD

// http://arduino.cc/en/Tutorial/LiquidCrystal
LiquidCrystal lcd(23, 25, 27, 29, 31, 33); // RS - ENABLE - D4 - D5 - D6 - D7


// Motors for pH automation
//*/

// Ethernet

Webserver webserver = Webserver();




// Timers
long int sensor_time = 0;






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
    Serial.println("Initializing...");


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

    if(millis() - sensor_time > 5000){
        measure_temperature();
        measure_humidity();

        Serial.println(get_sensor_values().temperature);
        lcd.clear();
        lcd.setCursor(1,0);
        lcd.print(get_sensor_values().temperature);

        sensor_time = millis();
    }

    webserver.process_request();

}
