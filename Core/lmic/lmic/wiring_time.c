/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "wiring_time.h"
#include "../../Inc/stm32l0xx_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t millis(void)
{
  // ToDo: ensure no interrupts
	return HAL_GetTick();
}

// Interrupt-compatible version of micros
uint32_t micros(void)
{
	/* Ensure COUNTFLAG is reset by reading SysTick control and status register */
	//LL_SYSTICK_IsActiveCounterFlag();
	uint32_t m = HAL_GetTick();
	const uint32_t tms = SysTick->LOAD + 1;
	__IO uint32_t u = tms - SysTick->VAL;
	/*if (LL_SYSTICK_IsActiveCounterFlag()) {
	m = HAL_GetTick();
	u = tms - SysTick->VAL;
	}*/
	return (m * 1000 + (u * 1000) / tms);
}

void delay(uint32_t ms)
{
  if (ms != 0) {
    uint32_t start = millis();
    do {
      //yield();
    } while (millis() - start < ms);
  }
}

void delayMicroseconds(uint32_t ms)
{
  if (ms != 0) {
    uint32_t start = micros();
    do {
      //yield();
    } while (micros() - start < ms);
  }
}

#ifdef __cplusplus
}
#endif
