/*
 * main.h
 *
 *  Created on: Jun 20, 2019
 *      Author: Milan Múčka
 *     Project: Low Energy GPS Tracker Using LoRa Network
 */

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32l0xx_hal.h"


#define LCD_ENABLE_Pin GPIO_PIN_0
#define LCD_ENABLE_GPIO_Port GPIOA
#define HF_Pin GPIO_PIN_1
#define HF_GPIO_Port GPIOA
#define LF_Pin GPIO_PIN_2
#define LF_GPIO_Port GPIOA
#define ACC_INTERRUPT_Pin GPIO_PIN_3
#define ACC_INTERRUPT_GPIO_Port GPIOA
#define ACC_INTERRUPT_EXTI_IRQn EXTI2_3_IRQn
#define NSS_Pin GPIO_PIN_4
#define NSS_GPIO_Port GPIOA
#define NSS_EXTI_IRQn EXTI4_15_IRQn
#define DIO2_Pin GPIO_PIN_0
#define DIO2_GPIO_Port GPIOB
#define DIO2_EXTI_IRQn EXTI0_1_IRQn
#define DIO3_Pin GPIO_PIN_1
#define DIO3_GPIO_Port GPIOB
#define DIO3_EXTI_IRQn EXTI0_1_IRQn
#define DIO1_Pin GPIO_PIN_2
#define DIO1_GPIO_Port GPIOB
#define DIO1_EXTI_IRQn EXTI2_3_IRQn
#define DIO0_Pin GPIO_PIN_10
#define DIO0_GPIO_Port GPIOB
#define DIO0_EXTI_IRQn EXTI4_15_IRQn
#define GPS_ENABLE_Pin GPIO_PIN_9
#define GPS_ENABLE_GPIO_Port GPIOA
#define GPS_INTERRUPT_Pin GPIO_PIN_11
#define GPS_INTERRUPT_GPIO_Port GPIOA
#define GPS_INTERRUPT_EXTI_IRQn EXTI4_15_IRQn
#define LED_Pin GPIO_PIN_3
#define LED_GPIO_Port GPIOB
#define NRST_Pin GPIO_PIN_11
#define NRST_GPIO_Port GPIOB


extern SPI_HandleTypeDef hspi1;
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;


void Error_Handler(void);

void Serial_DEBUG(const char*);
void Display_DEBUG(const char*);
void Display_DEBUG_N(const char* message, int line);
char* Int_to_Char(int val);


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
