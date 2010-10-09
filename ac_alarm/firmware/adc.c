#include "adc.h"

#define S_ADMUX0 ((1<<REFS1) | 0)
#define S_ADMUX1 ((1<<REFS1) | 1)
#define S_ADMUX2 ((1<<REFS1) | 2)
#define S_ADMUXT ((1<<REFS1) | 0x22)

#define S_ADCSRA ((1<<ADEN) | 6)

void adcInit(void)
{
    ADMUX = S_ADMUXT;
    ADCSRB = 0;
    DIDR0 = 0x07;
    ADCSRA = S_ADCSRA;
    ADCSRA |= (1<<ADSC); // Start conversion
    while(!(ADCSRA & (1<<ADIF))) // Wait conversion
        ;
    ADCSRA |= (1<<ADIF); // Clear ready flag
}

uint16_t adcRead(void)
{
    while(!(ADCSRA & (1<<ADIF))) // Wait conversion
        ;
    ADCSRA |= (1<<ADIF); // Clear ready flag
    uint8_t l = ADCL;
    return (ADCH<<8) | l;
}

