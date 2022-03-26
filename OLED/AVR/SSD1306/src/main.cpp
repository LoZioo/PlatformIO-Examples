#include <Arduino.h>
#include <U8glib.h>

void draw();

U8GLIB_SSD1306_128X64 display(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);	// I2C / TWI 

const unsigned char bitmap [] U8G_PROGMEM = {
	0x00, 0x3f, 0xf8, 0x00, 0x00, 0x1f, 0xf8, 0x00, 0x01, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0x00, 
	0x03, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xf0, 
	0x1f, 0xe0, 0x0f, 0xf8, 0x1f, 0xe0, 0x07, 0xf8, 0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 
	0x7f, 0x00, 0x00, 0xfe, 0x7e, 0x00, 0x00, 0xfe, 0x7e, 0x00, 0x00, 0x3f, 0xfc, 0x01, 0x00, 0x7e, 
	0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x03, 0x80, 0x3f, 0xf8, 0x00, 0x00, 0x0f, 0xf0, 0x03, 0x80, 0x1f, 
	0xf8, 0x1f, 0xf8, 0x07, 0xe0, 0x1f, 0xf0, 0x1f, 0xf8, 0x1f, 0xf0, 0x0f, 0xe0, 0x1f, 0xf0, 0x1f, 
	0xf8, 0x00, 0x00, 0x1f, 0xf0, 0x03, 0x80, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 0xf8, 0x03, 0x80, 0x3e, 
	0x7c, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7e, 0x7e, 0x00, 0x00, 0xfe, 0x7e, 0x00, 0x00, 0xfc, 
	0x3f, 0x80, 0x01, 0xfc, 0x3f, 0x80, 0x01, 0xfc, 0x1f, 0xe0, 0x07, 0xf8, 0x1f, 0xe0, 0x07, 0xf8, 
	0x0f, 0xff, 0xff, 0xf0, 0x0f, 0xfe, 0x3f, 0xf0, 0x07, 0xff, 0xff, 0xc0, 0x03, 0xff, 0xff, 0xc0, 
	0x01, 0xff, 0xff, 0x80, 0x00, 0xff, 0xff, 0x80, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x3f, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x0c, 0x1f, 0x0f, 0x84, 0x33, 0xf1, 0x84, 0x7c, 0x1e, 0x1b, 0x9d, 0xc6, 0x30, 0xe1, 0xcc, 0xee, 
	0x1e, 0x19, 0xdc, 0x66, 0x30, 0xc1, 0xcc, 0xc6, 0x33, 0x1f, 0x9c, 0x66, 0x30, 0xc1, 0xed, 0xc3, 
	0x33, 0x1f, 0x1c, 0x66, 0x30, 0xc1, 0xbd, 0xc3, 0x3f, 0x99, 0x9c, 0x66, 0x30, 0xc1, 0xbc, 0xc6, 
	0x61, 0x99, 0x9d, 0xc6, 0x70, 0xe1, 0x9c, 0xee, 0x61, 0x98, 0xcf, 0x03, 0xc3, 0xf9, 0x0c, 0x78
};

void setup() {
	Serial.begin(9600);
  display.begin();
}

void loop(){
	display.firstPage();

	do draw();
	while(display.nextPage());

	delay(1000);
}

void draw(){
	display.setFont(u8g_font_unifont);
	display.drawStr(0, 10, "Bel display!");

	display.drawBitmapP(32, 24, 8, 32, bitmap);
}