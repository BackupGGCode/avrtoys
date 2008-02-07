# Name: Makefile
# Project: 
# Author: Ivan "plumbum" A-R <ivalrom@gmail.com>
# Creation Date: Чтв Фев  7 13:28:48 MSK 2008
# Tabsize: 4
# Copyright: GPL v2
# This Revision: $Id$

# vim: sw=8:ts=8:si:noexpandtab

MCU=attiny13

MAIN=main

PROG=avrdude -P /dev/ttyUSB0 -b 115200 -c stk500v2 -B 2
PROG200=avrdude -P /dev/parport0 -c stk200

COMPILE=avra -fI

LFUSE=0x6A
HFUSE=0xFD

# symbolic targets:
all:    $(MAIN).asm
	$(COMPILE) $(MAIN).asm -l $(MAIN).lst -m $(MAIN).map -o $(MAIN).hex

clean:
	rm -f *.hex *.lst *.obj *.cof *.map *.eep.hex

program:	all
	$(PROG) -p $(MCU) -U flash:w:$(MAIN).hex

prg200:	all
	$(PROG200) -p $(MCU) -U flash:w:$(MAIN).hex

fuse:
	$(PROG) -p $(MCU) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

fuse200:
	$(PROG200) -p $(MCU) -U lfuse:w:$(LFUSE):m -U hfuse:w:$(HFUSE):m

stk500:
	$(PROG) -p $(MCU) -t

