
/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <inc/hw_i2c.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/i2c.h>
#include "seg7.h"

int i = 0;
int j = 0;
int k = 0;
int l = 0;

static uint8_t seg7Coding[10] = {
		0b00111111,			 // 0
		0b00000110,			// 1
		0b01011011,			// digit 2
		0b01001111,			// digit 3
		0b01100110,          //4
		0b01101101,          //5
		0b01111101,          //6
		0b00000111,          //7
		0b01111111,          //8
		0b01100111           //9
};

// The colon status: if colon == 0b10000000, then the colon is on,
// otherwise it is off.
static uint8_t colon = 0;

void clockUpdate(uint32_t time)
{
	uint8_t code[4];

	if(colon==0b10000000)
	{
		i=i+1;
		if(i>9)
		{
			i=0;
			j=j+1;
			if(j>4)
			{
				j=0;
				k=k+1;
				if(k>9)
				{
					k=0;
					l=l+1;
					if(l>1)
					{
						l=0;
					}
				}
			}
		}
	}

	code[0] = seg7Coding[i] + colon;
	code[1] = seg7Coding[j] + colon;
	code[2] = seg7Coding[k] + colon;
	code[3] = seg7Coding[l] + colon;
	seg7Update(code);

	if (colon == 0b00000000) {
		colon = 0b10000000;
	}
	else {
		colon = 0b00000000;
	}

	schdCallback(clockUpdate, time + 500);
}

int main(void)
{
	lpInit();
	seg7Init();

	uprintf("%s\n\r", "Lab2!");

	// Schedule the first callback events for LED flashing and push button checking.
	// Those trigger callback chains. The time unit is millisecond.

	schdCallback(clockUpdate, 1000);
	//schdCallback(checkPushButton, 1005);




	// Loop forever
	while (true) {
		schdExecute();
	}
}
