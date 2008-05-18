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

#include "helpers.h"
#include "lcd.h"
#include "lcdcg.h"

int main(void)
{
#ifdef WDTON
    wdt_enable(WDTO_15MS);
#endif

    lcd_init(LCD_DISP_ON);

    cgInit();

    lcd_clrscr();

    /*
    for(uint8_t i=0; i<16; ++i)
        lcd_data(i+0xF0);
    lcd_gotoxy(1,2);
    */
    lcd_puts_P("Heating    972" "\x08" "C\n");
    lcd_puts_P("           03:42\n");

    while(1)
    {
        wdr();

    }
}


