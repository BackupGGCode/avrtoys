/******************************************

 *      File Name  = display.h
 *
 *      Copyright (c) 2004
 *
 *  @author Ivan A. Alferov
 *  @date Saturday, June 04, 2005

******************************************/

#ifndef __display_H
#define __display_H

#include <string.h>

#define LINE_WIDTH_POW 7
#define LINE_WIDTH (1<<LINE_WIDTH_POW)

#define DISP_COLS 60
#define DISP_LINES 64
#define DISP_ROWS (DISP_LINES/8)

#define DEF_VIDEOMEM_ADDR 0x4000

typedef uint8_t * pVIDEO;

volatile extern pVIDEO disp_frame;
volatile extern pVIDEO disp_print;

extern uint8_t cur_row;
extern uint8_t cur_col;

void clrscr(void);

void putc(char ch);

void setpixel(uint16_t x, uint16_t y);

void wait_frame(void);

void scroll_up(uint8_t lines);
void putcc(uint8_t row, uint8_t col, char ch);

#endif
