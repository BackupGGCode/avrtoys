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


uint8_t keyCheck(uint8_t key);

static uint8_t key_down;
uint8_t keyCheck(uint8_t key)
{
    uint8_t ckey = GET_KEYS();
    uint8_t diffkey = ckey ^ key_down;
    key_down = ckey;
    return ckey & diffkey & key;
}


/*****************************************************/
#define DS1307_ADDR 0xD0

/* TODO error checks */
uint8_t dsRead(uint8_t reg)
{
    twiStart();
    twiSend(DS1307_ADDR | TWI_WR);
    twiSend(reg);
    twiStop();

    twiStart();
    twiSend(DS1307_ADDR | TWI_RD);
    uint8_t data = twiReceive(NACK);
    twiStop();
    return data;
}

void dsReadBuf(uint8_t reg, char* buf, uint8_t len)
{
    twiStart();
    twiSend(DS1307_ADDR | TWI_WR);
    twiSend(reg);
    twiStop();

    twiStart();
    twiSend(DS1307_ADDR | TWI_RD);
    while(--len)
    {
        *buf++ = twiReceive(ACK);
    }
    *buf++ = twiReceive(NACK);
    twiStop();
}

void dsWrite(uint8_t reg, uint8_t data)
{
    twiStart();
    twiSend(DS1307_ADDR | TWI_WR);
    twiSend(reg);
    twiSend(data);
    twiStop();
}

uint8_t dsdata[16];

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

        dsReadBuf(0, dsdata, 3);

        led7Char(0, (dsdata[2]>>4)&3);
        led7Char(1, dsdata[2]&15);
        led7Char(2, dsdata[1]>>4);
        led7Char(3, dsdata[1]&15);
        led7Dot (2, (dsdata[0]&1));
        led7Dot (3, (dsdata[0]&1));

        uint8_t keys = keyCheck(7);
        if(keys)
        {
            if(keys & 1)
            {
                uint8_t hour = dsRead(2) & 0x3F;
                hour++;
                if((hour & 0x0F) >= 10)
                {
                    hour = ((hour&0x30)+0x10);
                    if(hour > 0x20) hour = 0;
                    hour |= 0x40;
                }
                dsWrite(2, hour);
            }
            if(keys & 2)
            {
                uint8_t min = dsRead(1) & 0x7F;
                min++;
                if((min & 0x0F) >= 10)
                {
                    min = ((min&0x70)+0x10);
                    if(min > 0x60) min = 0;
                }
                dsWrite(1, min);
            }
        }
    }
    return 0;
}

// vim: sw=4:ts=4:si:et
