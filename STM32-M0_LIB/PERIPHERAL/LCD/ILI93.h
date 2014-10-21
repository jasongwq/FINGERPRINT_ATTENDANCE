#ifndef __ILI93_H
#define __ILI93_H
#include "sys.h"

//写寄存器地址函数
//regval:寄存器值
#define LCD_WR_REG(regval) LCD->LCD_REG =regval


//写寄存器
//LCD_Reg:寄存器地址
//LCD_RegValue:要写入的数据
#define LCD_WriteReg LCD_WR_CMD				//写入要写的寄存器序号
#define LCD_WR_CMD(LCD_Reg,LCD_RegValue) LCD->LCD_REG = LCD_Reg;\
LCD->LCD_RAM = LCD_RegValue;//写入数据

//开始写GRAM
#define LCD_WriteRAM_Prepare() LCD->LCD_REG=lcddev.wramcmd

//写16位像素数据函数
//LCD写GRAM
//RGB_Code:颜色值
//写LCD数据
#define LCD_WR_DATA  LCD_WR_Data
#define LCD_WriteRAM LCD_WR_Data
#define LCD_WR_Data(RGB_Code) LCD->LCD_RAM = RGB_Code

unsigned int LCD_RD_data(void);

#endif
