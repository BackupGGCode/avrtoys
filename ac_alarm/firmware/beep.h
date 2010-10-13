/*
 * =====================================================================================
 *
 *       Filename:  beep.h
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

#ifndef _BEEP_H_
#define _BEEP_H_

#include "config.h"

#define _TONE(t) (F_CPU / (2L * 256L * (t)))

void beepTone(uint8_t divider);

void beepOn(void);

void beepOff(void);

#define beepHz(hz) beepTone(_TONE(hz))

#endif

