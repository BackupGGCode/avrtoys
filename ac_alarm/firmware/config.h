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
 *         Author:  Ivan A-R <ivan@alferov.name>
 * 
 * =====================================================================================
 */

#ifndef  _CONFIG_H_
#define  _CONFIG_H_

#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>

#define SYS_CLOCK
#define PKT_PREAMBULA 4
#define PKT_EXT
#define PKT_NO_SADDR
#define PKT_NO_DADDR

#define PKT_DEBUG

#define TIMER_HOOK timer_hook


#define PORT_SET(port, bits) { (port) |= (bits); }
#define PORT_CLR(port, bits) { (port) &= ~(bits); }

/*
 * Pinout configuration
 */

#define LEDR    (1<<PB0)
#define LEDG    (1<<PB1)

#define LEDR_ON()       PORT_CLR(PORTB, LEDR)
#define LEDR_OFF()      PORT_SET(PORTB, LEDR)
#define LEDG_ON()       PORT_CLR(PORTB, LEDG)
#define LEDG_OFF()      PORT_SET(PORTB, LEDG)


#define BUZZER  (1<<PB2)

inline static void SET_DDR(void)
{
    PORTA = 0xFF;
    DDRA  = 0x00;
    PORTB = 0x00;
    DDRB  = 0x03;
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

