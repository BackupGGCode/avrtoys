#ifndef _MORSE_H_
#define _MORSE_H_

#include "global.h"

struct morse_s {
    unsigned code:  12;
    unsigned len:   4;
    unsigned scode: 8;
};

union morse_u {
    struct morse_s m;
    uint16_t u16;
};

void get_morse(uint8_t scode, struct morse_s* mc);

#endif
