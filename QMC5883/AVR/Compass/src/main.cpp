#include <Arduino.h>
#include <U8g2lib.h>
#include <QMC5883LCompass.h>

#define WIDTH 128
#define HEIGHT 64

U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
// U8G2_SSD1306_128X64_ALT0_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   //same as the NONAME variant, but may solve the "every 2nd line skipped" problem

QMC5883LCompass compass;

void setup(){
	display.begin();
  compass.init();

	// compass.setCalibration(-368, 1976, -1278, 1122, -655, 1793);
	// compass.setCalibration(-265, 1941, -1317, 1045, -616, 1781);
	// compass.setCalibration(-366, 2023, -1371, 1155, -675, 1728);
	// compass.setCalibration(-361, 2016, -1341, 1106, -675, 1728);
	// compass.setCalibration(-443, 2033, -1358, 1155, -697, 1793);

	compass.setCalibration(-443, 2033, -1358, 1155, -697, 1793);
}

void loop(){
	compass.read();

	display.clearBuffer();
	// display.setFont(u8g2_font_unifont_t_latin);
  display.setFont(u8g2_font_profont12_tf);

	display.drawStr(WIDTH/2 - 2, HEIGHT/2 - 16, "N");
	display.drawStr(WIDTH/2 - 2, HEIGHT/2 + 26, "S");
	display.drawStr(WIDTH/2 + 18, HEIGHT/2 + 4, "E");
	display.drawStr(WIDTH/2 - 22, HEIGHT/2 + 4, "W");

	float x = compass.getAzimuth() * PI / 180;

	display.drawCircle(WIDTH/2, HEIGHT/2, 16);
	display.drawLine(WIDTH/2, HEIGHT/2, WIDTH/2 + 15*sin(x), HEIGHT/2 - 15*cos(x));

  display.sendBuffer();
  delay(125);
}
