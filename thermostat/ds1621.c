#include "ds1621.h"

#include "global.h"
#include "helpers.h"

#include <util/twi.h>

#define P4_TWPS 1
#define C_TWBR(frq) ((F_CPU/(frq))-16)/(2*P4_TWPS)

#define _TB(reg, bit) ((reg) & _B(bit))
#define TWBUSY (TWCR & _B(TWINT))
#define TWWAIT while (!TWBUSY)

#define DS_ADDR (0x90 | 0x08)

static void  ds16_send_byte(uint8_t data)
{
    TWDR = data;
    TWCR = _B(TWINT) | _B(TWEN);
    TWWAIT;
    /*
     *if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
     *    ERROR();
     */
}

static uint8_t ds16_read_byte(void)
{
    return 0;
}


static void ds16_start(void)
{
    TWCR = _B(TWINT) | _B(TWSTA) | _B(TWEN); // Start condition
    TWWAIT;
    /*
     *if ((TWSR & 0xF8) != TW_START)
     *    ERROR();
     */
}

static void ds16_stop(void)
{
    TWCR = _B(TWINT) | _B(TWSTO) | _B(TWEN); // Stop condition
    /*
     *if ((TWSR & 0xF8) != TW_START)
     *    ERROR();
     */
}


void ds16Init(void)
{
    TWBR = 80; //C_TWBR(400000);
    TWSR = 0x00; // _B(TWPS1) | _B(TWPS0);
    //TWDR = 0xFF;

    TWCR = (1<<TWEN) |                                 // Enable TWI-interface and release TWI pins.
         (0<<TWIE)|(0<<TWINT)|                      // Disable Interupt.
         (0<<TWEA)|(0<<TWSTA)|(0<<TWSTO)|           // No Signal requests.
         (0<<TWWC);
}

void ds16Start(void)
{
    ds16_start();
    if ((TWSR & 0xF8) != TW_START)
        ERROR(1, TWSR);

    ds16_send_byte(DS_ADDR | TW_WRITE);
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        ERROR(2, TWSR & 0xF8);

    ds16_send_byte(0xEE);
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        ERROR(3, TWSR & 0xF8);

    ds16_stop();

    LEDON;
}

int16_t ds16Read(void)
{
    int16_t hb;

    ds16_start();
    if ((TWSR & 0xF8) != TW_START)
        ERROR(11, TWSR);

    ds16_send_byte(DS_ADDR | TW_WRITE);
    if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
        ERROR(12, TWSR & 0xF8);
    ds16_send_byte(0xAA);
    if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
        ERROR(13, TWSR & 0xF8);

    ds16_start();
    if ((TWSR & 0xF8) != TW_REP_START)
        ERROR(14, TWSR);

    ds16_send_byte(DS_ADDR | TW_READ);
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

    ds16_stop();

    return hb;
}

