#if (__GNUC__ * 100 + __GNUC_MINOR__) < 303
#error "This library requires AVR-GCC 3.3 or later, update to newer AVR-GCC compiler !"
#endif


#include <avr/io.h>
; #include <avr/interrupt.h>
; #include <avr/signal.h>

#define LCDPORT _SFR_IO_ADDR(PORTB)
#define D1      5
#define D2      3
#define CLK     7
#define FLM     0
#define MCTRL   1
#define LAT     2

#ifndef __zero_reg__
#define __zero_reg__ 0
#endif

.global	disp_frame
.global	asm_line
.global	asm_char

.global	__vector_8
	.type	__vector_8, @function
__vector_8:
/* prologue: frame size=0 */
	push 	r24
	push 	r25
	push 	r30
	push 	r31
	in 		r31, 0x3F  ; __SREG__
	push	r31
	
	in      r31, LCDPORT
	lds     r30, asm_line
	lds     r24, asm_char
	inc     r24
	cpi     r24, 60   ; Check end of scanline
	brlo    2f
	
	sbi     LCDPORT, LAT   ; Latch scanline data
	clr     r24
	inc     r30
	cbi     LCDPORT, LAT   ; Release latching clock
	cpi     r30, 64
	brlo    1f
	
	clr     r30
	sts     asm_line, r30
	ori     r31, (1<<FLM)   ; Set film pin
	rjmp    2f

1:
	sts     asm_line, r30
	cpi     r30, 1
	brne    2f
	ldi     r25, (1<<MCTRL)  ; Invert M pin
	eor     r31, r25
	andi    r31, ~(1<<FLM)   ; Reset Film pin
2:
	out     LCDPORT, r31   ; Apply control signals changed
	sts     asm_char, r24

	mov     r31, r30
	clr     r30
	lsr     r31
	ror     r30

	clr		r25				; Get corespondig data byte from video memory
	add     r30, r24
	adc     r31, r25
	lds     r24, disp_frame
	lds     r25, disp_frame+1
	add     r30, r24
	adc     r31, r25
	ld      r24, Z
	out     _SFR_IO_ADDR(SPDR), r24

	pop		r31
	out 	0x3F, r31 ; __SREG__
	pop 	r31
	pop 	r30
	pop 	r25
	pop 	r24
	reti
	
	.size	__vector_8, .-__vector_8
