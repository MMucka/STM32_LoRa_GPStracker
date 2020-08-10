/*
 * wiring_time.h
 *
 *  Created on: Jun 16, 2020
 *      Author: Intrak
 */

#ifndef LMIC_HAL_WIRING_TIME_H_
#define LMIC_HAL_WIRING_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


void delay(uint32_t ms);
void delayMicroseconds(uint32_t mms);

uint32_t micros(void);
uint32_t millis(void);

#ifdef __cplusplus
}
#endif



#endif /* LMIC_HAL_WIRING_TIME_H_ */
