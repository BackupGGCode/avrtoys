#ifndef _LED7_H_
#define _LED7_H_

#include "config.h"

extern volatile uint16_t seconds;
extern volatile uint16_t msec;

void led7Init(void);
void led7Set(uint8_t pos, uint8_t code);
void led7Char(uint8_t pos, uint8_t code);
void led7Dot(uint8_t pos, uint8_t state);

#endif /* _LED7_H_ */
