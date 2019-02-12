#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif

#include <ArduinoJson.h>
#include <MqttConnector.h>

#include "MqttConfig.h"
#include "fauxmoESP.h"
#include "init_mqtt.h"
#include "_publish.h"
#include "_receive.h"
#include "credentials.h"

MqttConnector *mqtt; 

char myName[40];

fauxmoESP fauxmo;

int relayPin = 15; 
int relayPinState = HIGH;
int LED_PIN = 2;

// -----------------------------------------------------------------------------

#define SERIAL_BAUDRATE     115200

#define LED_YELLOW          4
#define LED_GREEN           5
#define LED_BLUE            0
#define LED_PINK            2
#define LED_WHITE           15

#define ID_PLUG_01           "light number one"
#define ID_PLUG_02           "light number two"
#define ID_PLUG_03           "light number three"
#define ID_PLUG_04           "light number four"
#define ID_PLUG_05           "light number five"
#define ID_PLUG_06           "light number six"
#define ID_PLUG_07           "light number seven"
#define ID_PLUG_08           "light number eigth"
#define ID_PLUG_09           "light number nine"
#define ID_PLUG_10           "light number ten"

#define ID_LIGHT_ALL         "all light"
#define ID_PLUG_ALL         "all plug"


// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Wifi
// -----------------------------------------------------------------------------

void wifiSetup() {

    // Set WIFI module to STA mode
    WiFi.mode(WIFI_STA);

    // Connect
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    // Wait
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();

    // Connected!
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void setup() {

    // Init serial port and clean garbage
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();

    // LEDs
    pinMode(LED_YELLOW, OUTPUT);
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(LED_PINK, OUTPUT);
    pinMode(LED_WHITE, OUTPUT);
    digitalWrite(LED_YELLOW, LOW);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_PINK, LOW);
    digitalWrite(LED_WHITE, LOW);

    // Wifi
    wifiSetup();

    // By default, fauxmoESP creates it's own webserver on the defined port
    // The TCP port must be 80 for gen3 devices (default is 1901)
    // This has to be done before the call to enable()
    fauxmo.createServer(true); // not needed, this is the default value
    fauxmo.setPort(80); // This is required for gen3 devices

    // You have to call enable(true) once you have a WiFi connection
    // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // You can use different ways to invoke alexa to modify the devices state:
    // "Alexa, turn on light number one"
    // "Alexa, turn off light number one
    // "Alexa, set yellow lamp to fifty" (50 means 50% of brightness, note, this example does not use this functionality)

    // Add virtual devices
    fauxmo.addDevice(ID_PLUG_01);
    fauxmo.addDevice(ID_PLUG_02);
    fauxmo.addDevice(ID_PLUG_03);
    fauxmo.addDevice(ID_PLUG_04);
    fauxmo.addDevice(ID_PLUG_05);
    fauxmo.addDevice(ID_PLUG_06);
    fauxmo.addDevice(ID_PLUG_07);
    fauxmo.addDevice(ID_PLUG_08);
    fauxmo.addDevice(ID_PLUG_09);
    fauxmo.addDevice(ID_PLUG_10);
    fauxmo.addDevice(ID_LIGHT_ALL);

//    fauxmo.addDevice(ID_GREEN);
//    fauxmo.addDevice(ID_BLUE);
//    fauxmo.addDevice(ID_PINK);
//    fauxmo.addDevice(ID_WHITE);

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
        
        // Callback when a command from Alexa is received. 
        // You can use device_id or device_name to choose the element to perform an action onto (relay, LED,...)
        // State is a boolean (ON/OFF) and value a number from 0 to 255 (if you say "set kitchen light to 50%" you will receive a 128 here).
        // Just remember not to delay too much here, this is a callback, exit as soon as possible.
        // If you have to do something more involved here set a flag and process it in your main loop.
        
        Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);

        // Checking for device_id is simpler if you are certain about the order they are loaded and it does not change.
        // Otherwise comparing the device_name is safer.

        if (strcmp(device_name, ID_PLUG_01)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-001/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-001/$/command", state ? "ON" : "OFF", false);
        } else if (strcmp(device_name, ID_PLUG_02)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-002/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-002/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_03)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-003/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-003/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_04)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-004/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-004/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_05)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-005/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-005/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_06)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-006/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-006/$/command", state ? "ON" : "OFF", false);        
        } else if (strcmp(device_name, ID_PLUG_07)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-007/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-007/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_08)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-008/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-008/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_09)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-009/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-009/$/command", state ? "ON" : "OFF", false);          
        } else if (strcmp(device_name, ID_PLUG_10)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-010/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-010/$/command", state ? "ON" : "OFF", false);
        } else if (strcmp(device_name, ID_PLUG_ALL)==0 || strcmp(device_name, ID_LIGHT_ALL)==0) {
          mqtt->sync_advpub("", "CMMC/PLUG-001/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-002/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-003/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-004/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-005/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-006/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-007/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-008/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-009/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "CMMC/PLUG-010/$/command", state ? "ON" : "OFF", false);

          mqtt->sync_advpub("", "WORKSHOP/PLUG-001/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-002/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-003/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-004/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-005/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-006/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-007/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-008/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-009/$/command", state ? "ON" : "OFF", false);
          mqtt->sync_advpub("", "WORKSHOP/PLUG-010/$/command", state ? "ON" : "OFF", false);

        } 
//        else if (strcmp(device_name, ID_GREEN)==0) {
//            digitalWrite(LED_GREEN, state ? HIGH : LOW);
//        } else if (strcmp(device_name, ID_BLUE)==0) {
//            digitalWrite(LED_BLUE, state ? HIGH : LOW);
//        } else if (strcmp(device_name, ID_PINK)==0) {
//            digitalWrite(LED_PINK, state ? HIGH : LOW);
//        } else if (strcmp(device_name, ID_WHITE)==0) {
//            digitalWrite(LED_WHITE, state ? HIGH : LOW);
//        }

    });

    init_mqtt();
}

void loop() {

    mqtt->loop();
    // fauxmoESP uses an async TCP server but a sync UDP server
    // Therefore, we have to manually poll for UDP packets
    fauxmo.handle();

    // This is a sample code to output free heap every 5 seconds
    // This is a cheap way to detect memory leaks
    static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

    // If your device state is changed by any other means (MQTT, physical button,...)
    // you can instruct the library to report the new state to Alexa on next request:
    // fauxmo.setState(ID_YELLOW, true, 255);

}
