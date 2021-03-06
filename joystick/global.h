/*
 * =====================================================================================
 * 
 *       Filename:  global.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  Fri Mar 14 17:46:40 MSK 2008
 *       Revision:  $Id$
 *       Compiler:  gcc
 * 
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 * 
 * =====================================================================================
 */

#ifndef  GLOBAL_FILE_HEADER_INC
#define  GLOBAL_FILE_HEADER_INC

#include <inttypes.h>
#include <avr/io.h>
#include <avr/wdt.h>

//#define WDTON

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

#define _B(x) (1<<(x))

#define _SB(x) |= _B(x)
#define _RB(x) &= ~(_B(x))

#define _SET(x, val, msk) (x) = (val | ((x) & ~(msk)))

#define HI(x) (uint8_t)((x)>>8)
#define LO(x) (uint8_t)((x) & 0xFF)

#define NOP __asm__ __volatile__ ("nop")

#ifdef WDTON
#   define wdr() wdt_reset()
#else
#   define wdr()
#endif

#endif   /* ----- #ifndef GLOBAL_FILE_HEADER_INC  ----- */

