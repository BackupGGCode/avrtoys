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


void beepTone(uint8_t divider)
{
    OCR0A = divider;
}

void beepOn(void)
{
    TCCR0A = (0<<COM0A1) | (1<<COM0A0) | (1<<WGM01);
    TCCR0B = (4<<CS00);
    //PORTB |= BUZZER;
}

void beepOff(void)
{
    TCCR0A = (0<<COM0A1) | (0<<COM0A0) | (1<<WGM01);
    TCCR0B = (4<<CS00);
    //PORTB &= ~BUZZER;
}

