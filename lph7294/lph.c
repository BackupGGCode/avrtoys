#include "lph.h"

#include "global.h"
#include "helpers.h"

#include <util/twi.h>
#include <avr/pgmspace.h>

#define P4_TWPS 1
#define C_TWBR(frq) ((F_CPU/(frq))-16)/(2*P4_TWPS)

#define _TB(reg, bit) ((reg) & _B(bit))
#define TWBUSY (TWCR & _B(TWINT))
#define TWWAIT while (!TWBUSY)

#define LPH_ADDR 0x74

#define LPH_CLEAR        0x01
#define LPH_CR           0x02
#define LPH_CONF         0x08
#define LPH_CONF_DISPLAY 0x0C
#define LPH_CONF_CURSOR  0x0A
#define LPH_CONF_BLINK   0x09
#define LPH_INIT         0x2A

#define LPH_LINE1        0x84
#define LPH_LINE2        0xC4

static void twiSendByte(uint8_t data);
static void twiSendAddr(uint8_t addr);
static void twiStart(void);
static void twiStop(void);
static void lphSendCmd(uint8_t cmd);
static void lphSendData(uint8_t data);

static void  twiSendByte(uint8_t data)
{
    TWDR = data;
    TWCR = _B(TWINT) | _B(TWEN);
    TWWAIT;
}


