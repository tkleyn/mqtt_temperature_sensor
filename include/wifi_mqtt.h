#ifndef WIFI_MQTT_H
#define WIFI_MQTT_H

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

***REMOVED***

extern WiFiClientSecure espClient;
extern PubSubClient client;

//
void  wifi_mqtt_init();

//
void wifi_connect(int timeout);

//
void mqtt_connect();

//publising as string
void publishMessage(const char* topic, String payload , boolean retained);

//publising as uint8_t ( byte )
void publishMessage_bytes(const char* topic, uint8_t payload[] , boolean retained);

#endif