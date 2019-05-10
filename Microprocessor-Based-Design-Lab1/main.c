
/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"

int delay = 1000; //global delay
int s1 = 1; //global switch 1
int s2 = 1; //global switch 2

// Event-drive code for flashing LED
void
flashLED(uint32_t time)							// the scheduled time
{
	static enum {OFF, ON} state = OFF;			// LED ON/OFF state


	// Flip LED and record state change
	switch (state) {
	case OFF:
		if(s2 == 1){
			ledTurnOnOff(true, true, true);
			state = ON;
		}
		else if( s2 == 2 ){
			ledTurnOnOff(true, false, false);
			state = ON;
		}
		else if( s2 == 3){
			ledTurnOnOff(false, true, false);
			state = ON;
		}
		else{
			ledTurnOnOff(false, false, true);
			state = ON;
			s2 = 0;
		}

		break;

	case ON:
		ledTurnOnOff(false, false, false);
		state = OFF;
		break;
	}

	// Schedule the next callback
	uprintf("%d\t%d\n\r", time, delay);
	schdCallback(flashLED, time + delay);
}

// Event driven code for checking push button
void
checkPushButton(uint32_t time)
{
	int code = pbRead();
	uint32_t delay1;
	switch (code) {
	case 1:

		uprintf("%s\n\r", "SW1 is pushed.");
		s1 = s1 + 1;
		delay1= 250;	   // Use an inertia for soft debouncing

		if(s1 == 1){
			delay = 1000;
		}
		else if(s1 == 2){
			delay = 2000;
		}
		else{
			delay = 500;
			s1 = 0;
		}

		break;

	case 2:
		s2 = s2 + 1;
		uprintf("%s\n\r", "SW2 is pushed");
		delay1= 250;							// Use an inertia for soft debouncing
		break;

	default:
		delay1= 10;
	}

	schdCallback(checkPushButton, time + delay1);

}

int main(void)
{
	lpInit();

	uprintf("%s\n\r", "Welcome to ECE367!");

	// Schedule the first callback events for LED flashing and push button checking.
	// Those trigger callback chains. The time unit is millisecond.

	schdCallback(flashLED, delay);
	schdCallback(checkPushButton, 1005);




	// Loop forever
	while (true) {
		schdExecute();
	}
}
