/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  Keyboard music
 *
 *        Version:  1.0
 *        Created:  Fri Mar 14 17:43:49 MSK 2008
 *       Revision:  $Id$
 *       Compiler:  gcc
 *
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 *
 * =====================================================================================
 */

#include "global.h"

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/delay_basic.h>
#include <string.h>

#ifdef ENUART
#include "uart.h"
#endif

#include "helpers.h"
#include "morse.h"

static volatile uint8_t fifo_buf[16];
static volatile uint8_t fifo_top = 0;
static volatile uint8_t fifo_bottom = 0;

uint8_t fifo_push(uint8_t val)
{
    if(((fifo_bottom - fifo_top) & 15) != 1)
    {
        fifo_buf[fifo_top++] = val;
        fifo_top &= 15;
        return 0;
    }
    else
        return 1;
}

uint8_t fifo_pop(void)
{
    if(fifo_bottom != fifo_top)
    {
        uint8_t val = fifo_buf[fifo_bottom++];
        fifo_bottom &= 15;
        return val;
    }
    else
        return 0;
}

#ifdef TN13
#   define KPORT    PORTB
#   define KDDR     DDRB
#   define KPIN     PINB
#   define KDATA    PB3
#   define KCLK     PB4
#else
#   define KPORT    PORTB
#   define KDDR     DDRB
#   define KPIN     PINB
#   define KDATA    PB3
#   define KCLK     PB4
#endif
#define KMASK   _B(KDATA) | _B(KCLK)

#define KDATMARK KDDR _SB(KDATA)
#define KDATREL  KDDR _RB(KDATA)
#define KCLKMARK KDDR _SB(KCLK)
#define KCLKREL  KDDR _RB(KCLK)

static uint8_t kbd_recv_buf;
static uint8_t kbd_recv_cnt = 0;

struct kbd_flag_s
{
    unsigned rel:   1; // If key released (0xF0)
    unsigned ext:   1; // If extend key prefix (0xE0)
    unsigned err:   1;  // If kbd error (0xFE)
};
volatile struct kbd_flag_s kbd_flag = {
    rel: 0,
    ext: 0,
    err: 0
};

// Look kbd protocol description on
// http://www.beyondlogic.org/keyboard/keybrd.htm
//

// Receive procedure
ISR(PCINT_vect)
{
    if(!(KPIN & _B(KCLK)))
    {
        if(kbd_recv_cnt) // Read bits in progress
        {
            if(kbd_recv_cnt > 1)
            {
                kbd_recv_buf >>= 1;
                if(KPIN & _B(KDATA)) kbd_recv_buf |= 0x80;
            }
            kbd_recv_cnt--;
            if(kbd_recv_cnt == 0)
            {
                switch(kbd_recv_buf)
                {
                    case 0xE0:
                        kbd_flag.ext = 1;
                        break;
                    case 0xF0:
                        kbd_flag.rel = 1;
                        break;
                    case 0xFE:
                        kbd_flag.err = 1;
                        break;
                    case 0xFA: // Ack code
                    case 0xAA: // BAT (Basic Assurance Test) successful
                    case 0xFC: // Error after reset
                        break;
                    default:
                        if(kbd_flag.ext || kbd_flag.rel)
                        {
                            kbd_flag.ext = 0;
                            kbd_flag.rel = 0;
                        }
                        else
                            fifo_push(kbd_recv_buf);
                }
            }
        }
        else
        {
            if(!(KPIN & _B(KDATA))) // Wait for start condition
                kbd_recv_cnt = 9;
        }
    }
}

