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
#include "lcd.h"
#include "lcdcg.h"
#include "ds1621.h"


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

    /*
    for(uint8_t i=0; i<16; ++i)
        lcd_data(i+0xF0);
    lcd_gotoxy(1,2);
    */

    ds16Init();

    ds16Start();

    char outstr[32];

//    lcd_puts_P("Heating    972" "\x08" "C\n");
//    lcd_puts_P("           03:42\n");

    int16_t temp;

    while(1)
    {
        wdr();

        /*
         *procAdc();
         *procKeyboard();
         *procBuzzer();
         *procExtControl();
         *procDisplay();
         */

        temp = ds16Read();
        snprintf_P(outstr, 31, PSTR("Temp %6d" "\x08" "C\n"), temp/256);

        lcd_gotoxy(0, 2);
        lcd_puts(outstr);

    }
}


