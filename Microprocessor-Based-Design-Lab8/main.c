
/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>
#include <inc/hw_memmap.h>

uint32_t a = 20;
uint32_t b = 19;

void initPWM()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

	GPIOPinConfigure(GPIO_PF1_T0CCP1);
	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_1);

	TimerConfigure(TIMER0_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM );

	TimerEnable(TIMER0_BASE, TIMER_B);

	TimerLoadSet(TIMER0_BASE, TIMER_B, a);
//	TimerMatchSet(TIMER0_BASE, TIMER_B, b);
}


void colorsPWM()
{
/*
	TimerMatchSet(TIMER0_BASE, TIMER_B, b);
	if(b<90){
		b=a;
	}
	b--;
	uprintf("match = %d\n\rload = %d\n\r", TimerMatchGet(TIMER0_BASE, TIMER_B),TimerLoadGet(TIMER0_BASE,TIMER_B));
*/
/*
	while(1)
	{
	TimerMatchSet(TIMER0_BASE, TIMER_B, b++);
	if((b) >= (a))
		{
			b=0;
		}
    waitMs(3);
	}
*/
/*
	uint32_t ulPeriod, dutyCycle;
    ulPeriod = 1000;
    dutyCycle = 991;

    while(1) { // The following code varies the duty cycle over time
        TimerMatchSet(TIMER0_BASE, TIMER_B, dutyCycle++);

        if(dutyCycle >= ulPeriod - 1)
            dutyCycle = 0;

        SysCtlDelay(1);
    }
*/
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
//	GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, b++);

	int i;
	while(1){
		for(i = b; i > 15; i--){
		TimerMatchSet(TIMER0_BASE, TIMER_B, i);
		uprintf("match = %d\n\rload = %d\n\r", TimerMatchGet(TIMER0_BASE, TIMER_B),TimerLoadGet(TIMER0_BASE,TIMER_B));
		waitUs(1);
		}
	}


/*
	while(1){
		TimerMatchSet(TIMER0_BASE, TIMER_B, b++);
				if(b >= a){
					b = 100;
				}
		uprintf("match = %d\n\rload = %d\n\r", TimerMatchGet(TIMER0_BASE, TIMER_B),TimerLoadGet(TIMER0_BASE,TIMER_B));
		SysCtlDelay(1000);
	}
	*/
}

/*
void spamPWM(uint32_t time)
{
	colorsPWM();

	schdCallback(spamPWM, time + 100);
}
*/
int main(void)
{
	lpInit();
	initPWM();
	colorsPWM();

	uprintf("%s\n\r", "test");

//	schdCallback(spamPWM, 1000);

	while (true) {
		schdExecute();
	}
}