/*****************************************
 *
Below are all the commands the host may send to the keyboard:
  - 0xFF (Reset) - Keyboard responds with "ack" (0xFA), then enters "Reset" mode.  (See "Reset" section.)
  - 0xFE (Resend) - Keyboard responds by resending the last-sent byte.  The
      exception to this is if the last-sent byte was "resend" (0xFE).  If this is
      the case, the keyboard resends the last non-0xFE byte.  This command is used
      by the host to indicate an error in reception.
        The next six commands can be issued when the keyboard is in any mode, but it
        only effects the behavior of the keyboard when in "mode 3" (ie, set to scan
        code set 3.) 
  - *0xFD (Set Key Type Make) - Disable break codes and typematic repeat for
      specified keys.  Keyboard responds with "ack" (0xFA), then disables scanning
      (if enabled) and reads a list of keys from the host.  These keys are
      specified by their set 3 make codes.  Keyboard responds to each make code
      with "ack".  Host terminates this list by sending an invalid set 3 make code
      (eg, a valid command.)  The keyboard then re-enables scanning (if previously
      disabled).
  - *0xFC (Set Key Type Make/Break) - Similar to previous command, except this
      one only disables typematic repeat.
  - *0xFB (Set Key Type Typematic) - Similar to previous two, except this one
      only disables break codes.
  - *0xFA (Set All Keys Typematic/Make/Break) - Keyboard responds with "ack"
      (0xFA).  Sets all keys to their normal setting (generate scan codes on make,
      break, and typematic repeat)
  - *0xF9 (Set All Keys Make) - Keyboard responds with "ack" (0xFA).  Similar
      to 0xFD, except applies to all keys.
  - *0xF8 (Set All Keys Make/Break) - Keyboard responds with "ack" (0xFA).
      Similar to 0xFC, except applies to all keys.
  - *0xF7 (Set All Keys Typematic) - Keyboard responds with "ack" (0xFA).
      Similar to 0xFB, except applies to all keys.
  - 0xF6 (Set Default) - Load default typematic rate/delay (10.9cps / 500ms),
      key types (all keys typematic/make/break), and scan code set (2).
  - 0xF5 (Disable) - Keyboard stops scanning, loads default values (see "Set
      Default" command), and waits further instructions.
  - 0xF4 (Enable) - Re-enables keyboard after disabled using previous command.
  - 0xF3 (Set Typematic Rate/Delay) - Host follows this command with one
      argument byte that defines the typematic rate and delay as follows:
         Read detail on http://www.computer-engineering.org/ps2keyboard/
  - *0xF2 (Read ID) - The keyboard responds by sending a two-byte device ID of
      0xAB, 0x83. (0xAB is sent first, followed by 0x83.)
  - *0xF0 (Set Scan Code Set) -  Keyboard responds with "ack", then reads
      argument byte from the host.  This argument byte may be 0x01, 0x02, or 0x03
      to select scan code set 1, 2, or 3, respectively.  The keyboard responds to
      this argument byte with "ack".  If the argument byte is 0x00, the keyboard
      responds with "ack" followed by the current scan code set.
  - 0xEE (Echo) - The keyboard responds with "Echo" (0xEE).
  - 0xED (Set/Reset LEDs) - The host follows this command with one argument
      byte, that specifies the state of the keyboard's Num Lock, Caps Lock, and
      Scroll Lock LEDs.  This argument byte is defined as follows:
           Bit      Description
           7-3      Always 0
            2       Caps Lock
            1       Num Lock 
            0       Scroll Lock

*Originally available in PS/2 keyboards only.

 *
 *****************************************/

#define KBDCMD_LED    0xED

#define KBDLED_SCROLL 0x01
#define KBDLED_NUM    0x02
#define KBDLED_CAPS   0x04

// Send byte to kbd
void send_byte(uint8_t value)
{
    cli();

    KCLKMARK;
    _delay_us(70);
    KDATMARK;
    _delay_us(20);
    KCLKREL;
    _delay_us(10);

    // Normal send operation very short time and no need watchdog timer reset
    // If keyboard not responding we need to reset
    while(KPIN & _B(KCLK)); // wdr();

    uint8_t par = 0;
    for(uint8_t i=0; i<8; ++i)
    {
        if(value & 1)
        {
            KDATREL; // send 1
            par ^= 1;
        }
        else
            KDATMARK; // send 0
        value >>= 1;
        while(!(KPIN & _B(KCLK)));
        while(KPIN & _B(KCLK));
    }

    if(!par) KDATREL; else KDATMARK;

    while(!(KPIN & _B(KCLK)));
    while(KPIN & _B(KCLK));
    KDATREL; // Stopbit

    // wait for kbd ask
    do
    {
        while(!(KPIN & _B(KCLK))) wdr();
        while(KPIN & _B(KCLK)) wdr();
    }
    while(KPIN & _B(KDATA));


    EIFR = 0xFF;
    sei();
}

/* Tones */
#define T0PRESC 64
#define F_DIV(x) ((F_CPU/(2*T0PRESC*(x)))-1)

