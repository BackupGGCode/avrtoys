/******************************************

 *      File Name  = main.c.c
 *
 *      Copyright (c) 2004
 *
 *  @author Ivan A. Alferov
 *  @date

******************************************/

#define F_CPU 8000000UL  // 8 MHz

#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/wdt.h>
#include <avr/delay.h>


#define _B(x) (1<<(x))

#define DISPFREQ 200
#define _TIMER_COEF (255-(F_CPU/256)/DISPFREQ)

prog_char image[] = {
    0x46, 0xCE, 0x9A, 0x92, 0xF2, 0x66, 0x00, 0x00,
    0x7C, 0xC6, 0x82, 0x82, 0xC6, 0x7C, 0x00, 0x00,
    0x7C, 0xC6, 0x82, 0x82, 0xC6, 0x7C, 0x00, 0x00,
    0xC0, 0xC0, 0x86, 0x9E, 0xF8, 0xF0, 0x00, 0x00

/*    0x23, 0xE7, 0xCF, 0xCB, 0xFB, 0x33, 0x00, 0x00,
    0x3C, 0xFF, 0xC3, 0xC3, 0xFF, 0x3C, 0x00, 0x00,
    0x3C, 0xFF, 0xC3, 0xC3, 0xFF, 0x3C, 0x00, 0x00,
    0x3c, 0xff, 0xD3, 0xD3, 0xDF, 0x0C, 0x00, 0x00
*/
};

void display_init(void)
{
// Timer0 initialization
    TCCR0B = 0x00; //stop timer
    TCNT0 = _TIMER_COEF; //set count
    OCR0A = 0x80; //set count
    OCR0B = 0x80; //set count
    TCCR0A = 0x00; //start timer
    TCCR0B = 0x04; //start timer
}

void init_devices(void)
{
    cli(); //disable all interrupts
// Watchdog timer init
    wdt_enable(WDTO_1S);

    PORTB = 0xFF; // Leds off
    DDRB  = 0xFF; // Enable LEDS

    PORTD = 0xFF;
    DDRD = 0x00; // All reads

//    MCUCR = 0x00;
//    GIMSK = 0x00;

//	TIMSK = 0x82;

    cli(); //re-enable interrupts
}

INTERRUPT(SIG_TIMER0_OVF)
{
    TCNT0 = _TIMER_COEF; //reload counter value

}

uint8_t i;
uint16_t d;

// Main function
int main(void)
{
	init_devices();

    while(1) {
        if (!(PIND & (2 << 2)))
		{
			for(d=0; d<3000; d++) { wdt_reset(); __asm__ __volatile__ ("\tnop\n"); }
			for(i=0; i<32; i++)
			{
				PORTB = ~pgm_read_byte(image + i);
				for(d=0; d<1300; d++) { wdt_reset(); } // __asm__ __volatile__ ("\tnop\n"); }
			}
		}
/*
		if (!(PIND & (1 << 2)))
		{
			for(i=0; i<32; i++)
			{
				PORTB = ~pgm_read_byte(image + 31 - i);
				for(d=0; d<1300; d++) { wdt_reset(); } // __asm__ __volatile__ ("\tnop\n"); }
			}
		}
*/
        PORTB = 0xFF;
    }
}
