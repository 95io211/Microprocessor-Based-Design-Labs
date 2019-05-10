; Assembly file for seg7DigitUpdate() function
; Lab 3, ECE 367, fall 2016
; Created by Zhao Zhang

; The 7-segment coding table
					.data
g_seg7Coding		.byte 	00111111b			; digit 0
					.byte 	00000110b 			; digit 1
					.byte 	01011011b			; digit 2
					.byte	01001111b			; digit 3
					.byte	01100110b			; digit 4
					.byte	01101101b			; digit 5
					.byte	01111101b			; digit 6
					.byte	00000111b			; digit 7
					.byte	01111111b			; digit 8
					.byte	01101111b			; digit 9

					; ADDED DIGIT VALUES FROM LAB 2

; Output code, equivalent to declare "uint8_t code[4]" as static variable
					.bss 	g_code, 4

; Function prototype: seg7DigitUpdate(uint8_t c1, uint8_t c2, uint8_t s1, uint8_t s2);
; The function encoded the digits and calls seg7Update() to display the digits on
; the 4-digit 7-segment display. The colon should always be on.
					.text
					.global seg7DigitUpdate		; make this symbol visible to outside
					.global seg7Update			; use this global symbol

; TI assembler requires that symbols in data/bss sections be re-declared in code section before use
seg7Coding			.field	g_seg7Coding
code				.field  g_code


seg7DigitUpdate		PUSH	{lr, r4, r5, r6, r7}

					; Always show 01:23 on 7-segment display
					; CHANGE THE CODE

					MOV		r4, r0	;1st parameter
					MOV		r5, r1	;2nd parameter
					MOV		r6, r2	;3rd parameter
					MOV		r7, r3	;4th parameter

					LDR		r0, code    	 ;pointer to code, r0 is what is returned
					LDR		r12, seg7Coding  ;pointer to seg7Coding

					LDRB	r9, [r12, r4]    	;r9 = r12 + 1st parameter offset
					STRB	r9, [r0, #3]		;store r9 into most significant place

					LDRB	r9, [r12, r5]		;r9 = r12 + 2nd parameter offset
					STRB	r9, [r0, #2]

					LDRB	r9, [r12, r6]		;r9 = r12 + 3rd parameter offset
					STRB	r9, [r0, #1]

					LDRB	r9, [r12, r7]		;r9 = r12 + 4th parameter offset
					STRB	r9, [r0, #0]		;stores into least significant place


					; Call seg7Update(code)
					BL		seg7Update

					POP		{lr, r4, r5, r6, r7}
					BX 		lr

					.end
