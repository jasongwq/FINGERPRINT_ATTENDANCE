#ifndef __NOKIA_5110_H
#define __NOKIA_5110_H	 
#include "sys.h"
/*

*/

// pin define for n5110lcd_8key board
// change this if your hardware changed!

//sbit SCLK = P2 ^ 5;     // pin 2     header 5
//sbit SDIN = P2 ^ 4;     // pin 3     header 4
//sbit LCD_DC = P2 ^ 3;   // pin 4     header 3
//sbit LCD_CE = P2 ^ 2;   // pin 5     header 2
//sbit LCD_RST = P2 ^ 1;  // pin 9     header 1

#define LCD_RST PEout(7)
#define LCD_CE  PEout(8)
#define LCD_DC  PEout(9)
#define SDIN  	PEout(10)
#define SCLK  	PEout(11)





void Set_Nokia_Contrast(u8 Contrast);//设置对比度;

void NOKIA_5110_LCD_Init(void);
void NOKIA_5110_LCD_Clear(void);
void LCD_move_chinese_string(unsigned char X, unsigned char Y, unsigned char T);
void LCD_write_english_string(unsigned char X, unsigned char Y, char *s);
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                              unsigned char ch_with, unsigned char num,
                              unsigned char line, unsigned char row);
void chinese_string(unsigned char X, unsigned char Y, unsigned char T);
void LCD_write_char(unsigned char c);
void LCD_draw_bmp_pixel(unsigned char X, unsigned char Y, unsigned char *map,
                        unsigned char Pix_x, unsigned char Pix_y);
//void LCD_write_byte(unsigned char dat, unsigned char dc);
//void LCD_set_XY(unsigned char X, unsigned char Y);
#endif

