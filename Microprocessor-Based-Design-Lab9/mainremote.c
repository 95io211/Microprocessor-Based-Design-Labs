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
#include "seg7.h"
#include "seg7digit.h"
#include "rotary.h"
#include <driverlib/sysctl.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>

int i;
int j;
int k;
int l;
int q;
int w;
int e;
int r;
int z;
int t;
int TEMPC;
int TEMPF;

typedef enum {temp, raw} state_t;

static struct {
	state_t state;			// the running state of the stopwatch
	uint8_t s1;				// 1st digit of seconds
	uint8_t s2;				// 2nd digit of seconds
	uint8_t c1;				// 1st digit of centiseconds
	uint8_t c2;				// 2nd digit of centiseconds
} display = {raw, 0, 0, 0, 0};

// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.
// Update the clock display
void
displayUpdate(uint32_t time)							// The scheduled time
{
	if(display.state==temp){
		z=next();
		TEMPC = 147.5 - ((75 * (3.3) * (double)z) / 4096);
		TEMPF= ((TEMPC* 9) + 160) / 5;

		i = (TEMPF/1000);
		j = (TEMPF/100)%10;
		k = (TEMPF/10)%10;
		l = (TEMPF)%10;

		display.s1=i;
		if(display.s1==0){
			display.s1=0x2E;
		}
		display.s2=j;
		if(display.s1==0x2E && display.s2==0){
			display.s2=0x2E;
		}
		display.c1=k;
		if(display.s2==0x2E && display.c1==0){
			display.c1=0x2E;
		}
		display.c2=l;
		if(display.c1==0x2E && display.c2==0){
			display.c2=0x2E;
		}
	}
	else 	if(display.state==raw){
		z=next();

		i = (z/1000);
		j = (z/100)%10;
		k = (z/10)%10;
		l = (z%10);

		display.s1=i;
		if(display.s1==0){
			display.s1=0x2E;
		}
		display.s2=j;
		if(display.s1==0x2E && display.s2==0){
			display.s2=0x2E;
		}
		display.c1=k;
		if(display.s2==0x2E && display.c1==0){
			display.c1=0x2E;
		}
		display.c2=l;
		if(display.c1==0x2E && display.c2==0){
			display.c2=0x2E;
		}
	}

		// Update the display

		seg7DigitUpdate(display.s1, display.s2, display.c1, display.c2);

	// Call back after 10 milliseconds

schdCallback(displayUpdate, time + 100);
}
// Event driven code for checking push button. SW1 is the PAUSE/RESET key, and
// SW2 is the START key.
void
checkPushButton(uint32_t time)
{
	int code = pbRead();
	uint32_t delay;

	// YOUR CODE

	switch (code) {
		case 1:
			uprintf("%s\n\r", "SW1 is pushed.");
			display.state=temp;
			delay = 250;							// Use an inertia for soft debouncing
			break;

		case 2:
			uprintf("%s\n\r", "SW2 is pushed");
			display.state=raw;
			delay = 250;							// Use an inertia for soft debouncing
			break;

		default:
			delay = 50;
		}

	schdCallback(checkPushButton, time + delay);
}

int main(void)
{
		initADC();
		lpInit();
		seg7Init();


		uprintf("percent %d\n\r value \n\r",t,z);
		uprintf("%s\n\r", "Lab 5:");


		schdCallback(displayUpdate, 1000);
		schdCallback(checkPushButton, 1005);

		while (true) {
			schdExecute();
		}
}
