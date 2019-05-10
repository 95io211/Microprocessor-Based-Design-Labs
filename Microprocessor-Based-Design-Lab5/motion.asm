 .cdecls "stdint.h", "stdbool.h", "stdio.h", "inc/hw_memmap.h", "driverlib/pin_map.h", "driverlib/gpio.h", "driverlib/sysctl.h", "launchpad.h", "motion.h"


			.text
;			.global motionOn
;			.global motionOff

MOTION_PERIPH		.field	SYSCTL_PERIPH_GPIOC
MOTION_PORT			.field 	GPIO_PORTC_BASE
MOTION_PIN			.equ	GPIO_PIN_5

motionInit		PUSH {lr}

				LDR r0, MOTION_PERIPH
				BL SysCtlPeripheralEnable

				LDR r0, MOTION_PORT
				MOV r1, #MOTION_PIN
				BL GPIOPinTypeGPIOInput

				POP{PC}

motionOn		PUSH {lr}

				LDR r0, MOTION_PORT
				MOV r1, #MOTION_PIN
				MOV r2, #MOTION_PIN
				BL GPIOPinRead

				POP{PC}

motionOff		PUSH {lr}

				LDR r0, MOTION_PORT
				MOV r1, #MOTION_PIN
				MOV r2, #0
				BL GPIOPinRead

				POP{PC}
