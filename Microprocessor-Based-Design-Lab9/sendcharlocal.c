#include "driverlib/timer.c"
#include "driverlib/timer.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <driverlib/gpio.h>
#include <driverlib/gpio.c>
#include <driverlib/pin_map.h>
#include <driverlib/sysctl.h>
#include <driverlib/interrupt.h>
#include <driverlib/systick.h>
#include <driverlib/uart.h>
#include <driverlib/uart.c>
#include "launchpad.h"

#define LINE_SIZE 	80		// Line size limit for uprintf

// Declare GPIO_PA0_U0RX and GPIO_PA1_U0TX if they are not declared yet
// For some reason, those are not declared in TivaWare for device LM4F120H5QR
#ifndef GPIO_PB0_U1RX
#define GPIO_PB0_U1RX           0x00010001
#endif
#ifndef GPIO_PB1_U1TX
#define GPIO_PB1_U1TX           0x00010401
#endif

// Enable UART0 with baud rate of 115,200 bps, and 8-N-1 frame
void uartInit1()
{
	// Enable UART1 and GPIO Port B as peripherals
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART1);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);

	// Connect UART1 RX and TX pins
	GPIOPinConfigure(GPIO_PB0_U1RX);
	GPIOPinConfigure(GPIO_PB1_U1TX);
	GPIOPinTypeUART(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

	// Configure baud rate and frame format
	UARTConfigSetExpClk(UART1_BASE, SysCtlClockGet(), 115200,
			            UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
}

// Send a character throught UART1
void uartPutChar1(char ch)
{
  UARTCharPut(UART1_BASE, ch);
}

// Receive a character through UART1
char uartGetChar1()
{
	char ch = UARTCharGet(UART1_BASE);
	return ch;
}

int uartPutString1(char *buffer)
{
	char ch;
	int n = 0;

	// Repeat sending the char at the head until the NULL is seen
	while ((ch = *buffer++) != '\0') {
		uartPutChar1(ch);
		n++;
	}

	return n;
}

// Print a string through UART0 using an internal buffer of 80 characters.
// Return the number of characters printed.
int uprintf1(char* fmt, ...)
{
	char buffer[LINE_SIZE+1];	// an internal buffer of 81 characters
	va_list argptr;		// variable arguments list

	// Create a va_list, call vsnprintf function to format the output
	va_start(argptr, fmt);
	vsnprintf(buffer, LINE_SIZE+1, fmt, argptr);
	va_end(argptr);

	// Send the formatted string to UART
	return uartPutString1(buffer);
}
