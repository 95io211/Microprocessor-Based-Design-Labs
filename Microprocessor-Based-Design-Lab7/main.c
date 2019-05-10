
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

uint32_t a,b;

void timerInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);

	TimerControlEvent(WTIMER2_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_CAP_TIME_UP);
	TimerEnable(WTIMER2_BASE,TIMER_A); // This function enables operation of the timer module. The timer must be configured before it is enabled.
/*
	TimerEnable(WTIMER2_BASE,TIMER_A);
	TimerConfigure(WTIMER2_BASE, TIMER_CFG_PERIODIC_UP);
	TimerControlEvent(WTIMER2_BASE, TIMER_A, TIMER_EVENT_BOTH_EDGES);

	TimerIntEnable(WTIMER2_BASE, TIMER_CAPA_EVENT);
	TimerADCEventSet(WTIMER2_BASE, TIMER_ADC_CAPEVENT_A);
*/

}

void sendPulse()
{

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);

	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);
	waitMs(1);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, GPIO_PIN_0);
	waitMs(5);
	GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_0, 0);

	GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
	GPIOPinConfigure(GPIO_PD0_WT2CCP0);
}

uint32_t getValue()
{


/*
	TimerControlTrigger(WTIMER2_BASE, TIMER_A, true);

	while(TimerIntStatus(WTIMER2_BASE, true))
	{
		GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
		GPIOPinConfigure(GPIO_PD6_WT5CCP0);
		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0);
		a = TimerValueGet(WTIMER2_BASE, TIMER_A);

		//TimerControlTrigger(WTIMER2_BASE, TIMER_A, false);
		GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_0);
		GPIOPinConfigure(GPIO_PD6_WT5CCP0);
		GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_0);
		b = TimerValueGet(WTIMER2_BASE, TIMER_A);

		uprintf("a = %d\n\rb = %d\n\r",a,b);
		TimerIntClear(WTIMER2_BASE, GPIO_PD6_WT5CCP0);
	}
*/

	sendPulse();

	while(!TimerIntStatus(WTIMER2_BASE, false))
	{
		//do nothing
	}
	a = TimerValueGet(WTIMER2_BASE, TIMER_A);
		TimerIntClear(WTIMER2_BASE, TIMER_CAPA_EVENT);


	while(!TimerIntStatus(WTIMER2_BASE, false))
	{
		//do nothing
	}
	b = TimerValueGet(WTIMER2_BASE, TIMER_A);
		TimerIntClear(WTIMER2_BASE, TIMER_CAPA_EVENT);

	uprintf("a = %d\n\rb = %d\n\r",a,b);
	return b-a;
}

void getRange(uint32_t time)
{
	uprintf("difference = %d\n\r", getValue());

	schdCallback(getRange, time + 1000);
}

int main(void)
{
	lpInit();
	timerInit();
	uprintf("test\n\r");

	schdCallback(getRange,1000);

	// Loop forever
	while (true) {
		schdExecute();
	}
}
