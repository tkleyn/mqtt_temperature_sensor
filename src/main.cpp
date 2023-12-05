//main.cpp
#include <Arduino.h>
#include <time.h>
#include <Wire.h>

///#include <Wire.h>
///include <tinySHT2x.h>
///#include <NTPClient.h>

#include "wifi_mqtt.h"

//------Configs NTP
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

//---- Topics names
const char* topic_temperature = "temperature";
const char* topic_humidity = "humidity";
const char* topic_timestamp = "timestamp";

//---- Timings
int wait_before_publish = 10000; // attend 10 secondes avant de publier les sensors

// sensor value variables 
float temp;
float humidity;

String timestamp;

///tinySHT2x sht;

void printLocalTime()
{
	struct tm timeinfo;
	if(!getLocalTime(&timeinfo))
	{
		Serial.println("Failed to obtain time");
		return;
	}
	Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

String get_time_str()
{
	struct tm timeinfo;
	while(!getLocalTime(&timeinfo))
	{
	Serial.println("Failed to obtain time");
	}
	//return (String(&timeinfo));
	return String("Mon 25/03 12:23");
}

void setup()
{
	Serial.begin(9600);

	///sht.begin();

	wifi_mqtt_init();

	//init and get the time
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	printLocalTime();
}

void loop() 
{
	if (WiFi.status() != WL_CONNECTED)
	{
		wifi_connect(10);
	}

	if (!client.connected())
	{
		Serial.println("MQTT client not connected.");
		mqtt_connect();
	}

	///temp = sht.getTemperature();
	//humidity = sht.getHumidity();
	///timestamp = ;

	temp = 17.5;
	humidity = 65.2;
	timestamp = get_time_str();

	if (client.connected() != MQTT_CONNECTED)
	{
		Serial.println("MQTT client not connected.");
		mqtt_connect();
	}

	publishMessage(topic_temperature, (String(temp)), true);
	publishMessage(topic_humidity, (String(humidity)), true);
	publishMessage(topic_timestamp, timestamp, true);

	delay(wait_before_publish);
}
