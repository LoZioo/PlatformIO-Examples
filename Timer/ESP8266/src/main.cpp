/*
	Esempio di utilizzo del timer1 su ESP8266.

	Il timer0 non può essere utilizzato in quanto è riservato alla gestione interna del WiFI (e forse anche di altro).
	La lista di funzioni messe a disposizione dall'API ESP8266 è disponibile dentro Arduino.h.
*/

#include <Arduino.h>

void IRAM_ATTR onTimerISR();	//ISR Interrupt: IRAM_ATTR indica che deve risiedere in RAM e non in FLASH.

void setup(){
	pinMode(LED_BUILTIN, OUTPUT);
	
  timer1_attachInterrupt(onTimerISR);

	/*
		Autoreload (ARR) del timer1:
			ARR = (80.000.000 / PRE) / Freq. voluta
			In questo caso: Freq. voluta = 4Hz.
			
			max(ARR) = 8.388.607
	*/
	timer1_write(78125);

	/*
		Abilita il timer.
		
		Parametri (ESP8266 freq. CPU: 80MHz):
			Prescaler (PRE): TIM_DIV1, TIM_DIV16, TIM_DIV256.
			Modalità di funzionamento: TIM_EDGE; questa dovrebbe essere la sola modalità supportata dal timer1 (overflow).
			Clear del timer: TIM_SINGLE, TIM_LOOP:
				TIM_SINGLE non resetta il counter del timer alla fine dell'esecuzione dell'ISR.
				TIM_LOOP resetta il counter del timer automaticamente alla fine dell'esecuzione dell'ISR.
	*/
	timer1_enable(TIM_DIV256, TIM_EDGE, TIM_LOOP);
}

void loop(){
}

//ISR overflow timer1.
void IRAM_ATTR onTimerISR(){
	digitalWrite(LED_BUILTIN, !(digitalRead(LED_BUILTIN)));
}
