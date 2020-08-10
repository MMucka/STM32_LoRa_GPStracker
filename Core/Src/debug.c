/*
 * debug.c
 *
 *  Created on: 28. 7. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */


#include "debug.h"
#include "main.h"
#include "system_config.h"
#include <stdint.h>

char* Int_to_Char(int val){
	static char buf[30];
    char num[30], *p = num, *b = buf;
    uint32_t m, v;
    *p++='\0';

    v = (val < 0) ? -val : val;	// special handling of negative decimals

    do {						// generate digits backwards
        *p++ = ((m=v % 10 ) <= 9) ? m+'0' : m+'A'-10;
    } while( v /= 10 );

    if(val < 0) {				// prefix negative decimals with '-'
        *p++ = '-';
    }

    do *b++ = *--p;			// copy digits and sign forwards
    while( b-buf < 10 && p > num );

    return buf;
}


void Serial_DEBUG(const char* string){
#ifdef Serial_DEBUG_ENABLE
	for(; *string != '\0'; string++){
		HAL_UART_Transmit(&huart1, (uint8_t*)string, 1, HAL_MAX_DELAY);
	}
#endif
}

void Display_DEBUG(const char* message){
#ifdef LCD_ENABLE
	display_writeln(message);
#endif
}

void Display_DEBUG_N(const char* message, int line){
#ifdef LCD_ENABLE
	display_writeString(line, message);
#endif
}
