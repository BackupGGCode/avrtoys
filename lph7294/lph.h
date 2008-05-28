#ifndef _MYTWI_H_
#define _MYTWI_H_

#include "global.h"
#include <avr/pgmspace.h>

void twiInit(void);

/*
 *void lphSendCmd(uint8_t cmd);
 *void lphSendData(uint8_t data);
 */

void lphInit(void);

void lphGotoXY(uint8_t x, uint8_t y);

void lphPutc(char c);
void lphPuts(const char* str);
void lphPuts_p(const char* str);

void lphIcon(uint8_t cmd, uint8_t data);
void lphIconKey(char on);
void lphIconPhone(char on);
void lphIconCircle(char on);
void lphIconMicro(char on);
void lphIconNote(char on);
void lphIconWave(char on);
void lphIconBat(char mask);
void lphIconAnt(char mask);

void lphTest(void);

#endif

