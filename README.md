# LOW ENERGY GPS TRACKER USING LORA NETWORK
Author: [Milan Múčka](mailto:xmucka02@stud.fit.vutbr.cz)
Release date: 30.7.2020

STM32Cube application for LoRaWAN tracker
Using **STM32L051C8T6** Microcontroller for controlling board and **SX1276** for LoRa radio module


## SETTINGS
Project configuration can be found in **\Core\Inc\system_config.h**
LoRaWAN keys are configured in **\Core\Inc\lora.h**
Available options are:
```
#define GPS_ENABLE 		1**        --- enable GPS receiver 
#define UART_SPEED 		9600       --- define baudrate for UART communication (GPS need 9600)
#define LCD_ENABLE 		0          --- enable LCD display
#define LED_ENABLE 		1          --- enable debug LED
#define TX_INTERVAL		20         --- transmitting interval of LoRa in seconds
#define ACC_ENABLE		1          --- enable Accelerometer
#define ACC_INTERRUPT	1          --- enable interrupt from Accelerometer to wake up device
#define LORA_ENABLE		1          --- enable LoRa module for communication
#define LORA_INTERRUPT	1          --- enable interrupt for LoRa received messages and states

#define Serial_DEBUG_ENABLE 1      --- enable debugging with Serial UART communication

#define LORA_WAIT_ACK	0          --- wait for acknowlidgements from LoRa network 0/1
#define HF_FREQUENCY 	1          --- frequency used for LoRa 868MHz
#define LF_FREQUENCY 	1          --- frequency used for LoRa 433MHz
```