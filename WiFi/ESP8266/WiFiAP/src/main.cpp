#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include <ArduinoJSON.h>
#include <LittleFS.h>

#define VSCODE_LIVESERVER_PORT 5500

struct WIFI_SETTINGS_T{
	char ssid[16];
	char pass[16];
};

ESP8266WebServer server(80);

void setup(){
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

	if(!LittleFS.begin()){
    Serial.println("An error has occurred while mounting LittleFS; resetting...");
    ESP.restart();
  }

	ESPhttpUpdate.setLedPin(LED_BUILTIN, LOW);

	WIFI_SETTINGS_T settings;
	File file = LittleFS.open("/settings.bin", "r");

	//Tento di leggere le impostazioni del WiFi dal file se questo esiste.
	if(file){
		file.read((uint8_t *) &settings, sizeof(WIFI_SETTINGS_T));
		file.close();
	}
	else{
		strcpy(settings.ssid, "STASSID");
		strcpy(settings.pass, "STAPSK");
	}

	Serial.printf("\nConnecting to %s (pass: %s)", settings.ssid, settings.pass);

	WiFi.mode(WIFI_STA);
	WiFi.begin(settings.ssid, settings.pass);

	//Tentativo di connessione al WiFi salvato nelle configurazioni.
	int attempt = 0;
	while(WiFi.status() != WL_CONNECTED && attempt < 10){
		Serial.print('.');

		digitalWrite(LED_BUILTIN, LOW);
		delay(500);

		digitalWrite(LED_BUILTIN, HIGH);
		delay(500);

		attempt++;
  }
	Serial.println();
	
	//Se non riesco a connettermi.
	if(attempt >= 10){
		Serial.printf("Connection to %s failed...\n", settings.ssid);
		WiFi.disconnect();

		//Apertura dell'hotspot.
		Serial.println(WiFi.softAP("ESPsoftAP_01") ? "ESPsoftAP_01 Hotspot started!" : "Failed starting hotspot...");
		Serial.printf("Soft-AP IP address: %s\n", WiFi.softAPIP().toString().c_str());

		digitalWrite(LED_BUILTIN, LOW);
	}
	else
		Serial.printf("Connected to %s! IP: %s;\n", settings.ssid, WiFi.localIP().toString().c_str());

	//Scansione delle reti WiFi nei dintorni.
	server.on("/networks", HTTP_GET, [](){
		digitalWrite(LED_BUILTIN, LOW);

		DynamicJsonDocument json(1024);
		int n = WiFi.scanNetworks();

		for(int i=0; i<n; i++){
			json[i]["ssid"] = WiFi.SSID(i);
			json[i]["rssi"] = WiFi.RSSI(i) * (-1);
		}

		String res;
		serializeJson(json, res);

		server.send(200, "application/json", res);

		digitalWrite(LED_BUILTIN, HIGH);
	});

	//Set delle impostazioni del WiFi.
	server.on("/wifi", HTTP_POST, [](){
		WIFI_SETTINGS_T settings;

		strcpy(settings.ssid, server.arg("ssid").c_str());
		strcpy(settings.pass, server.arg("pass").c_str());

		File file = LittleFS.open("/settings.bin", "w");
		file.write((uint8_t *) &settings, sizeof(WIFI_SETTINGS_T));
		file.close();

		server.send(200);

		delay(500);
		ESP.restart();
	});

	//Aggiornamento FW.
	server.on("/update/fw", HTTP_GET, [](){
		server.send(200, "text", "Updating firmware...");

		WiFiClient client;
		String uri = "http://" + server.client().remoteIP().toString() + ':' + VSCODE_LIVESERVER_PORT + "/firmware.bin";
		
		ESPhttpUpdate.update(client, uri);
	});

	//Aggiornamento FS.
	server.on("/update/fs", HTTP_GET, [](){
		server.send(200, "text", "Updating LittleFS image...");

		WiFiClient client;
		String uri = "http://" + server.client().remoteIP().toString() + ':' + VSCODE_LIVESERVER_PORT + "/littlefs.bin";

		ESPhttpUpdate.updateFS(client, uri);
	});

	//Per il download del file di configurazione.
	server.serveStatic("/wifi/settings.bin", LittleFS, "/settings.bin");
	server.serveStatic("/", LittleFS, "/public/");
	
	server.begin();
}

void loop(){
	server.handleClient();
}
