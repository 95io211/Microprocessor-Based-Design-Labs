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
#include "driverlib/timer.c"
#include "driverlib/timer.h"
#include "launchpad.h"
#include "utils/uartstdio.h"
#include "utils/uartstdio.c"
#include "inc/hw_types.h"

#include "ms_dht11.h"
DHT_TypeDef th;
void ConfigureUART(void);

#define timer0base	TIMER0_BASE
#define timer1base	TIMER1_BASE
uint32_t ui32Value=0;
uint32_t moistureVal=0;

void initADC(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);
	ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceConfigure(ADC1_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
	ADCSequenceStepConfigure(ADC0_BASE, 0, 0,
	ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH6);
	ADCSequenceStepConfigure(ADC1_BASE, 0, 0,
	ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH5);
	ADCSequenceEnable(ADC0_BASE, 0);
	ADCSequenceEnable(ADC1_BASE, 0);

	//begin moisture and tmep
	SysCtlClockSet(SYSCTL_SYSDIV_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);



}

void initTimer(){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); 			// activate Timer 0
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); 			// initializing the peripheral c

	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);

	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_1);		// initializing GPIOPin as a timer pin
	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_2);
	GPIOPinTypeTimer(GPIO_PORTF_BASE, GPIO_PIN_3);

	TimerConfigure(timer0base, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM); //Configuring to Capture rising and falling edges
	TimerConfigure(timer1base, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_A_PWM | TIMER_CFG_B_PWM);

	TimerEnable(timer0base, TIMER_B);
	TimerEnable(timer1base, TIMER_A);
	TimerEnable(timer1base, TIMER_B);
}

uint32_t moisture(){
		// Trigger the sample sequence.
				//
				ADCProcessorTrigger(ADC1_BASE, 0);
				//
				// Wait until the sample sequence has completed.
				//
				while(!ADCIntStatus(ADC1_BASE, 0, false))
				{

				}
					//
				// Read the value from the ADC.
				//
				ADCSequenceDataGet(ADC1_BASE, 0, &moistureVal);

				return moistureVal;
}

int red = 0;
void sendRed(red){
	initTimer();
	GPIOPinConfigure(GPIO_PF1_T0CCP1);
	TimerLoadSet(timer0base, TIMER_B, 100);
	TimerMatchSet(timer0base, TIMER_B, red);
}

int blue = 0;
void sendBlue(blue){
	initTimer();
	GPIOPinConfigure(GPIO_PF2_T1CCP0);
	TimerLoadSet(timer1base, TIMER_A, 100);
	TimerMatchSet(timer1base, TIMER_A, blue);
}

int green = 0;
void sendGreen(green){
	initTimer();
	GPIOPinConfigure(GPIO_PF3_T1CCP1);
	TimerLoadSet(timer1base, TIMER_B, 100);
	TimerMatchSet(timer1base, TIMER_B, green);
}

//begin MT


void temp(){

	ConfigureUART();

//	UARTprintf("Let's get started (Freq=%d)!\r\n", SysCtlClockGet());

	dht_init();
	th.celsius_temp= 0;
	th.humidity = 0;
	//SysCtlDelay((SysCtlClockGet()/3)*2);



//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);

//		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
//		SysCtlDelay((SysCtlClockGet()/3));



}



void ConfigureUART(void)
{

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    UARTStdioConfig(0, 115200, SysCtlClockGet());

}


