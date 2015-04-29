#ifndef _TIME
#define _TIME

#include <Time.h>
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <LiquidCrystal.h>

#include "Arduino.h"

struct Time{
    int hour;
    int minutes;
    int seconds;
};

extern Time time_now;

extern IPAddress timeServer;

const int timeZoneOffset = 7200;        // Time-zone GMT +2
const unsigned int ntpSyncTime = 3600*6;    // Sync every 6 hours


/* ALTER THESE VARIABLES AT YOUR OWN RISK */

const unsigned int localPort = 8888;      // local port to listen for UDP packets
const int NTP_PACKET_SIZE= 48;  // NTP time stamp is in the first 48 bytes of the message
extern byte packetBuffer[NTP_PACKET_SIZE]; // Buffer to hold incoming and outgoing packets
extern EthernetUDP Udp;    // A UDP instance to let us send and receive packets over UDP
extern unsigned long ntpLastUpdate;    // Keeps track of how long ago we updated the NTP server



int init_time();

void set_time_manually(LiquidCrystal *lcd, char button_left, char button_right, char button_ok);

Time get_time_now();

int sync_time();

int getTimeAndDate();
unsigned long sendNTPpacket(IPAddress& address);


#endif
