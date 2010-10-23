#ifndef _TWI_H_
#define _TWI_H_

#include <inttypes.h>

typedef enum {ACK=0, NACK=1} twiACK;

#define TWI_WR 0x00
#define TWI_RD 0x01

void twiInit(void);

void twiStart(void);
void twiStop(void);

twiACK twiSend(uint8_t data);
uint8_t twiReceive(twiACK ack);

#endif /* _TWI_H_ */

