#include <Arduino.h> 

/* BOARD INFO */
String DEVICE_NAME      = "your_alexa"; 

/* MQTT INFO */ 
String MQTT_HOST        = "mqtt.cmmc.io";
String MQTT_USERNAME    = "";
String MQTT_PASSWORD    = "";
String MQTT_CLIENT_ID   = "";
String MQTT_PREFIX      = "/workshop";

int    MQTT_PORT        = 1883;
int PUBLISH_EVERY       = 10L * 1000;
int MQTT_CONNECT_TIMEOUT= 10; 
