
/*
 * main.c
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "launchpad.h"
#include <driverlib/sysctl.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/adc.c>
#include <driverlib/adc.h>

#define PORT_PERIPH		SYSCTL_PERIPH_GPIOA	// The peripheral def of Port A
#define PORT			GPIO_PORTA_BASE		// The base address of the I2C port
#define CLK				GPIO_PIN_6			// The 8-bit pin value (mask format) for clock
#define DIO				GPIO_PIN_7			// The 8-bit pin value (mask format) for data IO

void
ADCInit()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	//GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_3);
	ADCSequenceConfigure(ADC0_BASE, 2, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 2, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH7);
	ADCSequenceEnable(ADC0_BASE, 2);

//	SysTickIntDisable();
//	ADCIntEnable(ADC0_BASE, 2);
//	ADCIntRegister(ADC0_BASE, 2, getValue);
}

void getValue()
{
	//ADCIntClear(ADC0_BASE, 2);
	ADCProcessorTrigger(ADC0_BASE, 2);

	while(!ADCIntStatus(ADC0_BASE,2,false))
	{

	}
	uint32_t a = 0;
	uint32_t zz = 0;
	//GPIOPinRead(GPIO_PORTE_BASE, GPIO_PIN_3);
	ADCSequenceDataGet(ADC0_BASE, 2, &a);
	zz = ADCSequenceDataGet(ADC0_BASE, 2, &a);
	uprintf("zz = %d\n\ra = %d\n\r",zz,a);
}

void test(uint32_t time){
	getValue();
	schdCallback(test, time + 500);
}


int main(void)
{
	lpInit();
	ADCInit();

	schdCallback(test,1000);

	uprintf("test\n\r");


while (true) {
	schdExecute();
	}
}
