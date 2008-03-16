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
#include <avr/interrupt.h>
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
// Set block len
#define MMC_CMD16 (0x40 | 16)
#define MMC_CMD17 (0x40 | 17)

#define MMC_SEC_SIZE 512

/**
 * Latest MMC responce
 */
uint8_t mmcResp;

/**
 * mmcSend - send command to card
 * CS after command and get response not deactivated.
 * cmd - command code.
 */
void mmcSend(uint8_t cmd, uint32_t parm)
{
    MMC_SEL;
    sendSpi(cmd);
    sendSpi((parm & 0xFF000000) >> 24);
    sendSpi((parm & 0xFF0000) >> 16);
    sendSpi((parm & 0xFF00) >> 8);
    sendSpi((parm & 0xFF) >> 0);
    sendSpi(0x95); // dummy CRC7. Already 0x95 (needed for MMC_CMD0)
    
    while((mmcResp = sendSpi(0xFF)) == 0xFF); // Wait for response
}

void mmcInit(void)
{
    // Send more than 74 clocks to MMC
    MMC_UNSEL;
    for(uint8_t i = 0; i < 10; i++)
        sendSpi(0xFF);

    mmcSend(MMC_CMD0, 0); // CMD0 - reset
    MMC_UNSEL;

    do {
        // MMC_CMD1 init
        mmcSend(MMC_CMD1, 0); // Get card responce
        MMC_UNSEL;
    } while(mmcResp & _B(MMC_RESP_IDLE));
}

#define mmcBlockLen(len) { mmcSend(MMC_CMD16, len); MMC_UNSEL; }

/**
 * Initialize read data from mmc
 * After all read sequences must be execute MMC_UNSEL
 */
uint8_t mmcRead(void* dst, uint32_t srcAddr, uint16_t len)
{
    // MMC_CMD17
    mmcSend(MMC_CMD17, srcAddr);
    if(mmcResp == 0x00) // If no errors
    {
        while((mmcResp = sendSpi(0xFF)) == 0xFF); // Wait for data token

        if(mmcResp == 0xFE) // No errors
        {
            // Read sector - 512 bytes
            do
            {
                *((uint8_t*)dst) = sendSpi(0xFF); // Read into mmcResp
                dst++;
            }
            while((--len)>0);
            return(0); // No errors
        }
        return(mmcResp | 0x80); // If 7th bit is set, is on read error
    }
    return(mmcResp);
}


/**
 * Countinue mmcRead
 */
void mmcReadCnt(void *dst, uint16_t len)
{
    do
    {
        *((uint8_t*)dst) = sendSpi(0xFF); // Read into mmcResp
        dst++;
    }
    while((--len)>0);
}

void mmcReadDummy(uint16_t len)
{
    do
    {
        sendSpi(0xFF); // Dummy read
    }
    while((--len)>0);
}

uint16_t mmcReadCRC(void)
{
    uint16_t crc16 = (sendSpi(0xFF) << 8);
    crc16 |= sendSpi(0xFF);
    MMC_UNSEL;
    return(crc16);
}


#define mmcClosedRead(dst, srcAddr, len) { mmcRead(dst, srcAddr, len); MMC_UNSEL; }
#define mmcClosedReadCnt(dst, len) { mmcReadCnt(dst, len); MMC_UNSEL; }

#define FAT_ROOT_ENTRY_SIZE 32

#define FAT_ATTR_RO         0x01
#define FAT_ATTR_HIDDEN     0x02
#define FAT_ATTR_SYSTEM     0x04
#define FAT_ATTR_LABEL      0x08
#define FAT_ATTR_DIRECTORY  0x10
#define FAT_ATTR_ARCHIVE    0x20

typedef struct {
    char name[8];
    char ext[3];
    uint8_t attr;
    char reserved[10];
    uint16_t time; // Time 5/6/5 bits, for hour/minutes/doubleseconds
    uint16_t date; // Date 7/4/5 bits, for year-1980/month/day
    uint16_t cluster; // Starting cluster. 0 - empty file
    uint32_t filesize; // in bytes
} directoryEntry;

typedef directoryEntry* pDirectoryEntry;

uint8_t secPerCluster;
uint16_t rootDirEntries;
uint16_t fatsize;

uint32_t fatOffset;
uint32_t rootDirOffset;
uint32_t zeroClusterOffset;

void fatInit(void)
{
    uint16_t bytePerSec;

    uint32_t partition;
    uint16_t reservedSects; // PBR Reserved sectors
    uint8_t fats;

    mmcBlockLen(4);
    mmcRead(&partition, 0x01BE + 0x08, 4); // Read number of sectros between the MBR and the first sector in the partition
    mmcReadCRC();

    partition *= MMC_SEC_SIZE; // Sectors to linear. Sector on MMC alway 512 bytes

    mmcBlockLen(8);
    mmcRead(&bytePerSec, partition+11, 2); // Read bytes per sector from PBR

    mmcReadCnt(&secPerCluster, 1); // Read sectors per cluster
    mmcReadCnt(&reservedSects, 2);
    mmcReadCnt(&fats, 1); // Number of FAT copies
    mmcReadCnt(&rootDirEntries, 2);
    mmcReadCRC();
    
    mmcBlockLen(2);
    mmcRead(&fatsize, partition+22, 2);
    mmcReadCRC();

    fatOffset = partition + reservedSects * MMC_SEC_SIZE;
    rootDirOffset = fatOffset + (uint32_t)fatsize * fats * MMC_SEC_SIZE;
    zeroClusterOffset = rootDirOffset + (uint32_t)rootDirEntries*FAT_ROOT_ENTRY_SIZE; // - (uint32_t)secPerCluster*MMC_SEC_SIZE*2;
}

