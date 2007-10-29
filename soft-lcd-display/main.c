#include "main.h"
#include "display.h"

#include <string.h>

#include "gplrus.inc"

static void io_init(void)
{
    cli();
    // PortA
    PORTA = 0x0;
    DDRA = 0x0;
    // PortB
    PORTB = 0xf0;
    DDRB = 0xff;
    // PortC
    PORTC = 0x0;
    DDRC = 0x0;
    // PortD
    PORTD = 0x0;
    DDRD = 0x0;
    // PortE
    PORTE = 0x0;
    DDRE = 0x0;

    // Analog Comparator Disabled
    ACSR = 0x80;

    MCUCR = _B(SRE);

    SPCR = _B(SPIE) | _B(SPE) | _B(MSTR);
    SPSR = _B(SPI2X);

    sei();
}

int main(void)
{
    uint8_t col = 0;
    uint8_t row = 0;
    char *pch;
    char ch;

    io_init();


    SPDR = 0x00; // Run SPI process

    clrscr();
    for(col = 0; col < 480; col++)
    {
    	setpixel(col, col >> 3);
    }
    while(1)
    {
        pch = gpl;
        while(ch = pgm_read_byte(pch++))
        {
            putc(ch);
        }
    }
}
