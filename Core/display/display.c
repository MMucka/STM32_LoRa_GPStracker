/*
 * display.c
 *
 *  Created on: Jun 20, 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#include "display.h"
#include "ssd1306.h"

#include "system_config.h"


void display_init()
{
#ifdef LCD_ENABLE
	ssd1306_Init();

	ssd1306_Fill(Black);
	ssd1306_SetCursor(2, 0);
	ssd1306_WriteString("GPS Tracker", Font_7x10, White);
	ssd1306_SetCursor(2, 26);
	ssd1306_WriteString("LoRaWAN", Font_7x10, White);
	ssd1306_SetCursor(2, 26+18);
	ssd1306_WriteString("FIT VUTBR", Font_7x10, White);
	ssd1306_SetCursor(2, 26+18+10);
	ssd1306_WriteString("Milan Mucka", Font_7x10, White);
	ssd1306_UpdateScreen();
#endif
}

uint8_t stringLength(uint8_t* text)
{
	uint8_t i=0;
	for(; text[i]!= '\0'; i++);

	return i;
}

void display_clear(){
#ifdef LCD_ENABLE
	ssd1306_Fill(Black);
	ssd1306_UpdateScreen();
#endif

}


void display_writeString(int n, char* text){
#ifdef LCD_ENABLE
	ssd1306_SetCursor(2, n*10);
	ssd1306_WriteString(text, Font_7x10, White);
	ssd1306_UpdateScreen();
#endif

}

void display_writeln(const char* text){
#ifdef LCD_ENABLE
	ssd1306_Fill(Black);

	uint8_t n = stringLength((uint8_t*)text);
	for(int i=0; i*18 < n; i++){
		display_writeString(i, (char*) text+(i*18));
	}
#endif
}


void display_off(){
#ifdef LCD_ENABLE
	ssd1306_WriteCommand(0xAE);
#endif

}
