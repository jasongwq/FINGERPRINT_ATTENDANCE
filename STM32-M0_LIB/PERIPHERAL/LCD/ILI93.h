#ifndef __ILI93_H
#define __ILI93_H
#include "sys.h"

//д�Ĵ�����ַ����
//regval:�Ĵ���ֵ
#define LCD_WR_REG(regval) LCD->LCD_REG =regval


//д�Ĵ���
//LCD_Reg:�Ĵ�����ַ
//LCD_RegValue:Ҫд�������
#define LCD_WriteReg LCD_WR_CMD				//д��Ҫд�ļĴ������
#define LCD_WR_CMD(LCD_Reg,LCD_RegValue) LCD->LCD_REG = LCD_Reg;\
LCD->LCD_RAM = LCD_RegValue;//д������

//��ʼдGRAM
#define LCD_WriteRAM_Prepare() LCD->LCD_REG=lcddev.wramcmd

//д16λ�������ݺ���
//LCDдGRAM
//RGB_Code:��ɫֵ
//дLCD����
#define LCD_WR_DATA  LCD_WR_Data
#define LCD_WriteRAM LCD_WR_Data
#define LCD_WR_Data(RGB_Code) LCD->LCD_RAM = RGB_Code

unsigned int LCD_RD_data(void);

#endif
