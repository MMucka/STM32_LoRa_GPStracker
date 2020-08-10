/*
 * accelerometer.c
 *
 *  Created on: 14. 7. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#include "sd_hal_mpu6050.h"
#include "main.h"
#include "debug.h"

static SD_MPU6050 mpu1;		//accelerometer

void acc_init(){
	SD_MPU6050_Result result ;

	result = SD_MPU6050_Init(&hi2c1,&mpu1,SD_MPU6050_Device_0,SD_MPU6050_Accelerometer_2G,SD_MPU6050_Gyroscope_250s );
	HAL_Delay(500);

	if(result == SD_MPU6050_Result_Ok)
	{
		Serial_DEBUG("MPU WORKING\r\n");
		SD_MPU6050_PowerSave(&hi2c1, &mpu1, 0);
		SD_MPU6050_EnableInterrupts(&hi2c1, &mpu1);
		//SD_MPU6050_SetDataRate(&hi2c1, &mpu1, SD_MPU6050_DataRate_100Hz);
	}
	else
	{
		Serial_DEBUG("MPU NOT WORKING\r\n");
	}

}


void acc_read()
{
	char output[64];

	SD_MPU6050_ReadAll(&hi2c1,&mpu1);

	float temper = mpu1.Temperature;
	Serial_DEBUG(Int_to_Char((uint8_t)temper));

	SD_MPU6050_ReadGyroscope(&hi2c1,&mpu1);

	int16_t g_x = mpu1.Gyroscope_X;
	int16_t g_y = mpu1.Gyroscope_Y;
	int16_t g_z = mpu1.Gyroscope_Z;
	sprintf(output, "G %d %d %d\r\n", g_x, g_y, g_z);

	Serial_DEBUG(output);
	Display_DEBUG_N(output, 1);

	int16_t a_x = mpu1.Accelerometer_X;
	int16_t a_y = mpu1.Accelerometer_Y;
	int16_t a_z = mpu1.Accelerometer_Z;
	sprintf(output, "A %d %d %d\r\n", a_x, a_y, a_z);

	Serial_DEBUG(output);
	Display_DEBUG_N(output, 2);
}

