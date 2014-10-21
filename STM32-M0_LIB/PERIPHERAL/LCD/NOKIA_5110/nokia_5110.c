#include "nokia_5110.h"
#include "english_6x8_pixel.h"//ascll
#include "write_chinese_string_pixel.h"//中文
#include "delay.h"//中文


static void _GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //使能Px端口时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;   //PC.7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

}

/*-----------------------------------------------------------------------
LCD_write_byte    : 使用SPI接口写数据到LCD
输入参数：data    ：写入的数据；
          command ：写数据/命令选择；
-----------------------------------------------------------------------*/
static void LCD_write_byte(unsigned char dat, unsigned char command)
{
    unsigned char i;
    LCD_CE = 0;

    if (command == 0)
        LCD_DC = 0;
    else
        LCD_DC = 1;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
            SDIN = 1;
        else
            SDIN = 0;
        SCLK = 0;
        dat = dat << 1;
        SCLK = 1;
    }
    LCD_CE = 1;
}
void Set_Nokia_Contrast(u8 Contrast)
{
	  if(Contrast>5)Contrast=5;	
    LCD_write_byte(0x21, 0);    // 使用扩展命令设置LCD模式
	  LCD_write_byte(0xc0 | (1 << Contrast), 0); // 设置偏置电压
	  LCD_write_byte(0x20, 0);    // 使用基本命令
}

/*-----------------------------------------------------------------------
LCD_init          : 3310LCD初始化
编写日期          ：2004-8-10
最后修改日期      ：2004-8-10
-----------------------------------------------------------------------*/
void NOKIA_5110_LCD_Init(void)
{
    _GPIO_Init();

    // 产生一个让LCD复位的低电平脉冲
    LCD_RST = 0;
    delay_us(1);
    LCD_RST = 1;
    // 关闭LCD
    LCD_CE = 0;
    delay_us(1);
    // 使能LCD
    LCD_CE = 1;
    delay_us(1);

    /*功能设置
    0   水平 基本指令集
    1        拓展...
    2 垂直 基本指令集
    3      拓展...
    4 省电模式
    5
    6
    7
    */
    //    LCD_write_byte(0x21, 0);    // 使用省电模式
    LCD_write_byte(0x21, 0);    // 使用扩展命令设置LCD模式
    //LCD_write_byte(0x20, 0);    // 使用基本命令设置LCD模式
    //0~5 5对比度最高 0对比度最低
    LCD_write_byte(0xc0 | (1 << 4), 0); // 设置偏置电压
    /*
    +0x00~0x03
    0
    1
    9
    17
    24
    */
    LCD_write_byte((0x04 + 0x00), 0);  // 温度校正
    /*
    0X10~0X17
    BS2 BS1 BS0 n 推荐混合率
    0    0   0  7   1:100
    0    0   1  6   1:80
    0    1   0  5   1:65 / 1:65
    0    1   1  4   1:48
    1    0   0  3   1:40 / 1:34
    1    0   1  2   1:24
    1    1   0  1   1:18 / 1:16
    1    1   1  0   1:10 / 1:9 / 1:8
    */
    LCD_write_byte(0x10 + 0x00, 0);  // 1:48 设置偏置系统
    LCD_write_byte(0x20, 0);    // 使用基本命令
    NOKIA_5110_LCD_Clear();     // 清屏
    LCD_write_byte(0x0c, 0);    // 设定显示模式，正常显示
    NOKIA_5110_LCD_Clear();
    // 关闭LCD
    LCD_CE = 0;
}

/*-----------------------------------------------------------------------
LCD_clear         : LCD清屏函数
-----------------------------------------------------------------------*/
void NOKIA_5110_LCD_Clear(void)
{
    unsigned int i;
    LCD_write_byte(0x0c, 0);
    LCD_write_byte(0x80, 0);

    for (i = 0; i < 504; i++)
        LCD_write_byte(0, 1);
}

/*-----------------------------------------------------------------------
LCD_set_XY        : 设置LCD坐标函数
输入参数：X       ：0－83
          Y       ：0－5
-----------------------------------------------------------------------*/
static void LCD_set_XY(unsigned char X, unsigned char Y)
{
    LCD_write_byte(0x40 | Y, 0);        // column
    LCD_write_byte(0x80 | X, 0);            // row
}

/*-----------------------------------------------------------------------
LCD_write_char    : 显示英文字符
输入参数：c       ：显示的字符；
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
{
    unsigned char line;
    c -= 32;
    for (line = 0; line < 6; line++)
        LCD_write_byte(font6x8[c][line], 1);
}

/*-----------------------------------------------------------------------
LCD_write_english_String  : 英文字符串显示函数
输入参数：*s      ：英文字符串指针;
          X、Y    : 显示字符串的位置,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X, unsigned char Y, char* s)
{
    LCD_set_XY(X, Y);
    while (*s)
    {
        LCD_write_char(*s);
        s++;
    }
}
/*-----------------------------------------------------------------------
LCD_write_chinese_string: 在LCD上显示汉字
输入参数：X、Y    ：显示汉字的起始X、Y坐标；
          ch_with ：汉字点阵的宽度
          num     ：显示汉字的个数；
          line    ：汉字点阵数组中的起始行数
          row     ：汉字显示的行间距
测试：
    LCD_write_chi(0,0,12,7,0,0);
    LCD_write_chi(0,2,12,7,0,0);
    LCD_write_chi(0,4,12,7,0,0);
-----------------------------------------------------------------------*/
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                              unsigned char ch_with, unsigned char num,
                              unsigned char line, unsigned char row)
{
    unsigned char i, n;

    LCD_set_XY(X, Y);                            //设置初始位置

    for (i = 0; i < num;)
    {
        for (n = 0; n < ch_with * 2; n++)        //写一个汉字
        {
            if (n == ch_with)                    //写汉字的下半部分
            {
                if (i == 0) LCD_set_XY(X, Y + 1);
                else
                    LCD_set_XY((X + (ch_with + row)*i), Y + 1);
            }
            LCD_write_byte(write_chinese[line + i][n], 1);
        }
        i++;
        LCD_set_XY((X + (ch_with + row)*i), Y);
    }
}



/*-----------------------------------------------------------------------
LCD_draw_map      : 位图绘制函数
输入参数：X、Y    ：位图绘制的起始X、Y坐标；
          *map    ：位图点阵数据；
          Pix_x   ：位图像素（长）
          Pix_y   ：位图像素（宽）
-----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X, unsigned char Y, unsigned char* map,
                        unsigned char Pix_x, unsigned char Pix_y)
{
    unsigned int i, n;
    unsigned char row;

    if (Pix_y % 8 == 0) row = Pix_y / 8; //计算位图所占行数
    else row = Pix_y / 8 + 1;

    for (n = 0; n < row; n++)
    {
        LCD_set_XY(X, Y);
        for (i = 0; i < Pix_x; i++)
        {
            LCD_write_byte(map[i + n * Pix_x], 1);
        }
        Y++;                         //换行
    }
}




