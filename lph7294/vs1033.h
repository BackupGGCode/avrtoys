#ifndef _VS1033_H_
#define _VS1033_H_

#include "global.h"

// SCI registers
#define SCI_MODE            0x00
#define SCI_STATUS          0x01
#define SCI_BASS            0x02
#define SCI_CLOCKF          0x03
#define SCI_DECODE_TIME     0x04
#define SCI_AUDATA          0x05
#define SCI_WRAM            0x06
#define SCI_WRAMADDR        0x07
#define SCI_HDAT0           0x08
#define SCI_HDAT1           0x09
#define SCI_AIADDR          0x0a
#define SCI_VOL             0x0b
#define SCI_AICTRL0         0x0c
#define SCI_AICTRL1         0x0d
#define SCI_AICTRL2         0x0e
#define SCI_AICTRL3         0x0f

// SCI_MODE bit definitions  (page 36)
#define SM_DIFF          // Differential
#define SM_LAYER12       // Allow MPEG layers I & II
#define SM_RESET         // Soft reset
#define SM_OUTOFWAV      // Jump out of WAV decoding
#define SM_EARSPEAKER_LO // EarSpeaker low setting
#define SM_TESTS         // Allow SDI tests
#define SM_STREAM        // Stream mode
#define SM_EARSPEAKER_HI // EarSpeaker high setting
#define SM_DACT          // DCLK active edge
#define SM_SDIORD        // SDI bit order
#define SM_SDISHARE      // Share SPI chip select
#define SM_SDINEW        // VS1002 native SPI modes
#define SM_ADPCM         // ADPCM recording active
#define SM_ADPCM_HP      // ADPCM high-pass ﬁlter active
#define SM_LINE_IN       // ADPCM recording selector
#define SM_CLK_RANGE     // Input clock range


// SCI_STATUS bit
#define SS_VER           4 // Version
#define SS_APDOWN2       3 // Analog driver powerdown
#define SS_APDOWN1       2 // Analog internal powerdown
#define SS_AVOL          0 // Analog volume control

#define SS_VER_MASK      (7<<SS_VER) // Version
#define SS_APDOWN2_MASK  (1<<SS_APDOWN2) // Analog driver powerdown
#define SS_APDOWN1_MASK  (1<<SS_APDOWN1) // Analog internal powerdown
#define SS_AVOL_MASK     (3<<SS_AVOL) // Analog volume control

// SCI_BASS
#define ST_AMPLITUDE    12 // Treble Control in 1.5 dB steps (-8..7, 0 = off)
#define ST_FREQLIMIT     8 // Lower limit frequency in 1000 Hz steps (1..15)
#define SB_AMPLITUDE     4 // Bass Enhancement in 1 dB steps (0..15, 0 = off)
#define SB_FREQLIMIT     0 // Lower limit frequency in 10 Hz steps (2..15)

#define ST_AMPLITUDE_MASK   (0xF<<ST_AMPLITUDE)
#define ST_FREQLIMIT_MASK   (0xF<<ST_FREQLIMIT)
#define SB_AMPLITUDE_MASK   (0xF<<SB_AMPLITUDE)
#define SB_FREQLIMIT_MASK   (0xF<<SB_FREQLIMIT)

// SCI_CLOCKF
#define SC_MULT    13 // Clock multiplier
#define SC_ADD     11 // Allowed multiplier addition
#define SC_FREQ     0 // Clock frequency

#define SC_MULT_MASK    (0x7<<SC_MULT)
#define SC_ADD_MASK     (0x3<<SC_ADD)
#define SC_FREQ_MASK    (0x7FF<<SC_FREQ)


// Pin definitions
#define VS_xCS PC0
#define VS_xDCS PC1
#define VS_xRESET PC2
#define VS_DREQ PD2

// CS control
#define vsCSon   PORTC _RB(VS_xCS);
#define vsCSoff  PORTC _SB(VS_xCS);
#define vsDCSon  PORTC _RB(VS_xCS);
#define vsDCSoff PORTC _SB(VS_xCS);

void vsInit(void);
uint16_t vsRead(uint8_t addr);
void vsWrite(uint8_t addr, uint16_t val);

// Push data block into VS10xx. Normal size is 32 bytes.
void vsPushData(uint8_t* block, uint8_t size);
void vsPushData_p(uint16_t block, uint8_t size);

#endif

