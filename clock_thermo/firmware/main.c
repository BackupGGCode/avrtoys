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
#include "twi.h"

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


#define DS1307_ADDR 0xD0

int main(void)
{
#ifdef WDT_ENABLE
    wdt_enable(WDTO_1S);
#endif

    SET_DDR();
    led7Init();
    sei();

    putInt32(8888);
    beepHz(440);

    twiACK a;
    twiStart();
    a = twiSend(DS1307_ADDR | TWI_WR);
    if(a == NACK)
    {
        led7Dot(2, 1);
        twiStop();
        putInt32(1);
        for(;;) ;
    }
    //twiSend(0);
    //twiSend(19);
    twiStop();

    for(;;)
    {
        wdr();
        twiStart();
        twiSend(DS1307_ADDR | TWI_WR);
        twiSend(0);
        twiStop();

        twiStart();
        twiSend(DS1307_ADDR | TWI_RD);
        uint8_t sec = twiReceive(ACK);
        uint8_t min = twiReceive(ACK);
        uint8_t hour = twiReceive(NACK); // & 0x3F;
        twiStop();

        led7Char(0, hour>>4);
        led7Char(1, hour&15);
        led7Char(2, min>>4);
        led7Char(3, min&15);
        led7Dot(2, (sec&1));
        led7Dot(3, (sec&1));

        /*
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
        */
    }
    return 0;
}

// vim: sw=4:ts=4:si:et
