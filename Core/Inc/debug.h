/*
 * debug.h
 *
 *  Created on: 28. 7. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#ifndef INC_DEBUG_H_
#define INC_DEBUG_H_


char* Int_to_Char(int val);
void Serial_DEBUG(const char* string);
void Display_DEBUG(const char* message);
void Display_DEBUG_N(const char* message, int line);


#endif /* INC_DEBUG_H_ */
