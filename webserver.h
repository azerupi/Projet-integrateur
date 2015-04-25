#ifndef WEBSERVER
#define WEBSERVER

#include <Ethernet.h>
#include <SD.h>
#include <LiquidCrystal.h>

// To have access to saved sensor readings
#include "sensors.h"

class Webserver
{
public:
    Webserver();
    
    bool begin(bool serial=false);
    bool begin(LiquidCrystal *lcd);

    void process_request();


private:
    byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    EthernetServer server;

    bool webserver_available;

    String parse_request(String request);
    void serve_response(EthernetClient *client, String request);
    void process_api_request(EthernetClient *client, String request);
    
};

#endif