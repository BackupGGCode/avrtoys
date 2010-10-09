/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Terminal bridge
 *
 *        Version:  1.0
 *        Created:  Fri Mar 14 17:43:49 MSK 2008
 *       Revision:  $Id$
 *       Compiler:  gcc
 *
 *         Author:  Ivan A-R <ivan@alferov.name>
 *
 *        License:  private
 *
 * =====================================================================================
 */

#include "config.h"

#include <string.h>
#include <stdio.h>

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#include "helpers.h"
#include "beep.h"


int main(void)
{
#ifdef WDT_ENABLE
    wdt_enable(WDTO_15MS);
#endif
    SET_DDR();

    for(;;)
    {
        wdr();
        LEDR_ON();
        delay_ms(200);
        LEDG_ON();
        delay_ms(200);
        LEDR_OFF();
        delay_ms(200);
        LEDG_OFF();
        delay_ms(200);
    }
    return 0;
}

// vim: sw=4:ts=4:si:et
