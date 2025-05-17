#include <Arduino.h>
#include "at_parser.h"
#include <WiFi.h>

void processCommand(const char *cmd) {

     if (strncmp(cmd, "AT+WIFI=", 8) == 0) {
        // Format: AT+WIFI=SSID,PASSWORD
        const char *params = cmd + 8;
        char ssid[32] = {0};
        char password[64] = {0};
        
        const char *commaPos = strchr(params, ',');
        if (commaPos) {
            int ssidLen = commaPos - params;
            strncpy(ssid, params, ssidLen);
            ssid[ssidLen] = '\0';
            strncpy(password, commaPos + 1, sizeof(password) - 1);
            
            Serial.print("Connecting to WiFi SSID: ");
            Serial.println(ssid);
            
            WiFi.begin(ssid, password);
            
            int timeout = 10; 
            while (WiFi.status() != WL_CONNECTED && timeout > 0) {
                delay(1000);
                Serial.print(".");
                timeout--;
            }
            if (WiFi.status() == WL_CONNECTED) {
                Serial.println("\nOK: WiFi connected");
            } else {
                Serial.println("\nERROR: WiFi connection failed");
            }
        }
        else {
            Serial.println("ERROR: Invalid WIFI parameters");
        }
    }
    else if (strcmp(cmd, "AT+WIFI?") == 0) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.print("OK: Connected to ");
            Serial.println(WiFi.SSID());
        } else {
            Serial.println("OK: Not connected");
        }
    }
    else if (strcmp(cmd, "AT+WIFIDISCONNECT") == 0) {
        WiFi.disconnect();
        Serial.println("OK: WiFi disconnected");
    }
    else if (strcmp(cmd, "AT+IP") == 0) {
        if (WiFi.status() == WL_CONNECTED) {
            Serial.print("OK: IP address: ");
            Serial.println(WiFi.localIP());
        } else {
            Serial.println("ERROR: Not connected to WiFi");
        }
    }
    else {
        Serial.println("ERROR: Unknown Command");
    }
}

void setup() {
    Serial.begin(115200);
    at_parser_init();
    at_parser_set_callback(processCommand);
    Serial.println("AT Command Parser Ready");
}

void loop() {
    while (Serial.available()) {
        char c = Serial.read();
        at_parser_process_char(c);
    }
}
