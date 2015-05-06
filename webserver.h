#ifndef WEBSERVER
#define WEBSERVER

#include <Ethernet.h>
#include <SD.h>
#include <LiquidCrystal.h>

#include "Arduino.h"


class Webserver
{
public:
    Webserver();

    bool begin(bool serial=false);
    bool begin(LiquidCrystal *lcd);

    void process();
    static IPAddress get_ip();
    static bool get_availability();



private:
    byte mac[6] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    EthernetServer server;
    static IPAddress ip;
    static bool webserver_available;

    String request;
    bool api;
    bool fileDone;
    File webfile;

    enum{
        stateNoClient,
        stateReadingClient,
        stateExtractRequest,
        stateProcessRequest,
        stateSendHeader,
        stateOpenFile,
        stateSendFile,
        stateProcessAPI,
        stateDisconnecting,
    };

    char state;
    EthernetClient client;

    void readLineFromClient();
    void extractRequest();
    void processRequest();
    void sendHeader();
    void openFile();
    void sendFile();
    void processAPI();

};


#endif