static prog_uint8_t tones[] = {
// Octave 1. Base tone 440
    F_DIV(2*130.813), //    0: C (130.813 Hz)
    F_DIV(2*138.591), //    1: C# (138.591 Hz)
    F_DIV(2*146.832), //    2: D (146.832 Hz)
    F_DIV(2*155.563), //    3: D# (155.563 Hz)
    F_DIV(2*164.814), //    4: E (164.814 Hz)
    F_DIV(2*174.614), //    5: F (174.614 Hz)
    F_DIV(2*184.997), //    6: F# (184.997 Hz)
    F_DIV(2*195.998), //    7: G (195.998 Hz)
    F_DIV(2*207.652), //    8: G# (207.652 Hz)
    F_DIV(2*220.000), //    9: A (220.000 Hz)
    F_DIV(2*233.082), //   10: A# (233.082 Hz)
    F_DIV(2*246.942), //   11: B (246.942 Hz)
};

#define MORSE_DELAY 100
#define MORSE_TONE  16

void send_morse(uint8_t scancode)
{
    volatile struct morse_s mc;
    get_morse(scancode, &mc);

    uint16_t d = mc.code;
    for(uint8_t i = 0; i < mc.len; ++i)
    {
        OCR0A = MORSE_TONE;
        TCCR0A _SB(COM0A0); // Tone on
        send_byte(KBDCMD_LED); // Set leds
        if(d&1)
        { // Tire
            send_byte(KBDLED_NUM);
            delay_ms(3*MORSE_DELAY);
        }
        else
        { // Dot
            send_byte(KBDLED_CAPS);
            delay_ms(MORSE_DELAY);
        }
        TCCR0A _RB(COM0A0); // Mute
        send_byte(KBDCMD_LED); // Set leds
        send_byte(0);
        delay_ms(MORSE_DELAY);
        d >>= 1;
    }
}

uint8_t allow_power_down = 0;
int main(void)
{
    // OSCCAL = 0x6D;

    /* Init section */
    {
#ifdef WDTON
        wdt_enable(WDTO_1S);
#else
        wdt_disable();
#endif

    // Init pins
#ifdef TN2313
    PORTD = 0x00 | _B(PD0) | _B(PD1) | _B(PD5) | _B(PD4);
    DDRD  = 0x00 | _B(PD1)           | _B(PD5) | _B(PD4);
#endif

    PORTB = 0x00;
    DDRB  = 0x00 | _B(PB2); // All inputs

    _SET(KPORT, 0, KMASK);
    _SET(KDDR, 0, KMASK);

    // Init PC interrupts
    PCMSK = _B(KCLK); // all pins enabled
    MCUCR = 0; // (0x02<<ISC10); // On INT1 falling edge
    GIMSK = _B(PCIE);

    // Timer0 init
#ifdef TN13
#   error TODO tn13!
#else
    OCR0A = 0x00;
    OCR0B = 0x00;
    TCNT0 = 0x00;
    TCCR0A = (0x00<<COM0A0) | (0x02<<WGM00); // Toggle on CTC
    TCCR0B = _B(FOC0A) | (0x03<<CS00); // 64 prescaler
#endif

#ifdef TN2313
    uartInit(UART115200);
#endif

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    }

    sei();

    /*
     *uint8_t itone = 0;
     *while(1) {
     *   wdr();
     *   OCR0A = pgm_read_byte(tones + itone);
     *   if(++itone > 11) itone = 0;
     *   TCCR0A _SB(COM0A0);
     *   delay_ms(250);
     *}
     */

    delay_ms(500);
    send_byte(0xFF); // Reset
    delay_ms(500); // Wait kbd reset

    send_byte(KBDCMD_LED); // Set leds
    send_byte(KBDLED_SCROLL);
    
    while(1)
    {
        wdr();

        // Main loop
        if(fifo_top != fifo_bottom)
        {
            uint8_t sc = fifo_pop();
            uartPut(sc);

            send_morse(sc);
            delay_ms(3*MORSE_DELAY);

        }

        // Power down
        if(allow_power_down)
        {
#ifdef WDTON
            wdt_disable();
#endif
            cli();
            sleep_enable();
            sei();
            sleep_cpu();
            sleep_disable();
#ifdef WDTON
            wdt_enable(WDTO_1S);
#endif
        }
    }

}


