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
#include <driverlib/adc.c>
#include <driverlib/adc.h>
#include "launchpad.h"
#include "rotary.h"

uint32_t ui32Value=0;
void initADC(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_TS | ADC_CTL_IE | ADC_CTL_END);
	ADCSequenceEnable(ADC0_BASE, 0);
}

uint32_t next(){
		// Trigger the sample sequence.
				//
				ADCProcessorTrigger(ADC0_BASE, 0);
				//
				// Wait until the sample sequence has completed.
				//
				while(!ADCIntStatus(ADC0_BASE, 0, false))
				{

				}
					//
				// Read the value from the ADC.
				//
				ADCSequenceDataGet(ADC0_BASE, 0, &ui32Value);

				return ui32Value;
}

