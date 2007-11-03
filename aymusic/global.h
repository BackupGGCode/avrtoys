/*
 * =====================================================================================
 * 
 *       Filename:  global.h
 * 
 *    Description:  
 * 
 *        Version:  1.0
 *        Created:  18.08.2007 03:47:01 MSD
 *       Revision:  none
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

#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond

// Ports
#define BUS PORTC
#define RESET PA5
#define BDIR PA6
#define BC1 PA7

//#define WDTON

#define _B(x) (1<<(x))

#define _SB(x) |= _B(x)
#define _RB(x) &= ~(_B(x))

#define HI(x) (uint8_t)((x)>>8)
#define LO(x) (uint8_t)((x) & 0xFF)

#define NOP __asm__ __volatile__ ("nop")

#endif   /* ----- #ifndef GLOBAL_FILE_HEADER_INC  ----- */

