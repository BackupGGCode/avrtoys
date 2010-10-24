#include "twi.h"

#include "config.h"

#define SDA PD4
#define SCL PD3

#define SDA0() DDRD |=  (1<<SDA)
#define SDA1() DDRD &= ~(1<<SDA)
#define SCL0() DDRD |=  (1<<SCL)
#define SCL1() DDRD &= ~(1<<SCL)

#define DELAY() do { uint8_t i; for(i=16; i; i--) asm(""); } while(0)

void twiInit(void)
{
    PORTD &= ~(1<<SCL);
    PORTD &= ~(1<<SDA);
    SDA1();
    SCL1();
}

void twiStart(void)
{
    SDA0();
    SCL0();
}

void twiStop(void)
{
    SCL1();
    SDA1();
}

twiACK twiSend(uint8_t data)
{
    twiACK ack;
    uint8_t i = 7;
    do
    {
        if(data & 0x80) SDA1();
            else SDA0();
        data <<= 1;
        SCL1();
        DELAY();
        SCL0();
        DELAY();
    }
    while(i--);
    SDA1();
    SCL1();
    DELAY();
    ack = (PIND & (1<<SDA))?1:0;
    SCL0();
    return ack;
}

uint8_t twiReceive(twiACK ack)
{
    uint8_t d = 0;
    uint8_t i = 7;
    SDA1();
    do
    {
        SCL1();
        DELAY();
        d <<= 1;
        if(PIND & (1<<SDA)) d |= 1;
        SCL0();
        DELAY();
    }
    while(i--);
    if(ack == ACK) SDA0();
        else SDA1();
    SCL1();
    DELAY();
    SCL0();
    SDA1();
    return d;
}

