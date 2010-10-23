/*
 * =====================================================================================
 * 
 *       Filename:  config.h
 * 
 *    Description:  global configuration file
 * 
 *        Version:  1.0
 *        Created:  Fri Mar 14 17:46:40 MSK 2008
 *       Revision:  $Id$
 *       Compiler:  avr-gcc
 * 
 *         Author:  Ivan A-R <ivan@tuxotronic.org>
 * 
 * =====================================================================================
 */

#ifndef  _CONFIG_H_
#define  _CONFIG_H_

#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>


#define WDT_ENABLE

#define PORT_SET(port, bits) { (port) |= (bits); }
#define PORT_CLR(port, bits) { (port) &= ~(bits); }

#define L_A1 PB1
#define L_A2 PB0
#define L_A3 PD6
#define L_A4 PD5

#define RELAY PD1
#define ONEWIRE PD2

/*
 * Pinout configuration
 */

inline static void SET_DDR(void)
{
    PORTA = 0x03;
    DDRA = 0;
    PORTB = (1<<L_A2) | (1<<L_A1) |
        (1<<PB6);
    DDRB  = (1<<L_A2) | (1<<L_A1) |
        (1<<PB3) | (1<<PB4) |
        (1<<PB2) | (1<<PB6) | (1<<PB7);
    PORTD = (1<<L_A4) | (1<<L_A3);
    DDRD  = (1<<L_A4) | (1<<L_A3) | (1<<RELAY);
}

//#define WDT_ENABLE

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

#define _B(x) (1<<(x))

#define _SB(x) |= _B(x)
#define _RB(x) &= ~(_B(x))

#define _SET(x, val, msk) (x) = (val | ((x) & ~(msk)))

#define HI(x) (uint8_t)((x)>>8)
#define LO(x) (uint8_t)((x) & 0xFF)

#define NOP __asm__ __volatile__ (" nop ")

#ifdef WDT_ENABLE
#   define wdr() wdt_reset()
#else
#   define wdr()
#endif


#endif   /* ----- #ifndef _CONFIG_H_  ----- */

