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

#define CGCNT 4
static prog_uchar cgdata[CGCNT*8] = {
    0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00,
    0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, 0x00,
    0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x00,
    0x1F, 0x01, 0x01, 0x01, 0x01, 0x01, 0x1F, 0x00,
};

void cgInit(void)
{
    lcd_command(0x40 | 0x00);
    for(uint8_t i=0; i < (CGCNT*8); ++i)
        lcd_data(pgm_read_byte(cgdata + i));
    lcd_command(0x80 | 0x00);
}

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


