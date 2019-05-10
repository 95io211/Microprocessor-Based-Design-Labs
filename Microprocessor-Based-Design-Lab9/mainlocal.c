/*
 * main.c
 *
 *  Created on: Aug 23, 2016
 *      Author: zzhang
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "sendchar.h"

void checkPushButton(uint32_t time)
{
	char code = uartGetChar1();
	uint32_t delay;

	switch (code) {
		case 1:
			uprintf("%s\n\r", "SW1 is pushed.");
		delay = 250;							// Use an inertia for soft debouncing
		break;

		case 2:
			uprintf("%s\n\r", "SW2 is pushed");
			delay = 250;							// Use an inertia for soft debouncing
			break;

		default:
			delay = 50;
		}

	schdCallback(checkPushButton, time + delay);
}

int main(void){
		lpInit();
		uartInit1();

		uprintf("remoteremoterelocalcal");

		schdCallback(checkPushButton, 1005);

		while (true) {
			schdExecute();
		}
}
