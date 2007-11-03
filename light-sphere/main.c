/******************************************

 *      File Name  = main.c.c
 *
 *      Copyright (c) 2004
 *
 *  @author Ivan A. Alferov
 *  @date Sunday, February 20, 2005

******************************************/

#define F_CPU 8000000UL  // 4 MHz

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
// #include <avr/wdt.h>
#include <avr/delay.h>

#include "sintab.h"

#if defined (__AVR_ATtiny13__)
#	define LRED PB0
#	define LGREEN PB1
#	define LBLUE PB2
#else
#	define LRED PD5
#	define LGREEN PD4
#	define LBLUE PD6
#endif

#define _B(x) (1<<(x))

#define DISPFREQ 200
#define _TIMER_COEF (255-(F_CPU/256)/DISPFREQ)

uint8_t red = 0;
uint8_t green = 85;
uint8_t blue = 171;

volatile uint8_t vred = 0;
volatile uint8_t vgreen = 0;
volatile uint8_t vblue = 0;

void display_init(void)
{
// Timer0 initialization
#if defined (__AVR_AT90S2313__)
    TCCR0 = 0x00; //stop timer
    TCNT0 = _TIMER_COEF; //set count
    TCCR0 = 0x04; //start timer
#elif defined (__AVR_ATtiny2313__)
    TCCR0B = 0x00; //stop timer
    TCNT0 = _TIMER_COEF; //set count
    OCR0A = 0x80; //set count
    OCR0B = 0x80; //set count
    TCCR0A = 0x00; //start timer
    TCCR0B = 0x04; //start timer
#elif defined (__AVR_ATtiny13__)
    TCCR0B = 0x00; //stop timer
    TCNT0 = _TIMER_COEF; //set count
    OCR0A = 0x80; //set count
    OCR0B = 0x80; //set count
    TCCR0A = 0x00; //start timer
    TCCR0B = 0x05; //start timer
#endif
}

void init_devices(void)
{
    cli(); //disable all interrupts
// Watchdog timer init
//    wdt_enable(WDTO_1S);

// Port initializations
#if defined (__AVR_ATtiny13__)
    PORTB = 0x00;
    DDRB = _B(LRED) | _B(LGREEN) | _B(LBLUE);
#else
    PORTD = 0x00;
    DDRD = _B(LRED) | _B(LGREEN) | _B(LBLUE);
#endif
	
// Devices initializations
//
	display_init();

    MCUCR = 0x00;
    GIMSK = 0x00;

#if defined (__AVR_ATtiny13__)
	TIMSK0 = 0x02;
#else
	TIMSK = 0x82;
#endif
	
    sei(); //re-enable interrupts
}

#if defined (__AVR_AT90S2313__)
INTERRUPT(SIG_OVERFLOW0)
#elif defined (__AVR_ATtiny2313__)
INTERRUPT(SIG_TIMER0_OVF)
#elif defined (__AVR_ATtiny13__)
INTERRUPT(SIG_OVERFLOW0)
#else
#    error "device not support"
#endif
{
    TCNT0 = _TIMER_COEF; //reload counter value
    vred = pgm_read_byte(sin + red);
    vgreen = pgm_read_byte(sin + green);
    vblue = pgm_read_byte(sin + blue);
    red++;
    green++;
    blue++;
}

// Main function
int main(void)
{
	uint8_t cnt = 0;
	init_devices();
	while (1)
    {
#if defined (__AVR_ATtiny13__)
		PORTB =
#else
		PORTD =
#endif
			((cnt < vred)?_B(LRED):0) | 
			((cnt < vgreen)?_B(LGREEN):0) | 
			((cnt < vblue)?_B(LBLUE):0);
		cnt++;
	}
}
