#include "webserver.h"

#include "sensors.h"

Webserver::Webserver() :
    server(80), // Create a server at port 80
    webserver_available(false) // Webserver is not available until begin() is called
{

}

bool Webserver::begin(bool serial){

    // Start Ethenet configuration:

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


void Webserver::process_request(){

    EthernetClient client = server.available();  // try to get client

    String httpReq = "";
    bool httpReqFirstLine = true;

    if (client) {  // got client?
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client

                if(httpReqFirstLine){
                    httpReq += c;
                    if(c == '\n'){
                        httpReqFirstLine = false;
                    }
                }

                // every line of text received from the client ends with \r\n
                else if (c != '\r') {
                    // a text character was received from client
                    currentLineIsBlank = false;
                }

                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {

                    if(currentLineIsBlank){
                        String parsed_request = parse_request(httpReq);
                        Serial.println(parsed_request);

                        serve_response(&client, parsed_request);

                        break;
                    }
                    else{
                        // last character on line of received text
                        // starting new line with next character read
                        currentLineIsBlank = true;
                    }

                }

            } // end if (client.available())
        } // end while (client.connected())
        delay(1);      // give the web browser time to receive the data
        client.stop(); // close the connection
    } // end if (client)

}


void Webserver::serve_response(EthernetClient *client, String request){

    // send a standard http response header
    client->println("HTTP/1.1 200 OK");

    String file = "";

    if(request == "/"){
        file = "index.htm";
        client->println("Content-Type: text/html");
        client->println("Connection: close");
        client->println();
    }
    else if(request == "/style.css"){
        file = "style.css";
        client->println("Content-Type: text/css");
        client->println("Connection: close");
        client->println();
    }
    else if(request.substring(0,4) == "/api"){
        Serial.println("API...");
        process_api_request(client, request.substring(5));
    }

    if(file != ""){

        // File
        File webFile;

        char requestBuf[file.length()+1];
        file.toCharArray(requestBuf, file.length()+1);

        if(SD.exists(requestBuf)){
            webFile = SD.open(requestBuf);
        }
        else{
            Serial.println("File does not exist on the SD card");
        }

        // send web page
        if (webFile) {
            while(webFile.available()) {
                client->write(webFile.read()); // send web page to client
            }
            webFile.close();
        }
    }
}


void Webserver::process_api_request(EthernetClient *client, String request){

    client->println("Content-Type: application/json");
    client->println("Connection: close");
    client->println();

    Serial.println(request);

    if(request == "sensors"){
        client->print("{\"temp\": " + String(get_sensor_values().temperature) +",");
        client->print("\"ph\": " + String(get_sensor_values().pH) +",");
        client->print("\"humidity\": " + String(get_sensor_values().humidity) +"}");
    }


}


String Webserver::parse_request(String request){

    char startIndex = request.indexOf(' ') + 1;
    char endIndex = request.indexOf(' ', startIndex + 1);

    String processed = request.substring(startIndex, endIndex);

    return processed;
}
