#include "time.h"


IPAddress timeServer(195,220,94,163);   // IP adress for the ntp-server
unsigned long ntpLastUpdate = 0;

EthernetUDP Udp;
byte packetBuffer[NTP_PACKET_SIZE];


void init_time(){

    //Try to get the date and time
    int trys=0;

    while(!getTimeAndDate() && trys<10) {
        trys++;
    }
}


bool sync_time(){

    // Update the time via NTP server every X seconds set in ntpSyncTime
    if(now()-ntpLastUpdate > ntpSyncTime) {

        int trys=0;
        while(!getTimeAndDate() && trys<10){
            trys++;
        }

        if(trys<10){        // When synced, return true
            return true;
        }
    }
    return false;

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
