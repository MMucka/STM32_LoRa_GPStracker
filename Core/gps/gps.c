/*
 * gps.h
 *
 *  Created on: 28. 5. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#include "gps.h"

#include <stdio.h>
#include <string.h>
#include "debug.h"

/*
 * Read from Serial UART block of 255 characters and try to find there location
 */
void gps_read(UART_HandleTypeDef *uart, GPS_data* gps)
{
	uint8_t input[BUFFER_SIZE];
	uint8_t n = 0;

	HAL_UART_AbortReceive(uart);	//clear past RXs
	HAL_UART_Receive(uart, input, BUFFER_SIZE-1, 1000);	//read 255 characters from input
	for(uint8_t i=0; i<BUFFER_SIZE-1; i++){
		if(input[i] == '$'){
			for(n=0; input[i+n] != '\r' && i+n < BUFFER_SIZE-1; n++);			//count one line of NMEA
			if(gps_parse(gps, (char*)input+i, n) == 1){			//fixed, found location
				gps->fixed = 1;
				return;
			}
			i+=n;
		}
	}
	gps->fixed = 0;

}

void gps_debug(GPS_data* gps)
{
	Serial_DEBUG("Latitude: ");
	Serial_DEBUG(Int_to_Char(gps->latitude));
	Serial_DEBUG(".");
	Serial_DEBUG(Int_to_Char(gps->latitudeD));
	Serial_DEBUG("\r\nLongitude: ");
	Serial_DEBUG(Int_to_Char(gps->longitude));
	Serial_DEBUG(".");
	Serial_DEBUG(Int_to_Char(gps->longitudeD));
	Serial_DEBUG("\r\n");
}

/*
 * NMEA sentences:  https://www.gpsinformation.org/dale/nmea.htm#nmea
 *                  http://navspark.mybigcommerce.com/content/NMEA_Format_v0.1.pdf
 */
uint8_t gps_parse(GPS_data* gps, char* cmd, uint16_t n)
{
	char ns, status;
	int latitude, longitude;
	int latitudeD, longitudeD;
	int timefix, timefixD;

	// Global Positioning System Fix Data
	if(strncmp(cmd,"$GPGGA", 6) == 0)
	{
		//$GPGGA,213342.00,4912.76326,N,01635.61162,E,1,04,3.13,256.7,M,42.4,M,,*53
		if(sscanf(cmd, "$GPGGA,%d.%d,%d.%d,%c,%d.%d", &timefix, &timefixD, &latitude, &latitudeD, &ns, &longitude, &longitudeD) == 7){ //found location
			gps->latitude = latitude;
			gps->latitudeD = latitudeD;
			gps->longitude = longitude;
			gps->longitudeD = longitudeD;
			gps->fixed = 1;
			return 1;
		}
	}

	else if(strncmp(cmd,"$GPGLL", 6) == 0)
	{
		//$GPGLL,4912.76297,N,01635.61177,E,213341.00,A,A*6B
		if(sscanf(cmd, "$GPGLL,%d.%d,%c,%d.%d", &latitude, &latitudeD, &ns, &longitude, &longitudeD) == 5){
			gps->latitude = latitude;
			gps->latitudeD = latitudeD;
			gps->longitude = longitude;
			gps->longitudeD = longitudeD;
			gps->fixed = 1;
			return 1;
		}
	}

	else if(strncmp(cmd,"$GPRMC", 6) == 0)
	{
		//$GPRMC,213342.00,A,4912.76326,N,01635.61162,E,0.203,,280720,,,A*70
		if(sscanf(cmd, "$GPRMC,%d.%d,%c,%d.%d,%c,%d.%d", &timefix, &timefixD, &status, &latitude, &latitudeD, &ns, &longitude, &longitudeD) == 8){
			gps->latitude = latitude;
			gps->latitudeD = latitudeD;
			gps->longitude = longitude;
			gps->longitudeD = longitudeD;
			gps->fixed = 1;
			return 1;
		}
	}

	else if(strncmp(cmd,"$GNGGA", 6) == 0)
	{
		//$GPGGA,213342.00,4912.76326,N,01635.61162,E,1,04,3.13,256.7,M,42.4,M,,*53
		if(sscanf(cmd, "$GNGGA,%d.%d,%d.%d,%c,%d.%d", &timefix, &timefixD, &latitude, &latitudeD, &ns, &longitude, &longitudeD) == 7){ //found location
			gps->latitude = latitude;
			gps->latitudeD = latitudeD;
			gps->longitude = longitude;
			gps->longitudeD = longitudeD;
			gps->fixed = 1;
			return 1;
		}
	}

	else if(strncmp(cmd,"$GNGLL", 6) == 0)
	{
		//$GPGLL,4912.76297,N,01635.61177,E,213341.00,A,A*6B
		if(sscanf(cmd, "$GNGLL,%d.%d,%c,%d.%d", &latitude, &latitudeD, &ns, &longitude, &longitudeD) == 5){
			gps->latitude = latitude;
			gps->latitudeD = latitudeD;
			gps->longitude = longitude;
			gps->longitudeD = longitudeD;
			gps->fixed = 1;
			return 1;
		}
	}


	return 0;
}
