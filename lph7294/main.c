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

#include <stdio.h>

#include "helpers.h"
#include "lph.h"
#include "vs1033.h"


#define LEDG PB0
#define LEDR PB1

#define LEDRon  PORTB _RB(LEDR)
#define LEDRoff PORTB _SB(LEDR)

#define LEDGon  PORTB _RB(LEDG)
#define LEDGoff PORTB _SB(LEDG)

int main(void)
{
#ifdef MYWDTON
    wdt_enable(WDTO_15MS);
#endif

    DDRB  = _B(LEDG) | _B(LEDR);
    PORTB = _B(LEDG) | _B(LEDR);

    twiInit();
    delay_ms(100);

    lphInit();
    lphTest();

    uint16_t count = 0;
    char buf[20];

    vsInit();

    uint16_t mode = vsRead(SCI_MODE);
    LEDRon;
    sprintf_P(buf, PSTR("Mode 0x%04x"), mode);
    lphGotoXY(0,1);
    lphPuts(buf);

    uint16_t stat = vsRead(SCI_STATUS);
    LEDGon;
    sprintf_P(buf, PSTR("Status 0x%04x"), stat);
    lphGotoXY(0,2);
    lphPuts(buf);

    while(1)
    {
        wdr();

        /*
         *sprintf_P(buf, PSTR("Sec %6d."), count++);
         *lphGotoXY(0,2);
         *lphPuts(buf);
         *delay_ms(100);
         */

    }
}


