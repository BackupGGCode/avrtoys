#ifndef _DS1621_H_
#define _DS1621_H_

#include "global.h"

void ds16Init(void);

void ds16Start(void);
int16_t ds16Read(void);

#endif

