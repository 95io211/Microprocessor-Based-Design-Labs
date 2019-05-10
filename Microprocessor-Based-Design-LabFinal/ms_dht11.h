/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "inc/hw_timer.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "driverlib/timer.h"


#ifndef __DHT_H
#define __DHT_H

// default is DHT11, comment this define for DHT22 = RHT03
#define DHT22

#define DHT_PORT_PERIPH SYSCTL_PERIPH_GPIOD
#define DHT_PORT GPIO_PORTD_BASE
#define DHT_PIN GPIO_PIN_1

#define DHT_TIM_PERIPH	SYSCTL_PERIPH_WTIMER3
#define DHT_TIM_BASE	WTIMER3_BASE
#define DHT_TIM			TIMER_A

#define MCU_CLOCK SysCtlClockGet()
#define DHT_WAIT_18ms ((MCU_CLOCK*18)/3000)
#define DHT_WAIT_20us ((MCU_CLOCK*2)/300000)

#define DHT_TIMEOUT    ((MCU_CLOCK*9)/100000) // 90us
#define DHT_TIME_BIT_1 ((MCU_CLOCK*7)/100000) // 70us

#define DON()		GPIOPinTypeGPIOOutput(DHT_PORT, DHT_PIN)
#define DIN()		GPIOPinTypeGPIOInput(DHT_PORT, DHT_PIN)

#define DHT_DATA(__DATA__)	GPIOPinWrite(DHT_PORT, DHT_PIN, (__DATA__))
#define DHT_READ	GPIOPinRead(DHT_PORT, DHT_PIN)


typedef struct {
    float celsius_temp;
    float humidity;
} DHT_TypeDef;


// Interface
void dht_init(void);
uint8_t dht_readTH(DHT_TypeDef *);



#endif

