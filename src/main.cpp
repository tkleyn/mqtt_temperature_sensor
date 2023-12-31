//main.cpp
#include <Arduino.h>
#include <time.h>
#include "Wire.h"
#include "SHT2x.h"

///#include <NTPClient.h>

#include "wifi_mqtt.h"

uint32_t start;
uint32_t stop;

#define SDA   16
#define SCL   17

SHT2x sht;

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
	/*struct tm timeinfo;
	while(!getLocalTime(&timeinfo))
	{
		Serial.println("Failed to obtain time");
	}
	//return (String(&timeinfo));
	*/
	return String("Mon 25/03 12:23");
}

void setup()
{
	Serial.begin(9600);

	wifi_mqtt_init();

	//init and get the time
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	//printLocalTime();

	Wire.begin(SDA, SCL);
	sht.begin();

	uint8_t stat = sht.getStatus();
	Serial.print(stat, HEX);
	Serial.println();
}

void loop() 
{
	sht.read();

	temp = sht.getTemperature();
	humidity = sht.getHumidity();
	timestamp = get_time_str();

	if (wifi_connect(10) != WL_CONNECTED)
	{
		Serial.println("Failed to connect to Wi-Fi. Exiting... ");
		exit (-1);
	}

	if (mqtt_connect(10) != MQTT_CONNECTED)
	{
		Serial.println("Failed to connect to MQTT broker. Exiting... ");
		exit (-1);
	}

	publishMessage(topic_temperature, (String(temp)), true);
	publishMessage(topic_humidity, (String(humidity)), true);
	publishMessage(topic_timestamp, timestamp, true);

	delay(wait_before_publish);
}
