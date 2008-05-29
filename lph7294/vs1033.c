#include "vs1033.h"

#include "global.h"

#define SPI_BUSY (!(SPSR & (1<<SPIF)))
#define SPI_WAIT while(SPI_BUSY)

#define VS_WRITE 0x02
#define VS_READ  0x03

void spiInit(void)
{
    SPCR = 
        (0<<SPIE) | (1<<SPE) | // SPI enable, interrupt disable
        (0<<DORD) | // MSb first
        (1<<MSTR) | // Master mode
        (0<<CPOL) | (0<<CPHA) |
        (0<<SPR1) | (0<<SPR0); 

    SPSR = (0<<SPI2X); // Fast speed mode

}

void vsInit(void)
{
    spiInit();

    PORTC |= _B(VS_xCS) | _B(VS_xDCS) | _B(VS_xRESET);
    DDRC  |= _B(VS_xCS) | _B(VS_xDCS) | _B(VS_xRESET);

    PORTD |= _B(VS_DREQ);
    DDRD  &= ~_B(VS_DREQ);
}

uint16_t vsRead(uint8_t addr)
{
    uint16_t ret;

    vsCSon;

    SPDR = VS_READ;
    SPI_WAIT;
    SPDR = addr & 0x0F;
    SPI_WAIT;
    SPDR = 0;
    SPI_WAIT; ret = SPDR << 8;
    SPDR = 0;
    SPI_WAIT;
    ret |= SPDR;

    vsCSoff;

    return ret;
}

void vsWrite(uint8_t addr, uint16_t val)
{
    vsCSon;
    SPDR = VS_WRITE;
    SPI_WAIT;
    SPDR = addr & 0x0F;
    SPI_WAIT;
    SPDR = (addr >> 8);
    SPI_WAIT;
    SPDR = addr & 0xFF;
    SPI_WAIT;
    vsCSoff;
}

