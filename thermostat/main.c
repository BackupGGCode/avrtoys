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

int main(void)
{
#ifdef WDTON
    wdt_enable(WDTO_15MS);
#endif

    lcd_init(LCD_DISP_ON);

    lcd_command(0x40 | 0x00);
    lcd_data(0x06);
    lcd_data(0x09);
    lcd_data(0x09);
    lcd_data(0x06);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_data(0x00);
    lcd_command(0x80 | 0x00);

    lcd_clrscr();

    for(uint8_t i=8; i<16; ++i)
        lcd_write(i, 1);
    lcd_puts("Hello world!\n");

    while(1)
    {
        wdr();

    }
}


