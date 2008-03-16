/*
 * =====================================================================================
 *
 *       Filename:  uart.c
 *
 *    Description:  hardware UART routines
 *
 *        Version:  1.0
 *        Created:  03.11.2007 16:30:06 MSK
 *       Revision:  $Id$
 *       Compiler:  gcc
 *
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 *
 * =====================================================================================
 */

#include "uart.h"

#include "global.h"

#ifdef ENUART

#include <util/delay.h>

void uartInit(uint16_t baud)
{
    /* Set baud rate */
    UBRRH = (unsigned char)(baud>>8);
    UBRRL = (unsigned char)baud;
    /* Enable receiver and transmit */
    UCSRB = (1<<RXEN) | (1<<TXEN);
    /* Set frame format: 8data, 2stopbit */
    UCSRC = (1<<USBS) | (3<<UCSZ0);
}

uint8_t uartGet(void)
{
    /* Wait for data to be received */
    while ( !(UCSRA & (1<<RXC)) )
       wdr();
    /* Get and return received data from buffer */
    return UDR;
}


void uartPut(uint8_t data)
{
    /* Wait for empty transmit buffer */
    while ( !( UCSRA & (1<<UDRE)) )
         wdr();
    /* Put data into buffer, sends the data */
    UDR = data;
}

#endif

