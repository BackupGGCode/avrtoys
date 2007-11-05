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
#include "dataflash.h"

#define MOSI        PB5
#define MISO        PB6
#define SCLK        PB7
#define CS_FLASH    PB4
#define FLASH_SEL   PORTB _RB(CS_FLASH)
#define FLASH_UNSEL PORTB _SB(CS_FLASH)

#define RXD         PD0
#define TXD         PD1

#define SOUND       PB2

#define LED1        PB0
#define LED1ON      PORTB _RB(LED1)
#define LED1OFF     PORTB _SB(LED1)

#define LED2        PB1
#define LED2ON      PORTB _RB(LED2)
#define LED2OFF     PORTB _SB(LED2)


uint8_t sendSpi(uint8_t data)
{
    USIDR = data;

    /*
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    USICR = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    */

    USISR = _B(USIOIF);
    do {
        USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
    } while(!(USISR & _B(USIOIF)));

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

typedef struct {
    uint16_t bufferAddr;
    uint16_t pageAddr;
} flashMark;
typedef flashMark* pFlashMark;

flashMark flashCurrent;
uint8_t flashFlag = 0;

/*
void flashPush(uint8_t data)
{
    switch(flashFlag & 0x7F)
    {
        case 0: // Init to buffer transfer
            FLASH_SEL;
            if(flashFlag & 0x80)
                sendSpi(DATAFLASH_CMD_BUF1WR)
            else
                sendSpi(DATAFLASH_CMD_BUF2WR);
            sendSpi(0);
            sendSpi(0); // Buffer start 0
            sendSpi(0);
            flashCurrent.bufferAddr = 0;
            flashFlag = (flashFlag & 0x80) | 1;
            break;
        case 1: // Transfer in progress
            



    }
}

void flashStore(pFlashMark mark)
{
    memcpy(&flashCurrent, mark, 4);
}

void flashRestore(pFlashMark mark)
{
    memcpy(mark, &flashCurrent, 4);
}

flashMark flashStored;
*/

void flashWait(void)
{
    uint8_t st;
    LED1ON;
    do {
        FLASH_SEL;
        sendSpi(DATAFLASH_CMD_STATUS);
        st = sendSpi(0);
        FLASH_UNSEL;
    } while( ! (st & _B(DATAFLASH_STATUS_BUSY)) );
    LED1OFF;
}

// Start of Header     0x01
#define SOH     0x01
// End of Transmission 0x04
#define EOT     0x04
// Acknowledge         0x06
#define ACK     0x06
// Not Acknowledge     0x15
#define NAK     0x15

#define XON     0x11
#define XOFF    0x13
#define CAN     0x18

void xmodemFile(void)
{
    uint32_t delay = 0;
    uint8_t started = 0;

    uint16_t crc = 0; // Calculated CRC16
    uint16_t rcrc; // Received CRC16
    uint8_t packNum, nPackNum; // Packet Number and negative packet number
    uint8_t prevPackNum = 0; 

    uint8_t i; // Data index
    uint8_t data = 0; // Received char

    flashCurrent.bufferAddr = 0;
    flashCurrent.pageAddr = 0;

    uint8_t needBuffer = 1; // Flag if needed load buffer

    while(1)
    {
        if(uartSTATE)
        {
            data = uartGet();
            if(data == EOT)
            {
                break;
            }
            if(data == SOH)
            {
                started = 1;
                crc = 0;

                // flashStore(&flashStored);

                // Receive xmodem block (128 bytes of data + headers)
                packNum = uartGet();
                nPackNum = uartGet();

                if(needBuffer)
                {
                    FLASH_SEL;
                    if(flashFlag)
                        sendSpi(DATAFLASH_CMD_BUF1TRAN);
                    else
                        sendSpi(DATAFLASH_CMD_BUF2TRAN);
                    sendSpi(flashCurrent.pageAddr >> 7);
                    sendSpi(flashCurrent.pageAddr << 1);
                    sendSpi(0);
                    FLASH_UNSEL;

                    flashWait();
                    needBuffer = 0;
                }

                FLASH_SEL;
                if(flashFlag)
                    sendSpi(DATAFLASH_CMD_BUF1WR);
                else
                    sendSpi(DATAFLASH_CMD_BUF2WR);
                sendSpi(0);
                sendSpi((flashCurrent.bufferAddr >> 8) & 0x01);
                sendSpi(flashCurrent.bufferAddr);

                for(i=0; i<128; i++)
                {
                    data = uartGet();
                    crc = crc16(crc, data);
                    // Store data into memory
                    OCR0A = data; // TODO
                    sendSpi(data);
                }
                FLASH_UNSEL;

                rcrc = uartGet()<<8;
                rcrc |= uartGet();

                if((crc != rcrc) &&// CRC16 error
                   (packNum == prevPackNum)) // Duplicate frame
                {
                    LED2ON;
                    uartPut(NAK);
                    // flashRestore(&flashStored);
                }
                else
                {
                    flashCurrent.bufferAddr += 128;
                    if(flashCurrent.bufferAddr >= 256)
                    {
                        FLASH_SEL;
                        if(flashFlag)
                            sendSpi(DATAFLASH_CMD_BUF1MME);
                        else
                            sendSpi(DATAFLASH_CMD_BUF2MME);
                        sendSpi(flashCurrent.pageAddr >> 7);
                        sendSpi(flashCurrent.pageAddr << 1);
                        sendSpi(0);
                        FLASH_UNSEL;

                        needBuffer = 1;
                        flashFlag ^= 0xFF;

                        flashCurrent.bufferAddr = 0;
                        flashCurrent.pageAddr += 1;
                    }

                    flashWait();

                    prevPackNum = packNum;
                    uartPut(ACK);
                }
            }
        }
        if(!started)
        {
            if(delay == 0)
            {
                uartPut('C');
                delay = 500000;
            }
            delay--;
        }
    }
    uartPut(ACK);
    FLASH_UNSEL;
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
    PORTB = 0xFF; DDRB = _B(CS_FLASH) | _B(MISO) | _B(SCLK) | _B(SOUND) | _B(LED1) | _B(LED2);
    PORTD = 0xFF; DDRD = _B(TXD);

    // Init T0
    TCCR0A = (2<<COM0A0) | (3<<COM0B0) | (3<<WGM00);
    TCCR0B = (0<<WGM02) | (1<<CS00);

    // Init  Uart
    uartInit(UART115200);

    uartPut('H');
    uartPut('e');
    uartPut('l');
    uartPut('l');
    uartPut('o');
    uartPut('.');


//    xmodemFile();

    /*while(1)
    {
    }*/

    while(1)
    {
        OCR0A = uartGet();
    }

    uint16_t page = 0;
    while(1)
    {
        FLASH_SEL;
        sendSpi(DATAFLASH_CMD_CNTRD);
        sendSpi(page >> 7); // Address
        sendSpi(page << 1);
        sendSpi(0);
        sendSpi(0); // Don't care
        sendSpi(0);
        sendSpi(0);
        sendSpi(0);
        
        for(uint16_t b=0; b<=255; b++)
        {
            OCR0A = sendSpi(0); // Read data
            _delay_us(200); // Delay for 6000 Hz
        }
        FLASH_UNSEL;
        page++;
    }
}


