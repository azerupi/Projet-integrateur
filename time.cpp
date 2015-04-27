#include "time.h"


IPAddress timeServer(195,220,94,163);   // IP adress for the ntp-server
unsigned long ntpLastUpdate = 0;

EthernetUDP Udp;
byte packetBuffer[NTP_PACKET_SIZE];


int init_time(){

    //Try to get the date and time
    int trys=0;

    while(!getTimeAndDate() && trys<10) {
        trys++;
    }

    if(trys == 10){ // date isn't synced
        return 0;
    }

    return 1;
}

void set_time_manually(LiquidCrystal *lcd, char button_left, char button_right, char button_ok){

    int hour, minutes, seconds = 0;

    long int blink = millis();
    long int button_press = 0;

    while(digitalRead(button_ok) == HIGH){

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Set Hour:");
        lcd->setCursor(13,1);

        if(digitalRead(button_left) == LOW && millis() - button_press > 200){
            button_press = millis();
            hour--;
            if(hour < 0){
                hour = 23;
            }
        }
        else if(digitalRead(button_right) == LOW && millis() - button_press > 200){
            button_press = millis();
            hour++;
            if(hour > 23){
                hour = 0;
            }
        }

        if(millis() - blink > 300){
            lcd->print(hour);
        }
        else if(millis() - blink > 400){
            blink = millis();
        }
    }

    button_press = millis();

    while(digitalRead(button_ok) == HIGH || millis() - button_press < 200){

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Set Minutes:");
        lcd->setCursor(13,1);

        if(digitalRead(button_left) == LOW && millis() - button_press > 200){
            button_press = millis();
            minutes--;
            if(minutes < 0){
                minutes = 60;
            }
        }
        else if(digitalRead(button_right) == LOW && millis() - button_press > 200){
            button_press = millis();
            minutes++;
            if(minutes > 60){
                minutes = 0;
            }
        }

        if(millis() - blink > 300){
            lcd->print(minutes);
        }
        else if(millis() - blink > 400){
            blink = millis();
        }

    }

    button_press = millis();

    while(digitalRead(button_ok) == HIGH || millis() - button_press < 200){

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Set Seconds:");
        lcd->setCursor(13,1);

        if(digitalRead(button_left) == LOW && millis() - button_press > 200){
            button_press = millis();
            seconds--;
            if(seconds < 0){
                seconds = 60;
            }
        }
        else if(digitalRead(button_right) == LOW && millis() - button_press > 200){
            button_press = millis();
            seconds++;
            if(seconds > 60){
                seconds = 0;
            }
        }

        if(millis() - blink > 300){
            lcd->print(seconds);
        }
        else if(millis() - blink > 400){
            blink = millis();
        }

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

    delay(500);
}


int sync_time(){

    // Update the time via NTP server every X seconds set in ntpSyncTime
    if(now()-ntpLastUpdate > ntpSyncTime) {

        int trys=0;
        while(!getTimeAndDate() && trys<10){
            trys++;
        }

        if(trys<10){        // When synced, return true
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
