#include "webserver.h"

#include "sensors.h"
#include "heating.h"
#include "time.h"


bool Webserver::webserver_available = false;
IPAddress Webserver::ip;

Webserver::Webserver() : server(80) // Create a server at port 80
{
    api = false;
    request = "";
}

bool Webserver::begin(bool serial){

    // Start Ethernet configuration:

    if(serial){
        Serial.println("Configuring internet connection:");
    }


    // Try to get IP adress with DHCP protocol with Ethernet.begin(),
    // Returns 1 on success or 0 on failure
    if (!Ethernet.begin(mac)) {
        if(serial){
            Serial.println("Failed to configure Ethernet using DHCP");
            Serial.println("Webserver failed to initialize...");
        }
        webserver_available = false;
    }
    else{
        if(serial){
            // Get IP adress
            Serial.print("IP is: ");
            Serial.println(Ethernet.localIP());
            ip = Ethernet.localIP();
        }
        webserver_available = true;


        // give the Ethernet shield a second to initialize:
        delay(1000);

        // Start server to listen for incoming clients
        server.begin();

        // Initialize SD card

        if(serial){
            Serial.println("Initializing SD card...");
        }

        if (!SD.begin(4)) {
            if(serial){
                Serial.println("ERROR - SD card initialization failed!");
                // init failed
            }
        }
        else{
            if(serial){
                Serial.println("SUCCESS - SD card initialized.");
            }

            if(serial){
                // check for index.html file
                if (!SD.exists("index.htm")) {
                    Serial.println("ERROR - Can't find index.htm file!");
                    // can't find index file
                }
                else{
                    Serial.println("SUCCESS - Found index.htm file.");
                }
            }
        }

    }



    return webserver_available;
}

bool Webserver::begin(LiquidCrystal *lcd){

    lcd->clear();
    lcd->print("Configuring");
    lcd->setCursor(0,1);
    lcd->print("Ethernet");

    // Try to get IP adress with DHCP protocol with Ethernet.begin(),

    if (!Ethernet.begin(mac)) { // If connexion failed

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Connexion Failed");

        delay(2000); // This delay is for the user to have time to read...

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Webserver");
        lcd->setCursor(0,1);
        lcd->print("Unavailable");

        delay(2000); // This delay is for the user to have time to read...

        webserver_available = false;
    }
    else{ // If connexion successful

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Connexion");
        lcd->setCursor(0,1);
        lcd->print("Successful");

        delay(2000); // This delay is for the user to have time to read...

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("Webserver");
        lcd->setCursor(0,1);
        lcd->print("Available");

        delay(2000); // This delay is for the user to have time to read...

        lcd->clear();
        lcd->setCursor(0,0);
        lcd->print("IP Adress:");
        lcd->setCursor(0,1);
        lcd->print(Ethernet.localIP());
        ip = Ethernet.localIP();

        webserver_available = true;

        // give the Ethernet shield a second to initialize:
        delay(1000);

        // Start server to listen for incoming clients
        server.begin();

        // Initialize SD card

        if (!SD.begin(4)) {
            lcd->clear();
            lcd->setCursor(0,0);
            lcd->print("SD Card Error !");
        }
        else{

            if (!SD.exists("index.htm")) {
                lcd->clear();
                lcd->setCursor(0,0);
                lcd->print("index.html");
                lcd->setCursor(0,1);
                lcd->print("not found...");
                // can't find index file
            }

        }
    }

    return webserver_available;

}


