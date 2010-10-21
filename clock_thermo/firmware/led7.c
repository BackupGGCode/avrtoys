#include "led7.h"

#define SEG_A (1U<<0)
#define SEG_B (1U<<2)
#define SEG_C (1U<<4)
#define SEG_D (1U<<6)
#define SEG_E (1U<<7)
#define SEG_F (1U<<1)
#define SEG_G (1U<<3)
#define SEG_H (1U<<5)

const uint8_t PROGMEM chartab[] = {
    ~(SEG_H | SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F), // 0
    ~(SEG_H | SEG_B | SEG_C), // 1
    ~(SEG_H | SEG_A | SEG_B | SEG_D | SEG_E | SEG_G), // 2
    ~(SEG_H | SEG_A | SEG_B | SEG_C | SEG_D | SEG_G), // 3
    ~(SEG_H | SEG_B | SEG_C | SEG_F | SEG_G), // 4
    ~(SEG_H | SEG_A | SEG_C | SEG_D | SEG_F | SEG_G), // 5
    ~(SEG_H | SEG_A | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), // 6
    ~(SEG_H | SEG_A | SEG_B | SEG_C), // 7
    ~(SEG_H | SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), // 8
    ~(SEG_H | SEG_A | SEG_B | SEG_C | SEG_D | SEG_F | SEG_G), // 9
    ~(SEG_H | SEG_A | SEG_B | SEG_D | SEG_E | SEG_F | SEG_G), // A
    ~(SEG_H | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), // B
    ~(SEG_H | SEG_A | SEG_D | SEG_E | SEG_F), // C
    ~(SEG_H | SEG_C | SEG_D | SEG_E | SEG_F | SEG_G), // D
    ~(SEG_H | SEG_A | SEG_D | SEG_E | SEG_F | SEG_G), // E
    ~(SEG_H | SEG_A | SEG_E | SEG_F | SEG_G), // F
    ~(SEG_H | 0), //
    ~(SEG_H | SEG_G), // -
};

volatile uint16_t seconds = 0;
volatile uint16_t msec = 0;

uint8_t disp[4];
uint8_t disp_pos = 0;

void led7Init(void)
{
    uint8_t i;
    for(i=0; i<sizeof(disp); i++)
        disp[i] = 0xFF;

    OCR0A = F_CPU/1000/64; //125;
    TCCR0A = (2<<WGM00);
    TCCR0B = (0<<WGM02) | (3<<CS00);
    TIMSK |= (1<<OCIE0A);
}

void led7Set(uint8_t pos, uint8_t code)
{
    disp[pos] = ~code;
}

void led7Char(uint8_t pos, uint8_t code)
{
    disp[pos] = (disp[pos] & SEG_H) | pgm_read_byte(chartab+code);
}

void led7Dot(uint8_t pos, uint8_t state)
{
    if(state)
        disp[pos] &= ~SEG_H;
    else
        disp[pos] |= SEG_H;
}

static void _push_spi(uint8_t data)
{
    USIDR = data;
    register uint8_t a = (1<<USIWM0)|(0<<USICS0)|(1<<USITC);
    register uint8_t b = (1<<USIWM0)|(0<<USICS0)|(1<<USITC)|(1<<USICLK);
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    USICR = a; USICR = b;
    PORTB |= (1<<PB2);
    PORTB &= ~(1<<PB2);
}

ISR(TIMER0_COMPA_vect)
{
    PORTB |= (1<<L_A1);
    PORTB |= (1<<L_A2);
    PORTD |= (1<<L_A4);
    PORTD |= (1<<L_A3);

    _push_spi(disp[disp_pos]);

    switch(disp_pos)
    {
        case 0: PORTB &= ~(1<<L_A1); break;
        case 1: PORTB &= ~(1<<L_A2); break;
        case 2: PORTD &= ~(1<<L_A3); break;
        default: PORTD &= ~(1<<L_A4); break;
    }

    if(disp_pos >= sizeof(disp)-1)
        disp_pos = 0;
    else
        disp_pos++;

    if(msec>=1000)
    {
        msec = 0;
        seconds++;
    }
    msec++;
}

