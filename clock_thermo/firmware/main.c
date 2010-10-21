/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Thermostat
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

#include "config.h"

#include <string.h>
#include <stdio.h>

#include "helpers.h"
#include "led7.h"
#include "beep.h"

void putInt32(uint32_t val)
{
    signed char i, c;
    for(i=3; i>=0; i--)
    {
        if(val)
        {
            c = val % 10;
            val /= 10;
            led7Char(i, c);
        }
        else
            led7Char(i, 16);
    }
}

int main(void)
{
#ifdef WDT_ENABLE
    wdt_enable(WDTO_1S);
#endif

    SET_DDR();
    led7Init();
    sei();

    beepHz(440);

    for(;;)
    {
        wdr();
        putInt32(seconds);
        led7Dot(2, (msec<500)?0:1);
        led7Dot(3, (msec<500)?1:0);
        if(msec<10)
            beepOn();
            //PORTD |= (1<<RELAY);
        else
            beepOff();
            //PORTD &= ~(1<<RELAY);
        //_delay_ms(1);
    }
    return 0;
}

// vim: sw=4:ts=4:si:et