uint16_t fatCurrentEntryCnt = 0;
directoryEntry fatCurrentEntry;

uint16_t fatReadNextEntry(void)
{
    if(fatCurrentEntryCnt >= rootDirEntries)
        return(0xFFFF);

    mmcBlockLen(32);
    mmcRead(&fatCurrentEntry, rootDirOffset + (uint32_t)fatCurrentEntryCnt*FAT_ROOT_ENTRY_SIZE, 32);
    mmcReadCRC();

    fatCurrentEntryCnt++;
    return(fatCurrentEntryCnt);
}

// FIFO buffer

uint8_t fifoBuffer[16];
volatile uint8_t fifoHead = 0; // Pushed here
volatile uint8_t fifoTail = 0; // Poped here

uint8_t fifoPush(uint8_t val)
{
    if(((fifoHead+1) & 0x0F) != fifoTail)
    {
        // Buffer is not full
        fifoBuffer[fifoHead] = val;
        fifoHead = (fifoHead+1) & 0x0F;
        return(0);
    }
    else // Buffer is full
        return(0xFF);
}

uint8_t fifoWaitPush(uint8_t val)
{
    while(((fifoHead+1) & 0x0F) == fifoTail);
    // Buffer is not full
    fifoBuffer[fifoHead] = val;
    fifoHead = (fifoHead+1) & 0x0F;
}

/*
uint8_t fifoPop(void)
{
    uint8_t val;
    if(fifoTail != fifoHead)
    {
        val = fifoBuffer[fifoTail];
        fifoTail = (fifoTail+1) & 0x0F;
        return(val);
    }
    else
        return(0);
}
*/


/**
 * DELAY_COEF 3 - ~8000hz
 * DELAY_COEF 1 - ~16000hz
 */
#define DELAY_COEF 1

uint8_t timerDelay = DELAY_COEF;

ISR(SIG_OVERFLOW0)
{
    if(!timerDelay)
    {
        if(fifoTail != fifoHead)
        {
            OCR0A = fifoBuffer[fifoTail];
            fifoTail = (fifoTail+1) & 0x0F;
        }
        else
            OCR0A = 128;
        timerDelay = DELAY_COEF;
    }
    else
        timerDelay--;
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

    // Interrupts
    TIMSK = _B(TOIE0);

    // Init  Uart
    uartInit(UART9600);

    mmcInit();
    fatInit();

    sei();
    // Main loop

    //uint16_t e;
    uint8_t i;
    while(fatReadNextEntry() != 0xFFFF)
    {
        if((fatCurrentEntry.name[0] >= 32) &&
          ((fatCurrentEntry.attr & (FAT_ATTR_HIDDEN | FAT_ATTR_SYSTEM | FAT_ATTR_LABEL | FAT_ATTR_DIRECTORY)) == 0))
        {
            if(fatCurrentEntry.ext[0] = 'S')
                break;
            /*
            for(i=0; i<8; i++)
                uartPut(fatCurrentEntry.name[i]);
            uartPut('.');
            for(i=0; i<3; i++)
                uartPut(fatCurrentEntry.ext[i]);
            uartPut('\n');
            */
        }
    }
    LED1ON;
    for(i=0; i<8; i++)
        uartPut(fatCurrentEntry.name[i]);
    uartPut('\n');

    uint16_t currentCluster = fatCurrentEntry.cluster;

    uint8_t sec;
    uint16_t addr;

    while(1)
    {
        mmcBlockLen(MMC_SEC_SIZE);
        for(sec=0; sec<secPerCluster; sec++)
        {
            mmcSend(MMC_CMD17, zeroClusterOffset + ((uint32_t)currentCluster*secPerCluster + sec)*MMC_SEC_SIZE);
            if(mmcResp == 0x00) // If no errors
            {
                while((mmcResp = sendSpi(0xFF)) == 0xFF); // Wait for data token

                if(mmcResp == 0xFE) // No errors
                {
                    // Read sector - 512 bytes
                    for(uint16_t addr = 0; addr < MMC_SEC_SIZE; addr++)
                        fifoWaitPush(sendSpi(0xFF));
                    mmcReadCRC();
                }
            }
            MMC_UNSEL;
        }
        if(currentCluster >= 0xFFF8)
        {
            currentCluster = fatCurrentEntry.cluster;
        }
        else
        {
            mmcBlockLen(2);
            mmcRead(&currentCluster, fatOffset + (uint32_t)currentCluster*2, 2);
            mmcReadCRC();
        }
    }
}


