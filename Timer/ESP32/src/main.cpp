// Esempio di utilizzo dei timer su ESP32.

#include <Arduino.h>

void IRAM_ATTR onTimer();	//ISR Interrupt: IRAM_ATTR indica che deve risiedere in RAM e non in FLASH.

hw_timer_t *timer;

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);

	timer = timerBegin(0, 10000, true);						//Parametri:	Timer (da 0 a 3), Prescaler a 16 bit, direzione conteggio.
	timerAttachInterrupt(timer, &onTimer, true);	//L'ultimo parametro indica se lavorare in
																								//modalità edge (true) o in modalità level (false).

	timerWrite(timer, 0);													//Contatore a 64 bit.
	timerAlarmWrite(timer, 8000, true);						//Parametri:	Puntatore al timer, Autoreload a 64 bit, reset automatico.
	timerAlarmEnable(timer);
}

void loop(){
}

//ISR overflow timer 0.
void IRAM_ATTR onTimer(){
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}
