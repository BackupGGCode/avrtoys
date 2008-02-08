; vim: sw=4:ts=4:si:et:filetype=asm:syntax=avr

.include "tn13def.inc"
.include "avr.inc"

.equ    F_CPU       = 1200000

.equ    RxPin       = PORTB1
.equ    TxPin       = PORTB0
.equ	uartb	    = 17

.equ    SOUND       = PORTB2
.equ    LRED        = PORTB4
.equ    LGREEN      = PORTB3


.macro lredOn
    cbi     PORTB, LRED
.endm

.macro lredOff
    sbi     PORTB, LRED
.endm

.macro lgreenOn
    cbi     PORTB, LGREEN
.endm

.macro lgreenOff
    sbi     PORTB, LGREEN
.endm


.macro  ldil
    ldi r16, @1
    mov @0, r16
.endm

.org    0
    rjmp    reset

.org    TIM0_OVF0addr
    reti

reset:
    outi    SPL, RAMEND

    outi    PORTB, (1<<RxPin) | (1<<TxPin) ; | (1<<SOUND)
    outi    DDRB,  0b00011101

    lgreenOn
    rcall   beepHigh
    rcall   delay100
    lredOn
    rcall   beepHigh
    rcall   delay100
    lgreenOff
    rcall   beepHigh
    rcall   delay100
    lredOff

    ldi     r16, 0x80
    rcall   uartPut
    ldi     r16, 0x40
    rcall   uartPut
    ldi     r16, 0x20
    rcall   uartPut
    ldi     r16, 0x10
    rcall   uartPut
    ldi     r16, 0x08
    rcall   uartPut
    ldi     r16, 0x04
    rcall   uartPut
    ldi     r16, 0x02
    rcall   uartPut
    ldi     r16, 0x01
    rcall   uartPut

loop:
    rcall   uartGet
;    rcall   uartPut
;    rjmp    loop

    cpi     r16, '!' ;0x01
    brne    loop

    rcall   uartGet

checkRed:
    cpi     r16, 'R'
    brne    checkGreen
    lredOn
    rcall   beepLow
    rcall   beepLow
    rcall   beepLow
    rcall   beepLow
    rcall   beepLow
    rcall   beepHigh
    rcall   delay250
    lredOff
    ldi     r16, 'r'
    rcall   uartPut
    rjmp    loop

checkGreen:
    cpi     r16, 'G'
    brne    checkBeep
    lgreenOn
    rcall   beepHigh
    rcall   beepLow
    rcall   beepHigh
    rcall   delay250
    lgreenOff
    ldi     r16, 'g'
    rcall   uartPut
    rjmp    loop

checkBeep:
    cpi     r16, 'B'
    brne    checkOff
    ;; Need beep
    ldi     r16, 'b'
    rcall   uartPut
    rjmp    loop

checkOff:
    cpi     r16, '0'
    brne    checkEnd
    lgreenOff
    lredOff
    ldi     r16, '0'
    rcall   uartPut
    rjmp    loop

checkEnd:

    rjmp    loop


beepLow:
    ldiw    B, 50 
    ldiw    C, 500
    rjmp    beep

beepHigh:
    ldiw    B, 25 
    ldiw    C, 1000

;;; Some functions ;;;

; Beep
; B - tone
; C - delay
beep:
    movew   Z, C
    ldil    T6L, (1<<SOUND)

beep1:
    movew   Y, B
beep2:
    wdr                 ; 1
    subiw   Y, 1        ; 2
    brcc    beep2       ; 2/1

    in      r16, PORTB
    eor     r16, T6L
    out     PORTB, r16

    subiw   Z, 1
    brcc    beep1

    in      r16, PORTB
    andi    r16, ~(1<<SOUND)
    out     PORTB, r16

    ret
    
       
; Delay in ms
delay100:
    ldi     r16, 100
    rjmp    delay
delay250:
    ldi     r16, 250
delay:
    ldi     r17, 240
delay1:
    wdr                 ; 1
    nop                 ; 1
    dec     r17         ; 1
    brne    delay1      ; 1/2
    dec     r16
    brne    delay
    ret


; Uart put
; in: R16

uartPut:
    ldi     r17, 1+8+2	        ;1+8+sb (sb is # of stop bits)
    com     r16		            ;Inverte everything
    sec		        	        ;Start bit

putchar0:
    brcc	putchar1	        ;If carry set
;   cbi     PORTB, TxPin        ;   send a '0' en RS232 mode
    cbi     PORTB, TxPin        ;   send a '0' en TTL mode
    rjmp    putchar2	        ;else	

putchar1:
;   sbi     PORTB, TxPin        ;    send a '1' en RS232 mode
    sbi     PORTB, TxPin        ;    send a '1' en TTL mode
    nop

putchar2:
    rcall   uartDelay	        ; One bit delay
    rcall   uartDelay

    lsr     r16 		        ;Get next bit
    dec     r17 		        ;If not all bit sent
    brne    putchar0	        ;   send next
                                ;else
    ret                         ;   return

; Uart Get
; Return: r16
uartGet:
    ldi 	r17, 9	;8 data bit + 1 stop bit

uartGet1:
    wdr
    sbic 	PINB, RxPin	;Wait for start bit
    rjmp 	uartGet1

    rcall   uartDelay	    ;0.5 bit delay

uartGet2:
    rcall   uartDelay	    ;1 bit delay
    rcall   uartDelay

    clc			        ;clear carry
    sbic 	PINB, RxPin	;if RX pin high
    sec			;

    dec 	r17		;If bit is stop bit
    breq 	uartGet3	;   return
                ;else
    ror 	r16		;   shift bit into Rxbyte
    rjmp 	uartGet2	;   go get next

uartGet3:
    ret

; uart Delay
uartDelay:
    ldi	    r18, uartb         ; 1
uartDelay1:
    dec	    r18            ; 1
    brne	uartDelay1      ; 1/2
    ret                     ; 2

    
