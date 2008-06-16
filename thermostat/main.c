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

#include "global.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <string.h>

#include <stdio.h>

#include "helpers.h"
#include "lcd.h"
#include "lcdcg.h"
#include "adc.h"


int main(void)
{
#ifdef WDTON
    wdt_enable(WDTO_15MS);
#endif

    DDRB _SB(PB0);
    PORTB _SB(PB0);

    lcd_init(LCD_DISP_ON);

    cgInit();

    lcd_clrscr();
    lcd_puts_P("Simple demo\n");

    PORTC |= 0x03;

    adcInit();

    char outstr[32];

//    lcd_puts_P("Heating    972" "\x08" "C\n");
//    lcd_puts_P("           03:42\n");

    int16_t temp;
    int16_t temp2;

    while(1)
    {
        wdr();
        delay_ms(1000);

        /*
         *procAdc();
         *procKeyboard();
         *procBuzzer();
         *procExtControl();
         *procDisplay();
         */

        
        temp = adcRead();
        for(uint8_t i=0; i<9; i++)
            temp = (temp + adcRead())/2;

        snprintf_P(outstr, 31, PSTR("Temp 0x%04X" "\x08" "C\n"), temp);
        lcd_gotoxy(0, 2);
        lcd_puts(outstr);

    }
}

// vim: sw=4:ts=4:si:et
