#include <Arduino.h>
#include <IRremote.h>

int IR_RECEIVE_PIN = 11;

void setup(){
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

	IrReceiver.sendSamsung()
}

void loop(){
  if(IrReceiver.decode()){
    IrReceiver.printIRResultShort(&Serial);
    IrReceiver.resume();
  }

	IrSender.sendSony()
}

// Soundbar
// ON
// Protocol=SONY Address=0x30 Command=0x15 Raw-Data=0x1815 15 bits LSB first
// Protocol=SONY Address=0x30 Command=0x15 Repeat gap=21600us Raw-Data=0x1815 15 bits LSB first

// +
// Protocol=SONY Address=0x30 Command=0x12 Raw-Data=0x1812 15 bits LSB first
// Protocol=SONY Address=0x30 Command=0x12 Repeat gap=22200us Raw-Data=0x1812 15 bits LSB first

// -
// Protocol=SONY Address=0x30 Command=0x13 Raw-Data=0x1813 15 bits LSB first
// Protocol=SONY Address=0x30 Command=0x13 Repeat gap=21650us Raw-Data=0x1813 15 bits LSB first

//TV
// ON
// Protocol=SAMSUNG Address=0x707 Command=0x2 Raw-Data=0xFD020707 32 bits LSB first

// +
// Protocol=SAMSUNG Address=0x707 Command=0x7 Raw-Data=0xF8070707 32 bits LSB first

// -
// Protocol=SAMSUNG Address=0x707 Command=0xB Raw-Data=0xF40B0707 32 bits LSB first

// CH+
// Protocol=SAMSUNG Address=0x707 Command=0x12 Raw-Data=0xED120707 32 bits LSB first

// CH-
// Protocol=SAMSUNG Address=0x707 Command=0x10 Raw-Data=0xEF100707 32 bits LSB first
