/*
 * =====================================================================================
 *
 *       Filename:  beep.c
 *
 *    Description:  Beep module
 *
 *        Version:  1.0
 *       Revision:  $Id$
 *       Compiler:  gcc
 *
 *         Author:  Ivan A-R <ivan@alferov.name>
 *
 *        License:  private
 *
 * =====================================================================================
 */

#include "beep.h"

#include "config.h"


void beepTone(uint16_t divider)
{
    OCR1A = divider;
    OCR1B = divider;
}

void beepOn(void)
{
    TCCR1A = (1<<COM1A0) | (0<<COM1B0) | (0<<WGM10);
    TCCR1B = (1<<WGM12) | (2<<CS10);
    //PORTB |= BUZZER;
}

void beepOff(void)
{
    TCCR1A = (0<<COM1A0) | (0<<COM1B0) | (0<<WGM10);
    TCCR1B = (1<<WGM12) | (2<<CS10);
    //PORTB &= ~BUZZER;
}

