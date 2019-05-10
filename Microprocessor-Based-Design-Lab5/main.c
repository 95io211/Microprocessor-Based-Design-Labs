/*
 * main.c: For Lab 5, Sleeping LaunchPad
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include "launchpad.h"
#include "buzzer.h"
#include "motion.h"

// Uncomment this line if you want the LaunchPad to flash the LED instead of beeping
// If it is defined, then LED replaces the buzzer
#define QUIET
volatile int colorChoice;
/*
 * Buzzer play function. Note that this function must run in a thread, e.g. not be callback-based,
 * because the frequency of buzzer on/off is too high for the callback scheduling module.
 */
void
buzzPlay()
{
	while (true) {
		// Turn on/off the buzzer every 500 microseconds, producing a 1KHz square waveform
	    // Repeat it for three seconds
#ifndef QUIET
	    for (i = 0; i < 3000; i++) {
	    	buzzOn();
	    	ledOnOff()
	    	waitUs(500);
	    	buzzOff();
	    	waitUs(500);
	    }
#else
    	IntMasterDisable();				// disable all interrupts so that the color stays the same


		IntMasterEnable();				// enable interrupt signal from interrupt controller to CPU
#endif

		// Execute an WFI instruction to put the processor into the sleep mode
	    __asm("    wfi");
	}
}

/*
 * Interrupt handler for push button (on Port F)
 */
void pbIntrHandler()
{
	GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
	int code = pbRead();
	int i;
	// Clear interrupt. This is necessary, otherwise the
	// interrupt handler will be executed forever.

	switch(code){
	case 1:
    	for (i = 0; i < 3; i++) {
			ledTurnOnOff(true, true, false);
			waitMs(500);
			ledTurnOnOff(false, false, false);
			waitMs(500);
    	}
    	break;
	case 2:
    	for (i = 0; i < 3; i++) {
			ledTurnOnOff(false, false, true);
			waitMs(500);
			ledTurnOnOff(false, false, false);
			waitMs(500);
    	}
    	break;
	}
}

void handler()
{
	int i;
	GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_5);
	for (i = 0; i < 3; i++) {
		ledTurnOnOff(true, true, true);
		waitMs(500);
		ledTurnOnOff(false, false, false);
		waitMs(500);
	}
}

void buzzer()
{
	GPIOIntClear(GPIO_PORTC_BASE, GPIO_PIN_5);
	buzzOn();
	waitMs(500);
	buzzOff();
	waitMs(500);
}

/*
 * Select a set of interrupts that can wake up the LaunchPad
 */
void
setInterrupts()
{
	// Disable the system tick interrupt, which has been enabled in lpInit();
	// otherwise, it will wake up buzzPlay().
	SysTickIntDisable();

	// Set interrupt on Port F, pin 0 (SW1) and pin 4 (SW2)
	GPIOIntRegister(GPIO_PORTF_BASE, pbIntrHandler);			// register the interrupt handler
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);	// enable interrupts on SW1 and SW2 input
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, 	// interrupt on falling edge, note that SW1 and SW2 are active low
			GPIO_FALLING_EDGE);
	IntPrioritySet(INT_GPIOF, 0);								// raise interrupt level to 0 (highest) to make sure MCU wakes up

	GPIOIntRegister(GPIO_PORTC_BASE, buzzer);
	GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_5,
			GPIO_RISING_EDGE);
	IntPrioritySet(INT_GPIOF, 0);
}

int main(void)
{
	// Initialize the launchpad, buzzer
	lpInit();
	buzzInit();
#ifndef QUIET
	// Iniialize the buzzer
#endif

	// IN YOUR SOLUTION CODE, call the function that initializes the PIR montion sensor,
	//    which you wrote in Lab 4
	// pirInit();
	motionInit();

	setInterrupts();

	uprintf("%s\n\r", "Lab 5: Sleeping LaunchPad");

	buzzPlay();
}
