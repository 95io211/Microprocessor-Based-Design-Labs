/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// The state of the buzzer
static enum {Off, On} buzzState = Off;
/*
 * Buzzer play function. Note that this function must run in a thread, e.g. not be callback-based,
 * because the frequency of buzzer on/off is too high for the callback scheduling module.
 */

void
buzzPlay()
{
	while (true) {
		// Try executing callback events. Pushbutton is checked here
		schdExecute();

		// If buzzer state is not ON, skip the rest of the loop
		if (buzzState ==Off)
			if(motionOn()==0)
			continue;
		if (buzzState ==On)
			if(motionOn()==32)


		// Turn on/off the buzzer every 500 microseconds, producing a 1KHz square waveform
		buzzOn();
		waitUs(500);
		buzzOff();
		waitUs(500);
	}
}

/*
 * Event driven code for checking push button
 */
void
checkPushButton(uint32_t time)
{
	int code = pbRead();
	uint32_t delay;

	switch (code) {
	case 1:					// SW1: Start the buzzer
		buzzState = On;
		delay = 250;
		break;
	case 2:
		buzzState = Off;
		delay = 250;							// Use an inertia for soft debouncing
		break;
	default:
		delay = 10;
	}

	schdCallback(checkPushButton, time + delay);
}

int main(void)
{
	lpInit();
	buzzInit();
	motionInit();

	uprintf("%s\n\r", "Lab 4 starts");


	schdCallback(checkPushButton, 1010);
	buzzPlay();
}
