#include "sensors.h"

SensorValues sensor_values = { 0, 0, 0, 0};
DHT temperature_humidity_sensor(53, DHT22);

float offset = 0;

void init_sensors(){
    measure_temperature();
    measure_humidity();
    calibrate_pH();
    measure_pH();
}

SensorValues get_sensor_values(){
    return sensor_values;
}

void measure_pH(){

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("Measuring pH...");

    float phValue = pH();

    sensor_values.pH = phValue;
    sensor_values.last_ph_update = get_time_now();

}

float pH(){
    int buffer[10];

    for(int i = 0; i < 10; i++)
    {
        buffer[i] = analogRead(pH_sensor);
        delay(10);
    }

    int temp;
    for(int i = 0; i < 9; i++)
    {
        for(int j = i + 1; j < 10; j++)
        {
            if(buffer[i] > buffer[j])
            {
                temp = buffer[i];
                buffer[i] = buffer[j];
                buffer[j] = temp;
            }
        }
    }

    int avgValue = 0;
    for(int i = 2; i < 8; i++){
        avgValue+=buffer[i];
    }
    avgValue = avgValue / 6;

    float phValue= (float) avgValue * 5.0 / 1024;     //convert the analog into volt
    phValue= 3.5 * phValue + offset;                 //convert the millivolt into pH value

    return phValue;
}

void measure_temperature(){
    sensor_values.temperature = temperature_humidity_sensor.readTemperature();
}

void measure_humidity(){
    sensor_values.humidity = temperature_humidity_sensor.readHumidity();
}


void calibrate_pH(){

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("Calibrating pH...");
    get_lcd()->setCursor(0,1);
    get_lcd()->print("(3) when pH 7");

    while(digitalRead(button_3) == LOW){
        // Wait
    }

    offset = 7.0 - pH();

    while(digitalRead(button_3) == LOW){

        float phError = 4.66 - pH();
        get_lcd()->clear();
        get_lcd()->setCursor(0,0);
        get_lcd()->print("Change Gain");
        get_lcd()->setCursor(0,1);
        get_lcd()->print("pH Error = ");
        get_lcd()->print(phError);

        delay(100);

    }

    get_lcd()->clear();
    get_lcd()->setCursor(0,0);
    get_lcd()->print("pH Calibrated");

    delay(2000);

}


void pH_menu(){
    LiquidCrystal *lcd = get_lcd();

    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print("(1) to Measure");
    lcd->setCursor(0,1);
    lcd->print("(2) to Calibrate");

    while(!check_button(button_1, measure_pH) && !check_button(button_2, calibrate_pH) && digitalRead(button_6) == LOW){
        // Wait
    }

    long int pressed = millis();

    while (digitalRead(button_6) == LOW && millis() - pressed < 500) {
        // Wait
    }


}
