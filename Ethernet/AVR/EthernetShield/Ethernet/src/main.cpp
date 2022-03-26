#include <Arduino.h>
#include <Ethernet.h>

#define UDP_LOCAL_PORT 5000

//Un MAC Address a caso, basta che non sia uguale a qualcun'altro in rete.
uint8_t mac[] = { 0x4E, 0x89, 0x33, 0x9E, 0x67, 0x27 };

//Socket UDP.
EthernetUDP sock;

void setup(){
	Serial.begin(9600);

	//Disabilito il CS della scheda SD.
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);

	//CS ethernet su pin 10.
	Ethernet.init(10);

	//MAC Address.
	Ethernet.begin(mac);

	//Socket in ascolto sulla porta specificata.
	sock.begin(UDP_LOCAL_PORT);

	Serial.print("Socket UDP in ascolto su ");
	Serial.print(Ethernet.localIP());
	Serial.print(':');
	Serial.print(sock.localPort());
	Serial.println('.');
}

void loop(){
	//Se ci sono pacchetti da leggere nel buffer della scheda di rete; la funzione non è bloccante.
	if(sock.parsePacket()){
		uint8_t val;
		sock.read(&val, sizeof(val));

		Serial.print("Action: ");
		Serial.print(val);
		Serial.println('.');

		val++;

		sock.beginPacket(sock.remoteIP(), sock.remotePort());
		sock.write(&val, sizeof(val));
		sock.endPacket();
	}
}
