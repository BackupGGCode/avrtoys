/*
 * =====================================================================================
 *
 *       Filename:  display.c
 *
 *    Description:  HT1613 display driver
 *
 *        Version:  1.0
 *        Created:  20.08.2007 03:26:04 MSD
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 *
 * =====================================================================================
 */

#include "display.h"

#include <util/delay.h>
#include "global.h"

#define DISP_SIZE 10

void disp_put(uint8_t digit)
{
    uint8_t i;

    PORTB _SB(PB3);
    DDRB _SB(PB2);
    DDRB _SB(PB3);

    for(i=0; i<4; i++)
    {
        if(digit & 8)
            PORTB _SB(PB2);
        else
            PORTB _RB(PB2);
        _delay_us(10);
        PORTB _RB(PB3);
        _delay_us(10);
        PORTB _SB(PB3);
        digit <<= 1;
    }
    _delay_us(50);
}

void disp_put_str(uint8_t *str)
{
    uint8_t i;
    for(i=0; i<DISP_SIZE; i++)
       disp_put(*str++);
} 

void disp_digit(uint32_t digit, uint8_t *str)
{
    int8_t i;
    uint8_t d;
    for(i=DISP_SIZE-1; i>=0; i--)
    {
        if(!digit) break;
        d = digit % 10;
        if(d == 0) d = 10;
        str[i] = d; 
        digit /= 10;
    }
    if(i == DISP_SIZE-1)
        str[i--] = 10;

    while(i >= 0)
        str[i--] = 0;
}

