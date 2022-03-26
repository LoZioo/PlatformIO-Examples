/*
	Implementazione sincrona delle WebSocket.
	WebSockets by Markus Sattler.
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>
#include <WebSockets4WebServer.h>

ESP8266WebServer server(80);
WebSockets4WebServer ws;

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);

	WiFi.mode(WIFI_STA);
	WiFi.begin("STASSID", "STAPSK");

	while(WiFi.status() != WL_CONNECTED){
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
		delay(500);
	}

	digitalWrite(LED_BUILTIN, HIGH);

	/*
		URI: ws://indirizzo/ws

		id:				ID Client che ha scatenato l'evento; serve in seguito per rispondere.
		WStype:		Evento WebSocket (connessione, messaggio ecc ecc...).
	*/
	server.addHook(ws.hookForWebserver("/ws", [](uint8_t id, WStype_t type, uint8_t *payload, size_t length){
		char *text = (char *) payload;

		switch(type){
			case WStype_CONNECTED:
				Serial.printf("Client %d connesso\n", id);
				break;

			case WStype_DISCONNECTED:
				Serial.printf("Client %d disconnesso\n", id);
				break;

			case WStype_TEXT:
				//Caso in cui si riceve un messaggio testuale: rispondo con Ciao!
				ws.sendTXT(id, "Ciao!");
				break;

			default:
				break;
    }
	}));

	server.begin();
}

void loop(){
	server.handleClient();
	ws.loop();
}
