/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Dendy joystick to atari adapter
 *
 *        Version:  1.0
 *        Created:  Fri Mar 14 17:43:49 MSK 2008
 *       Revision:  $Id$
 *       Compiler:  gcc
 *
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 *
 * =====================================================================================
 */

#include "global.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>

#include "helpers.h"

#define TICK 2

#define RXD   PD0
#define TXT   PD1

#define JCLK  PD2
#define JRES  PD3

#define JDATC PD4
#define JDATB PD5
#define JDATA PD6

#define LED   PD7

#define LEDON  PORTD _RB(LED)
#define LEDOFF PORTD _SB(LED)

// TODO uart!!!
//
int main(void)
{
    wdt_enable(WDTO_15MS);

    // Init
    PORTA = 0x00;
    DDRA  = 0x00;
    PORTB = 0x00;
    DDRB  = 0x00;
    PORTC = 0x00;
    DDRC  = 0x00;

    PORTD = _B(JDATA) | _B(JDATB) | _B(JDATC) | _B(JCLK) | _B(LED);
    DDRD  = _B(JRES) | _B(JCLK) | _B(LED);

    uint8_t joya;
    uint8_t joyb;
    uint8_t joyc;

    while(1)
    {
        wdt_reset();

        PORTD _SB(JRES);
        _delay_us(TICK);
        PORTD _RB(JRES);
        _delay_us(TICK);

        for(uint8_t i=0; i<8; i++)
        {
            PORTD _RB(JCLK);
            _delay_us(TICK);

            joya >>= 1;
            if(!(PIND & _B(JDATA)))
                joya |= 0x80;

            joyb >>= 1;
            if(!(PIND & _B(JDATB)))
                joyb |= 0x80;

            joyc >>= 1;
            if(!(PIND & _B(JDATC)))
                joyc |= 0x80;

            PORTD _SB(JCLK);
            _delay_us(TICK);
        }
        DDRA = joya;
        DDRB = joyb;
        DDRC = joyc;
            if(joya || joyb || joyc) // If any button pressed
            LEDON;
        else
            LEDOFF;
    }
}


