 ;
 ; seg7digit.asm
 ;
 ;  Created on: Mar 6, 2019
 ;      Author: Kent
 ;

					.data
g_seg7Coding		.byte 	10111111b
					.byte 	10000110b 			; digit 1
					.byte 	11011011b			; digit 2
					.byte	11001111b			; digit 3
					.byte	11100110b			; digit 4
					.byte	11101101b			; digit 5
					.byte	11111101b			; digit 6
					.byte	10000111b			; digit 7
					.byte	11111111b			; digit 8
					.byte	11101111b			; digit 9

					; uint8_t code[4]
					.bss g_code, 4

; Function prototype: seg7DigitUpdate(uint8_t c1, uint8_t c2, uint8_t s1, uint8_t s2);
; The function encoded the digits and calls seg7Update() to display the digits on
; the 4-digit 7-segment display. The colon should always be on.					.text
					.global seg7DigitUpdate
					.global seg7Update

; TI assembler requires that symbols in data/bss sections be re-declared in code section before use
seg7Coding			.field g_seg7Coding
code				.field g_code

seg7DigitUpdate		PUSH{lr, r5}

					MOV r5, r0

					LDR r0, code ;bc code isn't a register, r0 stores the memory address of code and not the value
					LDR r12, seg7Coding
					;code[3] = seg7Coding[i] -> r12 = seg7Coding, r3 = i // code[0] = seg7Coding[i]
					;in assembly, need to LDR or LDRB of value in memory address after byte aligning it. Then store back into memory.
					LDRB r6, [r12, r3]
					STRB r6, [r0, #0]
					;code[2] = seg7Coding[k]
					LDRB r6, [r12, r2]
					STRB r6, [r0, #1]
					;code[1] = seg7Coding[j]
					LDRB r6, [r12, r1]
					STRB r6, [r0, #2]
					;code[0] = seg7Coding[l] -> r12 = seg7Coding, r5 = i and code[3] -> [r0, #3]
					LDRB r6, [r12, r5]
					STRB r6, [r0, #3]

					BL seg7Update

					POP{lr, r5}
					BX LR

					.end
