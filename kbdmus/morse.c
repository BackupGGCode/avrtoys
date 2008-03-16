#include "morse.h"

#include "global.h"
#include <avr/pgmspace.h>

#define HEX__(n) 0x##n##LU

#define MZB__(x) ((x&0x00000000000FLU)?1:0) \
                +((x&0x0000000000F0LU)?2:0) \
                +((x&0x000000000F00LU)?4:0) \
                +((x&0x00000000F000LU)?8:0) \
                +((x&0x0000000F0000LU)?16:0) \
                +((x&0x000000F00000LU)?32:0) \
                +((x&0x00000F000000LU)?64:0) \
                +((x&0x0000F0000000LU)?128:0) \
                +((x&0x000F00000000LU)?256:0) \
                +((x&0x00F000000000LU)?512:0) \
                +((x&0x0F0000000000LU)?1024:0) \
                +((x&0xF00000000000LU)?2048:0)

#define MZB(x) MZB__(HEX__(x))

struct morse_s PROGMEM morse_codes[] = {
    {len: 2, code: MZB( 10     ), scode: 0x1c },    //A   
    {len: 4, code: MZB( 0001   ), scode: 0x32 },    //B   
    {len: 4, code: MZB( 0101   ), scode: 0x21 },    //C
    {len: 3, code: MZB( 001    ), scode: 0x23 },    //D   
    {len: 1, code: MZB( 0      ), scode: 0x24 },    //E   
    {len: 4, code: MZB( 0010   ), scode: 0x2B },    //F   
    {len: 3, code: MZB( 011    ), scode: 0x34 },    //G   
    {len: 4, code: MZB( 0000   ), scode: 0x33 },    //H   
    {len: 2, code: MZB( 00     ), scode: 0x43 },    //I   
    {len: 4, code: MZB( 1110   ), scode: 0x3B },    //J   
    {len: 3, code: MZB( 101    ), scode: 0x42 },    //K   
    {len: 4, code: MZB( 0010   ), scode: 0x4B },    //L   
    {len: 2, code: MZB( 11     ), scode: 0x3A },    //M   
    {len: 2, code: MZB( 01     ), scode: 0x31 },    //N   
    {len: 3, code: MZB( 111    ), scode: 0x44 },    //O   
    {len: 4, code: MZB( 0110   ), scode: 0x4D },    //P   
    {len: 4, code: MZB( 1011   ), scode: 0x15 },    //Q   
    {len: 3, code: MZB( 010    ), scode: 0x2d },    //R   
    {len: 3, code: MZB( 000    ), scode: 0x1b },    //S   
    {len: 1, code: MZB( 1      ), scode: 0x2c },    //T   
    {len: 3, code: MZB( 100    ), scode: 0x3c },    //U   
    {len: 4, code: MZB( 0001   ), scode: 0x2a },    //V   
    {len: 3, code: MZB( 110    ), scode: 0x1d },    //W   
    {len: 4, code: MZB( 1001   ), scode: 0x22 },    //X   
    {len: 4, code: MZB( 1101   ), scode: 0x35 },    //Y   
    {len: 4, code: MZB( 0011   ), scode: 0x1a },    //Z   

//    {len: 4, code: MZB( 0111   ), scode: 0x00 },    //ch  
//    {len: 4, code: MZB( 1111   ), scode: 0x00 },    //sch 
//    {len: 5, code: MZB( 00100  ), scode: 0x00 },   //e   
//    {len: 4, code: MZB( 1100   ), scode: 0x00 },    //ju  
//    {len: 4, code: MZB( 1010   ), scode: 0x00 },    //ja  

    {len: 5, code: MZB( 11111  ), scode: 0x45 },   //0   
    {len: 5, code: MZB( 11110  ), scode: 0x16 },   //1   
    {len: 5, code: MZB( 11100  ), scode: 0x1e },   //2   
    {len: 5, code: MZB( 00000  ), scode: 0x26 },   //3   
    {len: 5, code: MZB( 10000  ), scode: 0x25 },   //4   
    {len: 5, code: MZB( 00000  ), scode: 0x2e },   //5   
    {len: 5, code: MZB( 00001 ), scode: 0x36 },   //6   
    {len: 5, code: MZB( 00011  ), scode: 0x3d },   //7   
    {len: 5, code: MZB( 00111  ), scode: 0x3e },   //8   
    {len: 5, code: MZB( 01111  ), scode: 0x46 },   //9   

    {len: 6, code: MZB( 000000 ), scode: 0x49 },  // . dot   
    {len: 6, code: MZB( 101010 ), scode: 0x41 },  // , coma   
    {len: 6, code: MZB( 001100 ), scode: 0x4A },  // ? quest   
//    {len: 5, code: MZB( 00110  ), scode: 0x00 },   //excl   
    {len: 6, code: MZB( 000111 ), scode: 0x4c },  // : colon   
    {len: 6, code: MZB( 010010 ), scode: 0x00 },  //quote   
    {len: 6, code: MZB( 011110 ), scode: 0x52 },  //apo   
// Fail?   {len: 5, code: MZB( 10001  ), scode: 0x55 },   //equ   
    {len: 5, code: MZB( 01001  ), scode: 0x5D },   //slash   
//    {len: 6, code: MZB( 011010 ), scode: 0x00 },  //dog   

    {len: 5, code: MZB( 10001 ), scode: 0x5A },   //divider
    {len: 8, code: MZB( 00000000 ), scode: 0x76 }, //err 

    {len: 0, code: 0, scode: 0} // End of array
};

void get_morse(uint8_t scode, struct morse_s* mc)
{
    uint8_t idx = 0;
    do
    {
        memcpy_P(mc, morse_codes+idx, 3);
        if(mc->scode == scode) break;
        idx++;
    } while(mc->len);
}

