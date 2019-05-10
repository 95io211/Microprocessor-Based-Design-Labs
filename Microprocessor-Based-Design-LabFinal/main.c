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
#include "utils/uartstdio.h"

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
int o;

#include "ms_dht11.h"
DHT_TypeDef th;

typedef enum {raw, percentage} state_t;

static struct {
	state_t state;			// the running state of the stopwatch
	uint8_t s1;				// 1st digit of seconds
	uint8_t s2;				// 2nd digit of seconds
	uint8_t c1;				// 1st digit of centiseconds
	uint8_t c2;				// 2nd digit of centiseconds
} display = {raw, 0, 0, 0, 0};


int humid;
int temperature;

int uses(){
	dht_readTH(&th);
	UARTprintf(" T=%d.%d H=%d.%d\r\n", (int)th.celsius_temp, ((int)th.celsius_temp)%10, (int)th.humidity, ((int)th.humidity)%10);
	SysCtlDelay((SysCtlClockGet()/3));
	return (int)th.humidity;
//	temperature=th.celsius_temp;
}
int getTemp(){
	dht_readTH(&th);
	UARTprintf(" T=%d.%d H=%d.%d\r\n", (int)th.celsius_temp, ((int)th.celsius_temp)%10, (int)th.humidity, ((int)th.humidity)%10);
	SysCtlDelay((SysCtlClockGet()/3));
//	humid=th.humidity;
	return (int)th.celsius_temp;
}

void
displayUpdate(uint32_t time)							// The scheduled time
{
	z=uses();
	t=getTemp();
	o=moisture();

//	uprintf("%d=H,%d=T",humid,temperature);
	if(o>300){
		sendRed(99);
		sendBlue(99);
		sendGreen(99);
		getTemp();
		if(getTemp()>25){
			sendRed(30);
			sendBlue(99);
			sendGreen(99);
		}
		if(z<45){
			sendRed(99);
			sendBlue(30);
			sendGreen(99);
		}
	}
	else if(o<300){
			sendRed(99);
			sendBlue(99);
			sendGreen(30);
	}

	if(display.state==raw){
		i = (z/1000);
		j = (z/100)%10;
		k = (z/10)%10;
		l = (z%10);

		display.s1=i;
		display.s2=j;
		display.c1=k;
		display.c2=l;

	}
	else if(display.state==percentage)
	{
		q = (t/1000);
		w = (t/100)%10;
		e = (t/10)%10;
		r = (t)%10;

		display.s1=q;
		display.s2=w;
		display.c1=e;
		display.c2=r;

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

	switch (code) {
		case 1:
		//	uprintf("%s\n\r", "SW1 is pushed.");
			display.state=raw;
			delay = 250;							// Use an inertia for soft debouncing
			break;

		case 2:
		//	uprintf("%s\n\r", "SW2 is pushed");
		//	uprintf("%s\n\r", "seg 7 humidity display");
			display.state=percentage;
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
		temp();
		lpInit();
		seg7Init();

		uprintf("%s\n\r", "Lab 6:ADC");

		schdCallback(displayUpdate, 1000);
		schdCallback(checkPushButton, 1005);

		while (true) {
			schdExecute();
		}
}
