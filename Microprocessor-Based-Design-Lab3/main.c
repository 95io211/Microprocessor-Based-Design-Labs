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

// Stop watch running state
typedef enum {Reset, Run, Pause} state_t;

// Status of the stopwatch
static struct {
	state_t state;			// the running state of the stopwatch
	uint8_t s1;				// 1st digit of seconds
	uint8_t s2;				// 2nd digit of seconds
	uint8_t c1;				// 1st digit of centiseconds
	uint8_t c2;				// 2nd digit of centiseconds
} display = {Reset, 0, 0, 0, 0};

// Update the clock display
void
stopWatchUpdate(uint32_t time)							// The scheduled time
{
	// FIXME The callback chain can be stopped if the stopwatch is not running. The
	// best option is to add a function in the event-driven scheduling to cancel a
	// future callback, and then use the function to remove the callback of
	// stopWatchUpdate(). This change would improve energy efficiency.

	// Update clock and display only if the stopwatch is running
if(display.state == Run){
		// Calculate the display digits for the next update
		if (++display.c2 >= 10)
		{
			display.c2 = 0;
			if(++display.c1 >= 10)
			{
				display.c1=0;
				if(++display.s2 >=10)
				{
					display.s2=0;
					if(++display.s1 >=6)
					{
						display.s1=0;
					}
				}
			}
		}
}
		// Update the display
		seg7DigitUpdate(display.s1, display.s2, display.c1, display.c2);


	// Call back after 10 milliseconds
	schdCallback(stopWatchUpdate, time + 10);
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

			if(display.state == Pause)
			{
				display.s1 = 0;
				display.s2 = 0;
				display.c1 = 0;
				display.c2 = 0;
				display.state = Reset;

			}
			// Update the display
			seg7DigitUpdate(display.s1, display.s2, display.c1, display.c2);

			delay = 250;							// Use an inertia for soft debouncing
			break;

		case 2:
			uprintf("%s\n\r", "SW2 is pushed");

			switch (display.state) {

			case Reset:
				display.state = Run;
			break;

			case Run:
				display.s1 = display.s1;
				display.s2 = display.s2;
				display.c1 = display.c1;
				display.c2 = display.c2;
				display.state = Pause;
			break;

			case Pause:
				display.state = Run;
			break;

			}

			delay = 250;							// Use an inertia for soft debouncing
			break;

		default:
			delay = 50;
		}

	schdCallback(checkPushButton, time + delay);
}

int main(void)
{
	lpInit();
	seg7Init();

	uprintf("%s\n\r", "Lab 3: Stopwatch");

	// Update the clock display
	seg7DigitUpdate(0, 0, 0, 0);

	// Schedule the first callback events for LED flashing and push button checking.
	// Those trigger callback chains. The time unit is millisecond.
	schdCallback(stopWatchUpdate, 1000);
	schdCallback(checkPushButton, 1005);

	// Loop forever
	while (true) {
		schdExecute();
	}
}
