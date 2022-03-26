#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <PubSubClient.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <password.h>

// Trasmettitore
// #define REQUEST_TOPIC "/GPS/Request"
// #define RESPONSE_TOPIC "/GPS/Response"

// Ricevitore
#define REQUEST_TOPIC "/GPS/Response"
#define RESPONSE_TOPIC "/GPS/Request"

void reconnect();

WiFiClient mqttClient;
PubSubClient mqtt(mqttClient);

AsyncWebServer server(80);

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);

	Serial.begin(9600);

	WiFi.mode(WIFI_STA);
	WiFi.begin(STASSID, STAPSK);

	while(WiFi.status() != WL_CONNECTED){
		digitalWrite(LED_BUILTIN, LOW);
		delay(500);
		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);
  }

	mqtt.setServer("192.168.1.100", 1883);
	mqtt.setCallback([](char *topic, byte *payload, unsigned int length){
		if(strcmp(REQUEST_TOPIC, topic) == 0)
			Serial.println((char*) payload);
	});

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
		request->send(200, "text/plain", "Hi, i'm ESP8266 GPS!");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();
}

void loop(){
	while(!mqtt.connected())
		reconnect();

	if(mqtt.connected() && Serial.available()){
		char str[128];
		strcpy(str, Serial.readStringUntil('\n').c_str());
		str[strlen(str) - 1] = '\t';

		mqtt.publish(RESPONSE_TOPIC, str);
	}

	mqtt.loop();
}

void reconnect(){
	//Create a random client ID.
	String id = F("GPS-");
	id += String(random(0xffff), HEX);

	//Attempt to connect.
	if(mqtt.connect(id.c_str()))
		mqtt.subscribe(REQUEST_TOPIC);			//Once connected, resubscribe.

	else
		delay(1000);
}
