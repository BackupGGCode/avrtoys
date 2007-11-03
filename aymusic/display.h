/*
 * =====================================================================================
 * 
 *       Filename:  display.h
 * 
 *    Description:  HT1613 display driver
 * 
 *        Version:  1.0
 *        Created:  20.08.2007 03:27:04 MSD
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  Ivan Alfiorov-Romamentiev (http://iar.spb.ru), ivalrom@gmail.com
 *        Company:  Freelancer
 * 
 * =====================================================================================
 */

#ifndef  DISPLAY_FILE_HEADER_INC
#define  DISPLAY_FILE_HEADER_INC

#include <inttypes.h>

void disp_put(uint8_t digit);

void disp_put_str(uint8_t *str);

void disp_digit(uint32_t digit, uint8_t *str);

#endif   /* ----- #ifndef DISPLAY_FILE_HEADER_INC  ----- */