void twiInit(void)
{
    // Experemental speed on 8mhz
    // fSLC = 15khz
    TWBR = 64;
    TWSR = (0<<TWPS1) | (1<<TWPS0);

    TWCR = (1<<TWEN) |                                 // Enable TWI-interface and release TWI pins.
         (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt.
         (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
         (0<<TWWC);
}

static void lphSendCmd(uint8_t cmd)
{
    twiSendByte(0x80);
    twiSendByte(cmd);
}

static void lphSendData(uint8_t data)
{
    twiSendByte(0xC0);
    twiSendByte(data);
}

void lphInit(void)
{
    twiSendAddr(LPH_ADDR);

    lphSendCmd(LPH_CLEAR);
    lphSendCmd(LPH_CONF_DISPLAY);
    lphSendCmd(LPH_INIT);
    lphSendCmd(LPH_LINE1);
    
    twiStop();
}

void lphGotoXY(uint8_t x, uint8_t y)
{
    twiSendAddr(LPH_ADDR);

    uint8_t base;
    if((x>=0) && (x<16))
    {
        switch(y)
        {
            case 0: case 1:
                base = LPH_LINE1;
                break;
            default:
                base = LPH_LINE2;
                break;
        }
        lphSendCmd(base+x);
    }
    twiStop();
}


void lphPutc(char c)
{
    twiSendAddr(LPH_ADDR);
    lphSendData(c);
    twiStop();
}

void lphPuts(const char* str)
{
    twiSendAddr(LPH_ADDR);
    while(*str != 0)
    {
        lphSendData(*str++);
    }
    twiStop();
}

void lphPuts_p(const char* str)
{
    char c;
    twiSendAddr(LPH_ADDR);
    while((c = pgm_read_byte(str)) != 0)
    {
        lphSendData(c);
        str++;
    }
    twiStop();
}


void lphIcon(uint8_t cmd, uint8_t data)
{
    twiSendAddr(LPH_ADDR);
    lphSendCmd (cmd);
    lphSendData(data);
    twiStop();
}

void lphIconAnt(char mask)
{
    if(mask & 0x01)
        lphIcon(0xC2, 0x10);
    else
        lphIcon(0xC2, 0x12);
    if(mask & 0x02)
        lphIcon(0x82, 0x10);
    else
        lphIcon(0x82, 0x12);
    if(mask & 0x04)
        lphIcon(0x83, 0x10);
    else
        lphIcon(0x83, 0x12);
}

void lphIconBat(char mask)
{
    if(mask & 0x01)
        lphIcon(0x96, 0x10);
    else
        lphIcon(0x96, 0x13);
    switch(mask & 0x06)
    {
        case 0x02:
            lphIcon(0x97, 0x04);
            break;
        case 0x04:
            lphIcon(0x97, 0x80);
            break;
        case 0x06:
            lphIcon(0x97, 0x40);
            break;
        default:
            lphIcon(0x97, 0x10);
    }
}

void lphIconKey(char on)
{
    if(on)
        lphIcon(0x80, 0x10);
    else
        lphIcon(0x80, 0x12);
}

void lphIconPhone(char on)
{
    if(on)
        lphIcon(0xA8, 0x10);
    else
        lphIcon(0xA8, 0x12);
}

void lphIconCircle(char on)
{
    if(on)
        lphIcon(0xC3, 0x10);
    else
        lphIcon(0xC3, 0x12);
}

void lphIconMicro(char on)
{
    if(on)
        lphIcon(0xD6, 0x10);
    else
        lphIcon(0xD6, 0x13);
}

void lphIconNote(char on)
{
    if(on)
        lphIcon(0xD7, 0x00);
    else
        lphIcon(0xD7, 0x01);
}

void lphIconWave(char on)
{
    if(on)
        lphIcon(0xD7, 0x03);
    else
        lphIcon(0xD7, 0x04);
}


void lphTest(void)
{
    twiSendAddr(LPH_ADDR); // Device address

    lphSendData(0x4C);
    lphSendData(0x50);
    lphSendData(0x48);
    lphSendData(0x37);
    lphSendData(0x32);
    lphSendData(0x39);
    lphSendData(0x34);
    lphSendData(0x2D);
    lphSendData(0x31);

    twiStop();
}

/*
void ds16Start(void)
{
    twiStart();
    if ((TWSR & 0xF8) != TW_START)
        ERROR(1, TWSR);

    twiSendByte(LPH_ADDR | TW_WRITE);
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        ERROR(2, TWSR & 0xF8);

    twiSendByte(0xEE);
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        ERROR(3, TWSR & 0xF8);

    twiStop();

    LEDON;
}

int16_t ds16Read(void)
{
    int16_t hb;

    twiStart();
    if ((TWSR & 0xF8) != TW_START)
        ERROR(11, TWSR);

    twiSendByte(LPH_ADDR | TW_WRITE);
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        ERROR(12, TWSR & 0xF8);
    twiSendByte(0xAA);
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        ERROR(13, TWSR & 0xF8);

    twiStart();
    if ((TWSR & 0xF8) != TW_REP_START)
        ERROR(14, TWSR);

    twiSendByte(LPH_ADDR | TW_READ);
    if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
        ERROR(15, TWSR & 0xF8);

    TWCR = _B(TWINT) | _B(TWEN) | (1<<TWEA);
    TWWAIT;
    if ((TWSR & 0xF8) != TW_MR_DATA_ACK)
        ERROR(16, TWSR & 0xF8);
    hb = (int8_t)(TWDR)*256;
    TWCR = _B(TWINT) | _B(TWEN) | (0<<TWEA);
    TWWAIT;
    if ((TWSR & 0xF8) != TW_MR_DATA_NACK)
        ERROR(17, TWSR & 0xF8);
    hb |= TWDR;

    twiStop();

    return hb;
}
*/

/**
 * twiSendAddr
 *  combined sequense on start condition and send address byte
 * @parm addr - address
 */
static void  twiSendAddr(uint8_t addr)
{
    TWCR = _B(TWINT) | _B(TWSTA) | _B(TWEN); // Start condition
    TWWAIT;
    TWDR = addr;
    TWCR = _B(TWINT) | _B(TWEN);
    TWWAIT;
}

/*
 *static uint8_t twiReadByte(void)
 *{
 *    return 0;
 *}
 */

static void twiStart(void)
{
    TWCR = _B(TWINT) | _B(TWSTA) | _B(TWEN); // Start condition
    TWWAIT;
}

static void twiStop(void)
{
    TWCR = _B(TWINT) | _B(TWSTO) | _B(TWEN); // Stop condition
}

