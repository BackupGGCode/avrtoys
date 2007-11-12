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
#include <string.h>

#include "uart.h"

#define DI          PB5
#define DO          PB6
#define SCLK        PB7
#define CS_MMC      PB4
#define MMC_SEL     PORTB _RB(CS_MMC)
#define MMC_UNSEL   PORTB _SB(CS_MMC)

#define RXD         PD0
#define TXD         PD1

#define SOUND       PB2

#define LED1        PB0
#define LED1ON      PORTB _RB(LED1)
#define LED1OFF     PORTB _SB(LED1)

#define LED2        PB1
#define LED2ON      PORTB _RB(LED2)
#define LED2OFF     PORTB _SB(LED2)


#define HISPI

uint8_t sendSpi(uint8_t data)
{
    USIDR = data;

#ifdef HISPI

    register uint8_t r16 = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    register uint8_t r17 = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = r16; USICR = r17; // MSB
    USICR = r16; USICR = r17;
    USICR = r16; USICR = r17;
    USICR = r16; USICR = r17;
    USICR = r16; USICR = r17;
    USICR = r16; USICR = r17;
    USICR = r16; USICR = r17;
    USICR = r16; USICR = r17; // LSB

#else

    USISR = _B(USIOIF);
    do {
        USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
    } while(!(USISR & _B(USIOIF)));

#endif

    return(USIDR);
}

uint16_t crc16(uint16_t crc, uint8_t data)
{
    uint8_t i;

    crc ^= (data << 8);
    for(i=8; i>0; i--)
        if(crc & 0x8000)
            crc = (crc<<1) ^ 0x1021;
        else
            crc = crc << 1;
    return crc;
}

#define MMC_RESP_IDLE 0
#define MMC_RESP_ERASE_RESET 1
#define MMC_RESP_ILLEGAL_CMD 2
#define MMC_RESP_CMD_CRC_ERROR 3
#define MMC_RESP_ERASE_SEQU_ERROR 4
#define MMC_RESP_ADDRESS_ERROR 5
#define MMC_RESP_PARAMETR_ERROR 6

#define MMC_CMD0  (0x40 | 0)
#define MMC_CMD1  (0x40 | 1)
#define MMC_CMD17 (0x40 | 17)


/**
 * mmcSend - send command to card
 * CS after command and get response not deactivated.
 * cmd - command code.
 */
uint8_t mmcSend(uint8_t cmd, uint32_t parm)
{
    uint8_t resp;

    MMC_SEL;
    sendSpi(cmd);
    sendSpi((parm & 0xFF000000) >> 24);
    sendSpi((parm & 0xFF0000) >> 16);
    sendSpi((parm & 0xFF00) >> 8);
    sendSpi((parm & 0xFF) >> 0);
    sendSpi(0x95); // dummy CRC7. Already 0x95 (needed for MMC_CMD0)
    
    while((resp = sendSpi(0xFF)) == 0xFF); // Wait for response

    return(resp);
}

void mmcInit(void)
{
    uint8_t resp;

    // Send more than 74 clocks to MMC
    MMC_UNSEL;
    for(resp = 0; resp < 10; resp++)
        sendSpi(0xFF);

    mmcSend(MMC_CMD0, 0); // CMD0 - reset
    MMC_UNSEL;

    do {
        // MMC_CMD1 init
        resp = mmcSend(MMC_CMD1, 0); // Get card responce
        MMC_UNSEL;
    } while(resp & _B(MMC_RESP_IDLE));
}

void mmcRead(uint32_t address)
{
    uint8_t resp;

    // MMC_CMD17
    if((resp = mmcSend(MMC_CMD17, address)) == 0x00) // If no errors
    {
        while(sendSpi(0xFF) != 0xFE); // Wait for data token

        // Read sector - 512 bytes
        for(uint16_t i = 0; i < 0x200; i++)
        {
            resp = sendSpi(0xFF);
            uartPut(resp);
        }
    }
    MMC_UNSEL;

}


int main(void)
{
    // OSCCAL = 0x6D;

    /* Init section */
#ifdef WDTON
        wdt_enable();
#else
        wdt_disable();
#endif

    // Do init
    PORTB = _B(CS_MMC) | _B(DO) | _B(DI) | _B(SOUND) | _B(LED1) | _B(LED2);
    DDRB = _B(CS_MMC) | _B(DO) | _B(SCLK) | _B(SOUND) | _B(LED1) | _B(LED2);

    PORTD = _B(RXD) | _B(TXD);
    DDRD = _B(TXD);

    // Init T0
    TCCR0A = (2<<COM0A0) | (3<<COM0B0) | (3<<WGM00);
    TCCR0B = (0<<WGM02) | (1<<CS00);

    // Init  Uart
    uartInit(UART9600);

    mmcInit();
    mmcRead(0);
    LED1ON;

    while(1)
    {
    }
}


