/*
 * lora.c
 *
 *  Created on: 20. 1. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#include "lora.h"
#include "main.h"
#include "../lmic/lmic.h"
#include "../display/display.h"

#include "system_config.h"

void os_getDevEui (uint8_t* buf) { memcpy(buf, DEVEUI, 8);}
void os_getArtEui (uint8_t* buf) { memcpy(buf, APPEUI, 8);}
void os_getDevKey (uint8_t* buf) { memcpy(buf, APPKEY, 16);}


static uint8_t send_data[] = "{\"lat\": 19.0000, \"lng\": 49.0000}";
static uint8_t send_length = 6;
static osjob_t sendjob;

GPS_data *gps_data;

/*
 * Message with data was sent to LoRa
 */
void dataSentComplete(){
	display_clear();
	Serial_DEBUG("EV_TXCOMPLETE (includes waiting for RX windows)\r\n");
	Display_DEBUG_N("EV_TXCOMPLETE",0);

#ifdef LORA_WAIT_ACK	//if we are waiting for acknowlidgement with signal information
	Serial_DEBUG("RSSI - "); Serial_DEBUG(Int_to_Char(LMIC.rssi));
	Serial_DEBUG("  SNR - "); Serial_DEBUG(Int_to_Char(LMIC.snr));

	Display_DEBUG_N("RSSI ",1); Display_DEBUG_N(Int_to_Char(LMIC.rssi), 1);
	Display_DEBUG_N("SNR: ",2); Display_DEBUG_N(Int_to_Char(LMIC.snr), 2);
#endif

	Serial_DEBUG("\r\n");

    if (LMIC.txrxFlags & TXRX_ACK) {    //received ACK (first window)
    	Serial_DEBUG("Received ack ");
    }

    if (LMIC.dataLen) {     			//received new data from TTN (second window)
        char TTN_response[LMIC.dataLen+1];
        memcpy(TTN_response, LMIC.frame, LMIC.dataLen);
        TTN_response[LMIC.dataLen] = '\0';

        Serial_DEBUG("Data Received:");
        Serial_DEBUG((const char*)TTN_response);
        Serial_DEBUG("\r\n");
        Display_DEBUG_N(TTN_response,3);
    }
}

/*
 * Send new message to LoRaWAN network
 */
void do_send(osjob_t* j)
{
	LED_ON();								//indicate sending LED

	GPS_data gps_new;
	gps_read(&huart1, &gps_new);			// read GPS position
	gps_debug(&gps_new);

	if(gps_new.fixed == 1){					// found GPS position
		if(gps_new.latitudeD == gps_data->latitudeD && gps_new.longitudeD == gps_data->longitudeD){	// GPS position is same as last time
			os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), *do_send);			// plan next sending on TX_INTERVAL
			return;
		}
		else{			//new position differs from last one
			memcpy(gps_data, &gps_new, sizeof(GPS_data));
		}

		// prepare message with location
		send_data[0] = gps_data->latitude;
		send_data[1] = ((uint8_t*)&gps_data->latitudeD)[1];
		send_data[2] = ((uint8_t*)&gps_data->latitudeD)[0];
		send_data[3] = gps_data->longitude;
		send_data[4] = ((uint8_t*)&gps_data->longitudeD)[1];
		send_data[5] = ((uint8_t*)&gps_data->longitudeD)[0];
	}
	else{														// GPS position not available
		Serial_DEBUG("GPS position not available\r\n");
		memset(send_data, 1, send_length*sizeof(uint8_t));		//send empty data 1
	}

    if (LMIC.opmode & OP_TXRXPEND) {		// LoRa is bussy
    	Serial_DEBUG("OP_TXRXPEND, not sending\r\n");
    } else {								// sending data
        LMIC_setTxData2(1, send_data, send_length, LORA_WAIT_ACK);
        Serial_DEBUG("Sending data\r\n");
    }
}

/*
 * Set sending data to lora
 */
void lora_send(GPS_data *gps)
{
	//send_length = stringLength(send_data);

	gps_data = gps;

	do_send(&sendjob);
}

/*
 * Init LMIC and LoRa module
 * Connect to LoRaWAN network
 */
void lora_init(){
	  os_init();  	 // LMIC init

	  LMIC_reset();

	  do_send(&sendjob);  // start up also the join
}

/*
 * New LoRa event
 */
void onEvent (ev_t ev) {
    //Serial.print(os_getTime());
    switch(ev) {
        case EV_TXCOMPLETE:
            dataSentComplete();

            LED_OFF();
            // Schedule next transmission
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), *do_send);
            break;

        case EV_JOINING:	// joining in progress
        	Serial_DEBUG("EV_JOINING: -> Joining...\r\n");
            break;

        case EV_JOINED:		// joined to network
        	Serial_DEBUG("EV_JOINED\r\n");
        	Display_DEBUG("JOINED");
            LMIC_setLinkCheckMode(0);
            break;

        case EV_RXCOMPLETE:	// receiving completed
        	Serial_DEBUG("EV_RXCOMPLETE\r\n");
            break;

        case EV_LINK_DEAD:
        	Serial_DEBUG("EV_LINK_DEAD\r\n");
            break;

        case EV_LINK_ALIVE:
        	Serial_DEBUG("EV_LINK_ALIVE\r\n");
            break;

        case EV_TXSTART:	// started transmitting
        	Serial_DEBUG("EV_TXSTART\r\n");
        	Display_DEBUG("EV_TXSTART");
        	break;

         default:
        	Serial_DEBUG("EV_Unknown: ");
        	Serial_DEBUG(Int_to_Char(ev));
            Serial_DEBUG("\r\n");
            break;
    }
}


void lora_loop()
{
	os_runloop_once();	// look for tasks
}
