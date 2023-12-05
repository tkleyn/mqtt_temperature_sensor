//wifi_mqtt.cpp

#include "wifi_mqtt.h"

WiFiClientSecure espClient;
PubSubClient client(espClient);

void wifi_mqtt_init()
{
	WiFi.mode(WIFI_STA);

	espClient.setCACert(root_ca);
	client.setServer(mqtt_server, mqtt_port);
	client.setSocketTimeout(5);
}

int8_t wifi_connect(uint8_t timeout)
{
	if (WiFi.status() != WL_CONNECTED)
	{
		int elapsed_time = 0;
		Serial.print("Connecting to WiFi..");

		WiFi.begin(ssid, password);

		while (WiFi.status() != WL_CONNECTED && elapsed_time < timeout * 1000) {
			delay(500);
			Serial.print(".");
			elapsed_time += 500;
		}

		if (WiFi.status() == WL_CONNECTED)
		{
			Serial.println("connected!");
			Serial.print("IP address: ");
			Serial.println(WiFi.localIP());
			return (WL_CONNECTED);
		} 
		else
		{
			Serial.println("connection failed.");
			return (WL_CONNECT_FAILED);
		}
	}
	return (WL_CONNECTED);
}

int8_t mqtt_connect(uint8_t timeout) 
{
	int elapsed_time = 0;

	while (!client.connected())
	{
		Serial.println("\nAttempting MQTT connection…");
		String clientId = "HumidorClient-"; // Create a random client ID
		clientId += String(random(0xffff), HEX);
		// Attempt to connect
		if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
		{
			Serial.println("connected");
		}
		else if(elapsed_time < timeout * 1000 )
		{
			delay(500);
			Serial.print(".");
			elapsed_time += 500;
		}
		else 
		{
			return (client.state());
		}
	}
	return (MQTT_CONNECTED);
}

//======================================= publising as string
void publishMessage(const char* topic, String payload , boolean retained)
{
	Serial.println("Message to publish ["+String(topic)+"]: "+payload);
	boolean publishStatus = client.publish(topic, payload.c_str(), true);

	if (publishStatus)
	{
		Serial.println("Message published successfully.");
		Serial.println("Message publised ["+String(topic)+"]: "+payload);

	} 
	else 
	{
		Serial.println("Publish failed.");
		Serial.print("MQTT Client State: "); Serial.println(client.state());
	}
}

//======================================= publising as uint8_t ( byte )
void publishMessage_bytes(const char* topic, uint8_t payload[] , boolean retained)
{
  if (client.publish(topic, payload, true))
  Serial.println("Message publised ["+String(topic)+"] ");
}