void Webserver::process(){

    if(state != stateNoClient && !client){
        state = stateDisconnecting;
    }

    switch(state){

    case stateNoClient:
        // check if a new client is waiting
        //Serial.println("No client");
        client = server.available();
        if (!client)
            break;

        state = stateReadingClient;
        break;

    case stateReadingClient:
        readLineFromClient();
        Serial.print("Request: ");
        Serial.println(request);
        state = stateExtractRequest;
        break;

    case stateExtractRequest:
        extractRequest();
        Serial.print("Request: ");
        Serial.println(request);
        state = stateProcessRequest;
        break;

    case stateProcessRequest:
        processRequest();
        Serial.print("Request: ");
        Serial.println(request);
        state = stateSendHeader;
        break;

    case stateSendHeader:
        sendHeader();
        Serial.println("Send Header");
        if(api)
            state = stateProcessAPI;
        else
            state = stateOpenFile;

        break;

    case stateOpenFile:
        openFile();
        if(!webfile){
            Serial.println("File failed to open...");
            state = stateDisconnecting;
            break;
        }
        Serial.println("File Opened");
        fileDone = false;
        state = stateSendFile;
        break;

    case stateSendFile:
        sendFile();
        Serial.println("Send file...");
        if(fileDone){
            Serial.println("File done");
            state = stateDisconnecting;
        }
        break;

    case stateProcessAPI:
        processAPI();
        Serial.print("Process API");
        state = stateDisconnecting;
        break;

    case stateDisconnecting:
        client.stop();
        Serial.println("Disconecting");
        state = stateNoClient;
        break;

    default:
        Serial.println("ERROR: Unknown State");
        state = stateDisconnecting;
        break;

    }

}

void Webserver::readLineFromClient(){
    int i = 0;
    char c;

    while(client.connected() && client.available()){
        c = client.read();
        if(c == '\n' || c == '\r'){
            break;
        }
        request += c;
    }
}

void Webserver::extractRequest(){
    char startIndex = request.indexOf(' ') + 1;
    char endIndex = request.indexOf(' ', startIndex + 1);

    request = request.substring(startIndex, endIndex);
}

void Webserver::processRequest(){
    api = false;

    if(request == "/")
        request = "index.htm";
    else if(request == "/style.css")
        request = "style.css";
    else if(request.substring(0,4) == "/api"){
        api = true;
        request = request.substring(5);
    }
    else
        request = "";

}

void Webserver::sendHeader(){
    // send a standard http response header

    if(api){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        return;
    }
    else if(request == "index.htm"){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        return;
    }
    else if(request == "style.css"){
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/css");
        client.println("Connection: close");
        client.println();
        return;
    }
    else{
        client.println("HTTP/1.1 404 Not Found");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
    }
}

void Webserver::openFile(){
    Serial.print("Trying to open file: ");
    Serial.println(request);
    if(request == "")
        return;

    char requestBuf[request.length()+1];
    request.toCharArray(requestBuf, request.length()+1);
    Serial.println(requestBuf);
    if(SD.exists(requestBuf)){
        webfile = SD.open(requestBuf);
    }

}

void Webserver::sendFile(){
    if (webfile) {
        int count = 0;
        while(webfile.available()) {
            client.write(webfile.read()); // send web page to client
            if(count > 120)
                return;
        }
        webfile.close();
        fileDone = true;
    }
}

void Webserver::processAPI(){
    if(request == "sensors"){
        Serial.println("API: Sensors");
        Time ph_time = get_sensor_values().last_ph_update;
        Time now = get_time_now();
        client.print("{\"temp\": " + String(get_sensor_values().temperature) +",");
        client.print("\"ph\": " + String(get_sensor_values().pH) +",");
        client.print("\"ph_hour\": " + String(now.hour - ph_time.hour) +",");
        client.print("\"ph_minutes\": " + String(now.minutes - ph_time.minutes) +",");
        client.print("\"humidity\": " + String(get_sensor_values().humidity) +"}");

        //
    }
    else if(request == "target-temp"){
        Serial.println("API: Target Temperature");

        client.print("{\"target_temp\": " + String((int)target_temperature)  +"}");
    }
    else if(request == "raise-temp"){
        Serial.println("API: Raise Temperature");

        raise_target_temperature();
        client.print("{\"target_temp\": " + String((int)target_temperature)  +"}");
    }
    else if(request == "lower-temp"){
        Serial.println("API: Lower Temperature");

        lower_target_temperature();
        client.print("{\"target_temp\": " + String((int)target_temperature)  +"}");
    }
    else if(request == "calibrate"){
        Serial.println("API: Calibrate");

        calibrate_pH();
        client.print("/n");
    }
    else if(request == "ph"){
        Serial.println("API: pH");

        measure_pH();
        client.print("{\"ph\": " +  String(get_sensor_values().pH)  +"}");
    }
}


IPAddress Webserver::get_ip(){
    return ip;
}

bool Webserver::get_availability(){
    return webserver_available;
}
