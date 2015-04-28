#include "time.h"


IPAddress timeServer(195,220,94,163);   // IP adress for the ntp-server
unsigned long ntpLastUpdate = 0;

EthernetUDP Udp;
byte packetBuffer[NTP_PACKET_SIZE];


int init_time(){

    //Try to get the date and time
    int trys=0;

    while(!getTimeAndDate() && trys<3) {
        trys++;
    }

    if(trys == 3){ // date isn't synced
        return 0;
    }

    return 1;
}

void set_time_manually(LiquidCrystal *lcd, char button_left, char button_right, char button_ok){

    int hour, minutes, seconds = 0;

    long int button_press = 0;

    while(digitalRead(button_ok) == LOW){

        bool hours_changed = true;

        if(digitalRead(button_left) == HIGH && millis() - button_press > 200){
            button_press = millis();
            hour--;
            hours_changed = true;
        }
        else if(digitalRead(button_right) == HIGH && millis() - button_press > 200){
            button_press = millis();
            hour++;
            hours_changed = true;
        }

        if(hour > 23){
            hour = 0;
        }
        else if(hour < 0){
            hour = 23;
        }

        if(hours_changed){
            lcd->clear();
            lcd->setCursor(0,0);
            lcd->print("Set Hour:");
            lcd->setCursor(13,1);
            lcd->print(hour);
            hours_changed = false;
        }

        delay(50);
    }

    button_press = millis();

    while(digitalRead(button_ok) == LOW || millis() - button_press < 500){

        bool minutes_changed = true;

        if(digitalRead(button_left) == HIGH && millis() - button_press > 200){
            button_press = millis();
            minutes--;
            minutes_changed = true;
        }
        else if(digitalRead(button_right) == HIGH && millis() - button_press > 200){
            button_press = millis();
            minutes++;
            minutes_changed = true;
        }

        if(minutes < 0){
            minutes = 59;
        }
        else if(minutes > 59){
            minutes = 0;
        }

        if(minutes_changed){
            lcd->clear();
            lcd->setCursor(0,0);
            lcd->print("Set Minutes:");
            lcd->setCursor(13,1);
            lcd->print(minutes);
            minutes_changed = false;
        }

        delay(50);

    }

    button_press = millis();

    while(digitalRead(button_ok) == LOW  || millis() - button_press < 500){

        bool seconds_changed = true;

        if(digitalRead(button_left) == HIGH && millis() - button_press > 200){
            button_press = millis();
            seconds--;
            seconds_changed = true;
        }
        else if(digitalRead(button_right) == HIGH && millis() - button_press > 200){
            button_press = millis();
            seconds++;
            seconds_changed = true;
        }

        if(seconds < 0){
            seconds = 59;
        }
        else if(seconds > 59){
            seconds = 0;
        }

        if(seconds_changed){
            lcd->clear();
            lcd->setCursor(0,0);
            lcd->print("Set Seconds:");
            lcd->setCursor(13,1);
            lcd->print(seconds);
            seconds_changed = false;
        }


        delay(50);

    }

    lcd->clear();
    lcd->setCursor(0,0);
    lcd->print("Time is set to:");
    lcd->setCursor(0,1);
    lcd->print(hour);
    lcd->print(":");
    lcd->print(minutes);
    lcd->print(":");
    lcd->print(seconds);

    setTime(hour, minutes, seconds, 1, 1, 2015);

    delay(2000);
}


int sync_time(){

    // Update the time via NTP server every X seconds set in ntpSyncTime
    if(now()-ntpLastUpdate > ntpSyncTime) {

        int trys=0;
        while(!getTimeAndDate() && trys<3){
            trys++;
        }

        if(trys<3){        // When synced, return true
            return 1;
        }
    }
    return 0;

}


int getTimeAndDate() {
    int flag=0;
    Udp.begin(localPort);
    sendNTPpacket(timeServer);

    delay(1000);

    if (Udp.parsePacket()){

        Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

        unsigned long highWord, lowWord, epoch;
        highWord = word(packetBuffer[40], packetBuffer[41]);
        lowWord = word(packetBuffer[42], packetBuffer[43]);
        epoch = highWord << 16 | lowWord;
        epoch = epoch - 2208988800 + timeZoneOffset;

        flag=1;

        setTime(epoch);
        ntpLastUpdate = now();

    }
    return flag;
}

// Do not alter this function, it is used by the system
unsigned long sendNTPpacket(IPAddress& address)
{
    memset(packetBuffer, 0, NTP_PACKET_SIZE);
    packetBuffer[0] = 0b11100011;
    packetBuffer[1] = 0;
    packetBuffer[2] = 6;
    packetBuffer[3] = 0xEC;
    packetBuffer[12]  = 49;
    packetBuffer[13]  = 0x4E;
    packetBuffer[14]  = 49;
    packetBuffer[15]  = 52;
    Udp.beginPacket(address, 123);
    Udp.write(packetBuffer,NTP_PACKET_SIZE);
    Udp.endPacket();
}
