#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#include <ArduinoJSON.h>
#include <LittleFS.h>

struct WIFI_SETTINGS_T{
	char ssid[16];
	char pass[16];
};

AsyncWebServer server(80);

void setup(){
	Serial.begin(9600);

	pinMode(LED_BUILTIN, OUTPUT);

	if(!LittleFS.begin()){
    Serial.println("An error has occurred while mounting LittleFS; resetting...");
    ESP.restart();
  }

	WIFI_SETTINGS_T settings;

	File file = LittleFS.open("/settings.bin", "r");

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

	//Prima scansione asincrona.
	WiFi.scanNetworks(true);

	//Scansione delle reti WiFi nei dintorni.
	server.on("/networks", HTTP_GET, [](AsyncWebServerRequest *req){
		DynamicJsonDocument json(1024);

		//Recupera i risultati della scansione precedente.
		int n = WiFi.scanComplete();

		if(n == WIFI_SCAN_FAILED || n == WIFI_SCAN_RUNNING)
			req->send(200, "application/json", "null");

		else{
			for(int i=0; i<n; i++){
				json[i]["ssid"] = WiFi.SSID(i);
				json[i]["rssi"] = WiFi.RSSI(i) * (-1);
			}

			WiFi.scanDelete();
			WiFi.scanNetworks(true);

			String res;
			serializeJson(json, res);

			req->send(200, "application/json", res);
		}
	});

	//Set delle impostazioni del WiFi.
	server.on("/wifi", HTTP_POST, [](AsyncWebServerRequest *req){
		WIFI_SETTINGS_T settings;

		strcpy(settings.ssid, req->arg("ssid").c_str());
		strcpy(settings.pass, req->arg("pass").c_str());

		File file = LittleFS.open("/settings.bin", "w");
		file.write((uint8_t *) &settings, sizeof(WIFI_SETTINGS_T));
		file.close();

		req->send(200);
		ESP.restart();
	});

	server.serveStatic("/", LittleFS, "/public/");
	server.begin();
}

void loop(){
}
