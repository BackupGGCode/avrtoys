/******************************************

 *      File Name  = display.h
 *
 *      Copyright (c) 2004
 *
 *  @author Ivan A. Alferov
 *  @date Saturday, June 04, 2005

******************************************/

#ifndef __main_H
#define __main_H

#include <inttypes.h>
//#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/signal.h>
#include <avr/pgmspace.h>

// #define F_CPU 12E6  // Define into Makefile
#include <avr/delay.h>

#define D1      PB5
#define D2      PB3
#define CLK     PB7
#define FLM     PB0
#define MCTRL   PB1
#define LAT     PB2

#define LED     PB4


#define _B(x) (1<<(x))
#define SET_B(x) |= (1<<(x))
#define CLR_B(x) &= ~(1<<(x))
#define INV_B(x) ^= (1<<(x))

#define LEDON  PORTB SET_B(LED)
#define LEDOFF PORTB CLR_B(LED)

#define DOCLK { PORTB SET_B(CLK); PORTB CLR_B(CLK); }


#endif
