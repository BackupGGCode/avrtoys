/******************************************

 *      File Name  = display.c
 *
 *      Copyright (c) 2004
 *
 *  @author Ivan A. Alferov
 *  @date Saturday, June 04, 2005

******************************************/


#include "main.h"
#include "display.h"

#include "8x8.inc"

volatile pVIDEO disp_frame = (pVIDEO)(DEF_VIDEOMEM_ADDR);
volatile pVIDEO disp_print = (pVIDEO)(DEF_VIDEOMEM_ADDR);

volatile uint8_t disp_line asm("asm_line") = 0;
volatile uint8_t disp_char asm("asm_char") = 0;

uint8_t cur_row = 0;
uint8_t cur_col = 0;

void clrscr(void)
{
    cur_row = 0;
    cur_col = 0;
    memset(disp_print, 0x00, LINE_WIDTH*DISP_LINES);
}

void scroll_up(uint8_t lines)
{
    char *target = disp_print;
    char *source = target + LINE_WIDTH*lines;
    memmove(target, source, LINE_WIDTH*(DISP_LINES - lines));
    target += LINE_WIDTH*(DISP_LINES - lines);
    memset(target, 0x00, LINE_WIDTH*lines);
}

void nextline(void)
{
    cur_row++;
    if (cur_row >= DISP_ROWS)
    {
        cur_row--;
//        scroll_up(8);
        for(uint8_t i = 0; i < 8; i++)
        {
            scroll_up(1);
        }
    }
}

void nextch(void)
{
    cur_col++;
    if (cur_col >= DISP_COLS)
    {
        cur_col = 0;
        nextline();
    }
}

void setpixel(uint16_t x, uint16_t y)
{
    *(pVIDEO)(disp_print + (y<<LINE_WIDTH_POW) + (x>>3)) |= (1<<(x & 7));
}

void putcc(uint8_t row, uint8_t col, char ch)
{
    pVIDEO target = disp_print + ((uint16_t)(row)<<(LINE_WIDTH_POW+3)) + col;
    uint16_t src = font8x8 + (ch<<3);
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        *target = pgm_read_byte(src++);
        target += LINE_WIDTH;
    }
}

void putc(char ch)
{
    if (ch == 0x0D)
    {
        cur_col = 0;
    }
    else if (ch == 0x0A)
    {
        nextline();
    }
    else
    {
        putcc(cur_row, cur_col, ch);
        nextch();
    }
}

void wait_frame(void)
{
    while (!disp_line) ;
    while (disp_line) ;
}


/*
    uint8_t port = PORTB;

    disp_char++;
    if (disp_char >= 480/8)
    {
        disp_char = 0;
        PORTB SET_B(LAT);
        PORTB CLR_B(LAT);

        disp_line++;
        if (disp_line >= 64)
        {
            disp_line = 0;
            port SET_B(FLM);
        }
        else
        {
            if (disp_line == 1)
            {
                port INV_B(MCTRL);
                port CLR_B(FLM);
            }
        }
    }
    PORTB = port;

    SPDR = *(uint8_t *)((uint16_t)(disp_char) + ((uint16_t)(disp_line)<<6) + 0x260);
*/


