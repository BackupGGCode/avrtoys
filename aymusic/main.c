/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  18.08.2007 03:36:32 MSD
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 *
 * =====================================================================================
 */

#include "global.h"

#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//#include "fat.h"
#include "mmc.h"
#include "display.h"

#define BUS_NA PORTA = (PORTA & ~(_B(BDIR) | _B(BC1)))
#define BUS_RD PORTA = (PORTA & ~(_B(BDIR) | _B(BC1))) | (_B(BC1))
#define BUS_WR PORTA = (PORTA & ~(_B(BDIR) | _B(BC1))) | (_B(BDIR))
#define BUS_ADDR PORTA = (PORTA & ~(_B(BDIR) | _B(BC1))) | (_B(BC1) | _B(BDIR))

//#define OPDELAY NOP; NOP; NOP; NOP;
#define OPDELAY

#define ADDRESS(x) { BUS = (x); OPDELAY BUS_ADDR; OPDELAY BUS_NA; OPDELAY }
#define DATA(x) { BUS = (x); OPDELAY BUS_WR; OPDELAY BUS_NA; OPDELAY }

#define REG(addr, data) { ADDRESS(addr); DATA(data); }

/*
static prog_uint8_t melody[] = {
#include "sample.inc"
};
*/

void delay_ms(unsigned int ms)
/* delay for a minimum of <ms> */
{
        // we use a calibrated macro. This is more
        // accurate and not so much compiler dependent
        // as self made code.
        while(ms){
                _delay_ms(0.96);
                ms--;
        }
}

void reg(uint8_t addr, uint8_t data)
{
    REG(addr, data);
}

/*
void play(void)
{
    uint8_t rg;
    uint16_t ptr = 0;

    for(rg = 0; rg < 14; rg++)
    {
        reg(rg, pgm_read_byte(melody+ptr));
        ptr++;
    }

    while((rg = pgm_read_byte(melody+ptr)) != 0xFE)
    {
        ptr++;
        if(rg == 0xFF)
        {
                delay_ms(19); // Wait to next step
        }
        else
        {
            reg(rg, pgm_read_byte(melody+ptr));
            ptr++;
        }
    }
}
*/

uint8_t disp[10];

uint8_t data[512];
uint32_t current_sector = 0xFFFF;

uint8_t read_byte(uint32_t addr)
{
    uint8_t e;
    uint32_t sec = addr >> 9;
    if(sec != current_sector)
    {
        if(e = mmcRead(sec, data))
        {
            disp_digit(e, disp); disp[0] = 14; disp_put_str(disp);
            while(1);
        }
        current_sector = sec;
    }
    return(data[addr & 0x01FF]);
}

void play(void)
{
    uint16_t i;
    uint8_t j;
    uint8_t timer = 50;
    uint32_t ptr = 0;

    for(i = 0; i < 9600; i++)
    {
        for(j = 0; j < 14; j++)
        {
            reg(j, read_byte(ptr++));
        }
        if(timer >= 50)
        {
            disp_digit(i/50, disp); disp_put_str(disp);
            timer = 0;
        }
        else
            timer++;
        delay_ms(20);
    }
}

int main(void)
{
    uint8_t e;

    /* Init section */
#ifdef WDTON
        wdt_enable();
#else
        wdt_disable();
#endif

    // Do init
    PORTB = 0x00; DDRB = 0x00;
    PORTD = 0x00; DDRD = 0x00;
    PORTC = 0x00; // Data 
    DDRC = 0xFF;
    PORTA = _B(RESET); // Controls
    DDRA = _B(BDIR) | _B(BC1) | _B(RESET);

    PORTA _RB(RESET);
    NOP; NOP; NOP;
    PORTA _SB(RESET);
    NOP; NOP; NOP;

    reg(0x07, 0xFF);

    reg(0x0E, 0xAA);
    reg(0x0F, 0x55);

    mmcInit();
    delay_ms(1000);
    if(e = mmcReset())
    {
        disp_digit(e, disp); disp[0] = 11; disp_put_str(disp);
        while(1);
    }

    while(1)
    {
        play();
    }
}


