/*******************************************************************************
 * Copyright (c) 2015 Matthijs Kooijman
 * Copyright (c) 2018-2019 MCCI Corporation
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * which accompanies this distribution, and is available at
 * http://www.eclipse.org/legal/epl-v10.html
 *
 * This the HAL to run LMIC on top of the Arduino environment.
 *******************************************************************************/

// include all the lmic header files, including ../lmic/hal.h
#include "../lmic.h"
#include "hal.h"
#include "wiring_time.h"

#include "../../Inc/main.h"
#include "../../Inc/stm32l0xx_hal.h"


void hal_init () {
    // initialize SPI chip select to high (it's active low)
    hal_pin_nss(1);
}

// set radio NSS pin to given value
void hal_pin_nss (u1_t val) {
    HAL_GPIO_WritePin(NSS_GPIO_Port,NSS_Pin, (GPIO_PinState)val);
}


// set radio RST pin to given value (or keep floating!)
void hal_pin_rst (u1_t val) {
    if(val == 0 || val == 1) { // drive pin
    	GPIO_InitTypeDef GPIO_InitStruct;
    	GPIO_InitStruct.Pin = NRST_Pin;
    	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    	HAL_GPIO_Init(NRST_GPIO_Port, &GPIO_InitStruct);

    	HAL_GPIO_WritePin(NRST_GPIO_Port,NRST_Pin, (GPIO_PinState)val);

    } else { // keep pin floating
        GPIO_InitTypeDef GPIO_InitStruct;
    	GPIO_InitStruct.Pin = NRST_Pin;
    	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    	GPIO_InitStruct.Pull = GPIO_NOPULL;
    	HAL_GPIO_Init(NRST_GPIO_Port, &GPIO_InitStruct);
    }
}


// generic EXTI IRQ handler for all channels
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin_int){
/*	debugString("Interrupt ");
	debugInt(GPIO_Pin_int);
	debugString(" \r\n");*/
	// DIO 0
	if(GPIO_Pin_int == DIO0_Pin) {
		// invoke radio handler (on IRQ!)
		radio_irq_handler(0);
	}
	// DIO 1
	if(GPIO_Pin_int == DIO1_Pin) {
	    // invoke radio handler (on IRQ!)
		radio_irq_handler(1);
	}
	// DIO 2
	if(GPIO_Pin_int == DIO2_Pin) {
	    // invoke radio handler (on IRQ!)
	    radio_irq_handler(2);
	}
}

// val ==1  => tx 1, rx 0 ; val == 0 => tx 0, rx 1
void hal_pin_rxtx (u1_t val) {
#ifdef RX_GPIO_Port
  #ifdef TX_GPIO_Port
    HAL_GPIO_WritePin(RX_GPIO_Port,RX_Pin,~val);
    HAL_GPIO_WritePin(TX_GPIO_Port,TX_Pin,val);
  #endif
#endif
}

// -----------------------------------------------------------------------------
// SPI

u1_t hal_spi (u1_t out) {
	u1_t outbuffer[] ="";
	u1_t inbuffer[] ="";
	outbuffer[0] = out;
	HAL_SPI_TransmitReceive(&hspi1,outbuffer,inbuffer,sizeof(outbuffer),HAL_MAX_DELAY);
	return inbuffer[0];
}

static void hal_spi_trx(u1_t cmd, u1_t* buf, size_t len, bit_t is_read) {
	hal_pin_nss(0);

	hal_spi(cmd);

    for (; len > 0; --len, ++buf) {
        u1_t data = is_read ? 0x00 : *buf;
        data = hal_spi(data);
        if (is_read)
            *buf = data;
    }

	hal_pin_nss(1);
}

void hal_spi_write(u1_t cmd, const u1_t* buf, size_t len) {
    hal_spi_trx(cmd, (u1_t*)buf, len, 0);
}

void hal_spi_read(u1_t cmd, u1_t* buf, size_t len) {
    hal_spi_trx(cmd, buf, len, 1);
}

// -----------------------------------------------------------------------------
// TIME


