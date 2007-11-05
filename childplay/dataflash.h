#ifndef _DATAFLASH_H
#define _DATAFLASH_H

// Read command
// Continuous Array Read
#define DATAFLASH_CMD_CNTRDic  0x68           // Inactive Clock Polarity Low or High 68H
#define DATAFLASH_CMD_CNTRD    0xE8           // SPI Mode 0 or 3                     E8H
// Main Memory Page Read
#define DATAFLASH_CMD_PGRDic   0x52           // Inactive Clock Polarity Low or High 52H
#define DATAFLASH_CMD_PGRD     0xD2           // SPI Mode 0 or 3                     D2H
// Buffer 1 Read
#define DATAFLASH_CMD_BUF1RDic 0x54           // Inactive Clock Polarity Low or High 54H
#define DATAFLASH_CMD_BUF1RD   0xD4           // SPI Mode 0 or 3                     D4H
// Buffer 2 Read
#define DATAFLASH_CMD_BUF2RDic 0x54           // Inactive Clock Polarity Low or High 56H
#define DATAFLASH_CMD_BUF2RD   0xD4           // SPI Mode 0 or 3                     D6H
// Status Register Read
#define DATAFLASH_CMD_STATUSic 0x57           // Inactive Clock Polarity Low or High 57H
#define DATAFLASH_CMD_STATUS   0xD7           // SPI Mode 0 or 3                     D7H


// Write command
#define DATAFLASH_CMD_BUF1WR    0x84        // Buffer 1 Write
#define DATAFLASH_CMD_BUF2WR    0x87        // Buffer 2 Write
#define DATAFLASH_CMD_BUF1MME   0x83        // Buffer 1 to Main Memory Page Program with Built-in Erase
#define DATAFLASH_CMD_BUF2MME   0x86        // Buffer 2 to Main Memory Page Program with Built-in Erase
#define DATAFLASH_CMD_BUF1MM    0x88        // Buffer 1 to Main Memory Page Program without Built-in Erase
#define DATAFLASH_CMD_BUF2MM    0x89        // Buffer 2 to Main Memory Page Program without Built-in Erase
#define DATAFLASH_CMD_PE        0x81        // Page Erase
#define DATAFLASH_CMD_BE        0x50        // Block Erase
#define DATAFLASH_CMD_WR1       0x82        // Main Memory Page Program through Buffer 1
#define DATAFLASH_CMD_WR2       0x85        // Main Memory Page Program through Buffer 2

// Additional command
#define DATAFLASH_CMD_BUF1TRAN  0x53        // Main Memory Page to Buffer 1 Transfer Any 53H
#define DATAFLASH_CMD_BUF2TRAN  0x55        // Main Memory Page to Buffer 2 Transfer Any 55H
#define DATAFLASH_CMD_BUF1CMP   0x60        // Main Memory Page to Buffer 1 Compare  Any 60H
#define DATAFLASH_CMD_BUF2CMP   0x61        // Main Memory Page to Buffer 2 Compare  Any 61H
#define DATAFLASH_CMD_REWRBUF1  0x58        // Auto Page Rewrite through Buffer 1    Any 58H
#define DATAFLASH_CMD_REWRBUF2  0x59        // Auto Page Rewrite through Buffer 2    Any 59H


// status register bits
#define DATAFLASH_STATUS_BUSY	7	// busy, write in progress
#define DATAFLASH_STATUS_COMP	6	// compared

// device constants
#define DATAFLASH_PAGESIZE		264		// 256 bytes/page

#endif
