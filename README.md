# LOW ENERGY GPS TRACKER USING LORA NETWORK
Author: [Milan Múčka](mailto:xmucka02@stud.fit.vutbr.cz)
Release date: 30.7.2020

Project was created for Master Thesis on Faculthy of Information Technology in Brno University of Technology. It is using LoRaWAN module from Ai-Thinker **RHF76-052** with included **STM32L051C8T6** and **SX1276** for LoRa radio module. For GPS I was using **Neo-M8N**, but every NMEA based GPS receiver should work. To project was added **MPU-6050** Accelerometer for power saving and interrupt control while moving. Also there is OLED display controller **SSD1306** for informing about actual status of tracker. For connection details see file Schematic.pdf.

This part is STM32Cube application for hardware LoRaWAN tracker using platform **TheThingsNetwork**.

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

## Other parts
TTN MQTT Receiver - https://github.com/MMucka/TTN_MQTT_Receiver

WEB Page for User interface - https://github.com/MMucka/TTN_LoRaTracker_MAP
