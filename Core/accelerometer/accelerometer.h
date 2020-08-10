/*
 * accelerometer.h
 *
 *  Created on: 14. 7. 2020
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#ifndef ACCELEROMETER_ACCELEROMETER_H_
#define ACCELEROMETER_ACCELEROMETER_H_

#include "sd_hal_mpu6050.h"

void acc_init();

SD_MPU6050 acc_read();


#endif /* ACCELEROMETER_ACCELEROMETER_H_ */
