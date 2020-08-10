/*
 * gps.h
 *
 *  Created on: 28. 5. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#ifndef GPS_GPS_H_
#define GPS_GPS_H_

#include <stdint.h>
#include "main.h"

#define BUFFER_SIZE 	256

typedef struct GPS_data{
	uint8_t fixed;

	uint8_t latitude;
	uint16_t latitudeD;
	uint8_t longitude;
	uint16_t longitudeD;

}GPS_data;



void gps_init(GPS_data* gps);
uint8_t gps_parse(GPS_data* gps, char* input, uint16_t length);

void gps_read(UART_HandleTypeDef* uart, GPS_data* gps);
void gps_debug(GPS_data* gps);



#endif /* GPS_GPS_H_ */
