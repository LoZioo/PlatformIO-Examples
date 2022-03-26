#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

#include <password.h>

AsyncWebServer server(80);

void setup(void){
  Serial.begin(9600);
	Serial.print("\n\n\n");

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASS);

	Serial.print("Connecting to ");
	Serial.print(SSID);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
	
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hi! I am ESP8266.");
  });

  AsyncElegantOTA.begin(&server);
  server.begin();

  Serial.println("HTTP server started!");
}

void loop(void){
  AsyncElegantOTA.loop();
}
