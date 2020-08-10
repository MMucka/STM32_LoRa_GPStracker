/*
 * system_config.h
 *
 *  Created on: Jun 20, 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#ifndef INC_SYSTEM_CONFIG_H_
#define INC_SYSTEM_CONFIG_H_


void system_init();

// CONFIGURATION OF SYSTEM
#define GPS_ENABLE 		1
#define UART_SPEED 		9600
#define LCD_ENABLE 		0
#define LED_ENABLE 		1
#define TX_INTERVAL		20
#define ACC_ENABLE		1
#define LORA_ENABLE		1

#define Serial_DEBUG_ENABLE 1

//#define ACC_INTERRUPT	1

#define LORA_INTERRUPT	1
#define LORA_WAIT_ACK	0
#define HF_FREQUENCY 	1

void LED_ON();
void LED_OFF();

#endif /* INC_SYSTEM_CONFIG_H_ */
