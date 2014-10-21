#ifndef __OURSTM400240_H_
#define __OURSTM400240_H_

/****************************************/
/*** LCD ***/
#define LCD_CONGIG
//RST
#define RCC_LCD_RST     RCC_APB2Periph_GPIOE
#define LCD_RST_GPIO    GPIOE
#define LCD_RST_Pin     GPIO_Pin_1
//LIGHT
#define RCC_LCD_LIGHT   RCC_APB2Periph_GPIOD
#define LCD_LIGHT_GPIO  GPIOD
#define LCD_LIGHT_Pin   GPIO_Pin_13
#define LCD_LED_1       GPIO_SetBits(  LCD_LIGHT_GPIO,LCD_LIGHT_Pin);//开背光PCout(13) //LCD背光             PD13 
#define LCD_LED_0       GPIO_ResetBits(LCD_LIGHT_GPIO,LCD_LIGHT_Pin);//

//RD
#define RCC_LCD_RD  RCC_APB2Periph_GPIOD
#define LCD_RD_GPIO GPIOD
#define LCD_RD_Pin  GPIO_Pin_4
//WR
#define RCC_LCD_WR  RCC_APB2Periph_GPIOD
#define LCD_WR_GPIO GPIOD
#define LCD_WR_Pin  GPIO_Pin_5
//CS
#define RCC_LCD_CS  RCC_APB2Periph_GPIOD
#define LCD_CS_GPIO GPIOD
#define LCD_CS_Pin  GPIO_Pin_7
//RS
#define RCC_LCD_RS  RCC_APB2Periph_GPIOD
#define LCD_RS_GPIO GPIOD
#define LCD_RS_Pin  GPIO_Pin_11
/*** LCD ***/
/****************************************/
/***TOUCH***/
#define LCD_TOUCH_SPI_SOFT 0
#define LCD_TOUCH_SPI_1    1
#define LCD_TOUCH_SPI_2    0

#define RCC_LCD_TOUCH_PEN   RCC_APB2Periph_GPIOB
#define LCD_TOUCH_PEN_GPIO GPIOB
#define LCD_TOUCH_PEN_Pin  GPIO_Pin_14

#define RCC_LCD_TOUCH_TCS   RCC_APB2Periph_GPIOB
#define LCD_TOUCH_TCS_GPIO GPIOB
#define LCD_TOUCH_TCS_Pin   GPIO_Pin_10
//与触摸屏芯片连接引脚
#define PEN     PBin(6)      //PF10 INT
#define TCS     PBout(7)     //PB2  CS

#if LCD_TOUCH_SPI_SOFT
#define RCC_LCD_TOUCH_MISO RCC_APB2Periph_GPIOB
#define LCD_TOUCH_MISO_GPIO GPIOB
#define LCD_TOUCH_MISO_Pin  GPIO_Pin_12

#define RCC_LCD_TOUCH_MOSI RCC_APB2Periph_GPIOB
#define LCD_TOUCH_MOSI_GPIO GPIOB
#define LCD_TOUCH_MOSI_Pin  GPIO_Pin_13

#define RCC_LCD_TOUCH_SCK RCC_APB2Periph_GPIOB
#define LCD_TOUCH_SCK_GPIO GPIOB
#define LCD_TOUCH_SCK_Pin  GPIO_Pin_11

//模拟SPI引脚
#define DOUT    PBin (12)//PAin(6)     //PF8  MISO
#define TDIN    PBout(13)//PAout(7)    //PF9  MOSI
#define TCLK    PBout(11)//PAout(5)    //PB1  SCLK

#endif
#define LCD_TP_CONGIG
#define LCD_TP_xfac 0.066844
#define LCD_TP_yfac 0.109823
#define LCD_TP_xoff (-18)
#define LCD_TP_yoff (-22)
//默认为touchtype=0的数据.
#define LCD_TP_TOUCHTYPE 1
//新增的参数,当触摸屏的左右上下完全颠倒时需要用到.
//touchtype=0的时候,适合左右为X坐标,上下为Y坐标的TP.
//touchtype=1的时候,适合左右为Y坐标,上下为X坐标的TP.
#if LCD_TP_TOUCHTYPE
#define LCD_TP_DEFCMD_RDX 0X90;
#define LCD_TP_DEFCMD_RDY 0XD0;
#else
#define LCD_TP_DEFCMD_RDX 0XD0;
#define LCD_TP_DEFCMD_RDY 0X90;
#endif

/***TOUCH***/
/****************************************/

#endif
