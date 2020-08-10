/*
 * main.c
 *
 *  Created on: 28. 1. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#include "main.h"
#include "system_config.h"

#include "debug.h"
#include "lora.h"
#include "../display/display.h"
#include "../accelerometer/accelerometer.h"
#include "../gps/gps.h"


I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart1;
SPI_HandleTypeDef hspi1;

GPS_data gps;


int main(void)
{
	system_init();
	LED_ON();

#ifdef LCD_ENABLE
	display_init();
#endif

#ifdef ACC_ENABLE
	acc_init();
#endif

#ifdef LORA_ENABLE
	lora_init();
#endif

#ifdef GPS_ENABLE
	gps_read(&huart1, &gps);
#endif

	LED_OFF();

	lora_send(&gps);

	for (long i=0; ; i++)
	{
		lora_loop();
	}
}

void Error_Handler(void)
{
	Serial_DEBUG("ERROR HANDLER!\r\n");
}
