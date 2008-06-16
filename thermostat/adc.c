#include "adc.h"
#include "global.h"

void adcInit(void)
{
    ADMUX =
        (3<<REFS0) | // Int 2.56V
        (1<<ADLAR) |
        (0x0B<<MUX0);

    // MUX = 0x1E // 1.22v
    // MUX = 0x1F // 0v

    ADCSRA =
        (1<<ADEN) | // Enable ADC
        (0<<ADSC) | // Start conversion
        (6<<ADPS0);

}


uint16_t adcRead(void)
{
    ADCSRA _SB(ADSC); // Start conversion

    while(ADCSRA & _B(ADSC)) ; // Wait while conversion in progress

    return ADC;
}

// vim: sw=4:ts=4:si:et