u4_t hal_ticks () {
    // Because micros() is scaled down in this function, micros() will
    // overflow before the tick timer should, causing the tick timer to
    // miss a significant part of its values if not corrected. To fix
    // this, the "overflow" serves as an overflow area for the micros()
    // counter. It consists of three parts:
    //  - The US_PER_OSTICK upper bits are effectively an extension for
    //    the micros() counter and are added to the result of this
    //    function.
    //  - The next bit overlaps with the most significant bit of
    //    micros(). This is used to detect micros() overflows.
    //  - The remaining bits are always zero.
    //
    // By comparing the overlapping bit with the corresponding bit in
    // the micros() return value, overflows can be detected and the
    // upper bits are incremented. This is done using some clever
    // bitwise operations, to remove the need for comparisons and a
    // jumps, which should result in efficient code. By avoiding shifts
    // other than by multiples of 8 as much as possible, this is also
    // efficient on AVR (which only has 1-bit shifts).
    static uint8_t overflow = 0;

    // Scaled down timestamp. The top US_PER_OSTICK_EXPONENT bits are 0,
    // the others will be the lower bits of our return value.
    uint32_t scaled = micros() >> US_PER_OSTICK_EXPONENT;
    // Most significant byte of scaled
    uint8_t msb = scaled >> 24;
    // Mask pointing to the overlapping bit in msb and overflow.
    const uint8_t mask = (1 << (7 - US_PER_OSTICK_EXPONENT));
    // Update overflow. If the overlapping bit is different
    // between overflow and msb, it is added to the stored value,
    // so the overlapping bit becomes equal again and, if it changed
    // from 1 to 0, the upper bits are incremented.
    overflow += (msb ^ overflow) & mask;

    // Return the scaled value with the upper bits of stored added. The
    // overlapping bit will be equal and the lower bits will be 0, so
    // bitwise or is a no-op for them.
    return scaled | ((uint32_t)overflow << 24);

    // 0 leads to correct, but overly complex code (it could just return
    // micros() unmodified), 8 leaves no room for the overlapping bit.
//    static_assert(US_PER_OSTICK_EXPONENT > 0 && US_PER_OSTICK_EXPONENT < 8, "Invalid US_PER_OSTICK_EXPONENT value");
}

// Returns the number of ticks until time. Negative values indicate that
// time has already passed.
static s4_t delta_time(u4_t time) {
    return (s4_t)(time - hal_ticks());
}

// deal with boards that are stressed by no-interrupt delays #529, etc.
#if defined(ARDUINO_DISCO_L072CZ_LRWAN1)
# define HAL_WAITUNTIL_DOWNCOUNT_MS 16      // on this board, 16 ms works better
# define HAL_WAITUNTIL_DOWNCOUNT_THRESH ms2osticks(16)  // as does this threashold.
#else
# define HAL_WAITUNTIL_DOWNCOUNT_MS 8       // on most boards, delay for 8 ms
# define HAL_WAITUNTIL_DOWNCOUNT_THRESH ms2osticks(9) // but try to leave a little slack for final timing.
#endif

u4_t hal_waitUntil (u4_t time) {
    s4_t delta = delta_time(time);
    // check for already too late.
    if (delta < 0)
        return -delta;

    // From delayMicroseconds docs: Currently, the largest value that
    // will produce an accurate delay is 16383. Also, STM32 does a better
    // job with delay is less than 10,000 us; so reduce in steps.
    // It's nice to use delay() for the longer times.
    while (delta > HAL_WAITUNTIL_DOWNCOUNT_THRESH) {
        // deliberately delay 8ms rather than 9ms, so we
        // will exit loop with delta typically positive.
        // Depends on BSP keeping time accurately even if interrupts
        // are disabled.
        delay(HAL_WAITUNTIL_DOWNCOUNT_MS);
        // re-synchronize.
        delta = delta_time(time);
    }

    // The radio driver runs with interrupt disabled, and this can
    // mess up timing APIs on some platforms. If we know the BSP feature
    // set, we can decide whether to use delta_time() [more exact, 
    // but not always possible with interrupts off], or fall back to
    // delay_microseconds() [less exact, but more universal]

#if defined(_mcci_arduino_version)
    // unluckily, delayMicroseconds() isn't very accurate.
    // but delta_time() works with interrupts disabled.
    // so spin using delta_time().
    while (delta_time(time) > 0)
        /* loop */;
#else // ! defined(_mcci_arduino_version)
    // on other BSPs, we need to stick with the older way,
    // until we fix the radio driver to run with interrupts
    // enabled.
    if (delta > 0)
        delayMicroseconds(delta * US_PER_OSTICK);
#endif // ! defined(_mcci_arduino_version)

    // we aren't "late". Callers are interested in gross delays, not
    // necessarily delays due to poor timekeeping here.
    return 0;
}

// check and rewind for target time
u1_t hal_checkTimer (u4_t time) {
    // No need to schedule wakeup, since we're not sleeping
    return delta_time(time) <= 0;
}

static uint8_t irqlevel = 0;

void hal_disableIRQs () {

}

void hal_enableIRQs () {

}

uint8_t hal_getIrqLevel(void) {
    return irqlevel;
}

void hal_sleep () {
	// low power sleep mode
#ifndef CFG_no_low_power_sleep_mode
	PWR->CR |= PWR_CR_LPSDSR;
#endif
    // suspend execution until IRQ, regardless of the CPSR I-bit
    __WFI();
}


void hal_failed(const char *file, u2_t line){
	Serial_DEBUG("HAL Error ");
	Serial_DEBUG(file);
	Serial_DEBUG(" - ");
	Serial_DEBUG(Int_to_Char(line));
	Serial_DEBUG("\r\n");

	Display_DEBUG("HAL ERROR");
	Display_DEBUG_N(Int_to_Char(line), 1);

    hal_sleep();
    while(1);
}

ostime_t hal_setModuleActive (bit_t val){
	return 0;
}

// -----------------------------------------------------------------------------

