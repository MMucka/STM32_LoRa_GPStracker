/*
 * display.h
 *
 *  Created on: Jun 20, 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include <stdint.h>

void display_init();
void display_writeln(const char*);
void display_writeString(int n, char* text);
void display_clear();
void display_off();

uint8_t stringLength(uint8_t* text);

#endif /* INC_DISPLAY_H_ */
