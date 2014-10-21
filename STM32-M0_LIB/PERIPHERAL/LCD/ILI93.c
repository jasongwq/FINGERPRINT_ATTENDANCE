#include "delay.h"
#include "gpio.h"
#include "lcd.h"
#include "USART.h"
#include "stdio.h"
//////////////////////////////////////////////////////////////////////////////////   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//2.4/2.8寸/3.5寸 TFT液晶驱动   
//支持驱动IC型号包括:ILI9341/ILI9325/RM68042/RM68021/ILI9320/ILI9328/LGDP4531/LGDP4535/SPFD5408/SSD1289/1505/B505/C505等     
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/10/7
//版本：V2.2
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved   
//********************************************************************************
//V1.2修改说明
//支持了SPFD5408的驱动,另外把液晶ID直接打印成HEX格式.方便查看LCD驱动IC.
//V1.3
//加入了快速IO的支持
//修改了背光控制的极性（适用于V1.8及以后的开发板版本）
//对于1.8版本之前(不包括1.8)的液晶模块,请修改LCD_Init函数的LCD_LED=1;为LCD_LED=1;
//V1.4
//修改了LCD_ShowChar函数，使用画点功能画字符。
//加入了横竖屏显示的支持
//V1.5 20110730
//1,修改了B505液晶读颜色有误的bug.
//2,修改了快速IO及横竖屏的设置方式.
//V1.6 20111116
//1,加入对LGDP4535液晶的驱动支持
//V1.7 20120713
//1,增加LCD_RD_DATA函数
//2,增加对ILI9341的支持
//3,增加ILI9325的独立驱动代码
//4,增加LCD_Scan_Dir函数(慎重使用)    
//6,另外修改了部分原来的函数,以适应9341的操作
//V1.8 20120905
//1,加入LCD重要参数设置结构体lcddev
//2,加入LCD_Display_Dir函数,支持在线横竖屏切换
//V1.9 20120911
//1,新增RM68042驱动（ID:6804），但是6804不支持横屏显示！！原因：改变扫描方式，
//导致6804坐标设置失效，试过很多方法都不行，暂时无解。
//V2.0 20120924
//在不硬件复位的情况下,ILI9341的ID读取会被误读成9300,修改LCD_Init,将无法识别
//的情况（读到ID为9300/非法ID）,强制指定驱动IC为ILI9341，执行9341的初始化。
//V2.1 20120930
//修正ILI9325读颜色的bug。
//V2.2 20121007
//修正LCD_Scan_Dir的bug。
//////////////////////////////////////////////////////////////////////////////////   
                 

//读LCD数据
//返回值:读到的值
u16 LCD_RD_DATA(void)
{                                                  
    return LCD->LCD_RAM;         
}                      
//读寄存器
//LCD_Reg:寄存器地址
//返回值:读到的数据
u16 LCD_ReadReg(u8 LCD_Reg)
{                                          
    LCD_WR_REG(LCD_Reg);        //写入要读的寄存器序号
    delay_us(5);          
    return LCD_RD_DATA();       //返回读到的值
}   
//读显示地址数据
unsigned int LCD_RD_data(void)
{
    unsigned int a = 0;
    a = LCD->LCD_RAM; //空操作
    a = LCD->LCD_RAM; //读出的实际16位像素数据
    return (a);
}
//设置LCD的自动扫描方向
//注意:其他函数可能会受到此函数设置的影响(尤其是9341/6804这两个奇葩),
//所以,一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.
//dir:0~7,代表8个方向(具体定义见lcd.h)
//9320/9325/9328/4531/4535/1505/b505/8989/5408/9341等IC已经实际测试
void LCD_Scan_Dir(u8 dir)
{
    u16 regval = 0;
    u8 dirreg = 0;
    u16 temp;
    if (lcddev.dir == 1 && lcddev.id != 0X6804) //横屏时，对6804不改变扫描方向！
    {
        switch (dir) //方向转换
        {
        case 0: dir = 6; break;
        case 1: dir = 7; break;
        case 2: dir = 4; break;
        case 3: dir = 5; break;
        case 4: dir = 1; break;
        case 5: dir = 0; break;
        case 6: dir = 3; break;
        case 7: dir = 2; break;
        }
    }
    if (lcddev.id == 0x9341 || lcddev.id == 0X6804) //9341/6804,很特殊
    {
        switch (dir)
        {
        case L2R_U2D://从左到右,从上到下
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;
        case L2R_D2U://从左到右,从下到上
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;
        case R2L_U2D://从右到左,从上到下
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;
        case R2L_D2U://从右到左,从下到上
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;
        case U2D_L2R://从上到下,从左到右
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;
        case U2D_R2L://从上到下,从右到左
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;
        case D2U_L2R://从下到上,从左到右
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;
        case D2U_R2L://从下到上,从右到左
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
        }
        dirreg = 0X36;
        regval |= 0X08; //BGR
        if (lcddev.id == 0X6804)regval |= 0x02; //6804的BIT6和9341的反了
        LCD_WriteReg(dirreg, regval);
        if (regval & 0X20)
        {
            if (lcddev.width < lcddev.height) //交换X,Y
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
        else
        {
            if (lcddev.width > lcddev.height) //交换X,Y
            {
                temp = lcddev.width;
                lcddev.width = lcddev.height;
                lcddev.height = temp;
            }
        }
        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(0); LCD_WR_DATA(0);
        LCD_WR_DATA((lcddev.width - 1) >> 8); LCD_WR_DATA((lcddev.width - 1) & 0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(0); LCD_WR_DATA(0);
        LCD_WR_DATA((lcddev.height - 1) >> 8); LCD_WR_DATA((lcddev.height - 1) & 0XFF);
    }
    else
    {
        switch (dir)
        {
        case L2R_U2D://从左到右,从上到下
            regval |= (1 << 5) | (1 << 4) | (0 << 3);
            break;
        case L2R_D2U://从左到右,从下到上
            regval |= (0 << 5) | (1 << 4) | (0 << 3);
            break;
        case R2L_U2D://从右到左,从上到下
            regval |= (1 << 5) | (0 << 4) | (0 << 3);
            break;
        case R2L_D2U://从右到左,从下到上
            regval |= (0 << 5) | (0 << 4) | (0 << 3);
            break;
        case U2D_L2R://从上到下,从左到右
            regval |= (1 << 5) | (1 << 4) | (1 << 3);
            break;
        case U2D_R2L://从上到下,从右到左
            regval |= (1 << 5) | (0 << 4) | (1 << 3);
            break;
        case D2U_L2R://从下到上,从左到右
            regval |= (0 << 5) | (1 << 4) | (1 << 3);
            break;
        case D2U_R2L://从下到上,从右到左
            regval |= (0 << 5) | (0 << 4) | (1 << 3);
            break;
        }
        if (lcddev.id == 0x8989) //8989 IC
        {
            dirreg = 0X11;
            regval |= 0X6040; //65K
        }
        else //其他驱动IC
        {
            dirreg = 0X03;
            regval |= 1 << 12;
        }
        LCD_WriteReg(dirreg, regval);
    }
}
//设置LCD显示方向（6804不支持横屏显示）
//dir:0,竖屏；1,横屏
void LCD_Display_Dir(u8 dir)
{
    if (dir == 0) //竖屏
    {
        lcddev.dir = 0; //竖屏
        lcddev.width = 240;
        lcddev.height = 320;
        if (lcddev.id == 0X9341 || lcddev.id == 0X6804)
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
            if (lcddev.id == 0X6804)
            {
                lcddev.width = 320;
                lcddev.height = 480;
            }
        }
        else if (lcddev.id == 0X8989)
        {
            lcddev.wramcmd = R34;
            lcddev.setxcmd = 0X4E;
            lcddev.setycmd = 0X4F;
        }
				else if(lcddev.id == 0Xb509)
				{
					    lcddev.height = 400;
							lcddev.wramcmd = 0x0202;
							lcddev.setxcmd = 0x0200;
							lcddev.setycmd = 0x0201;
				}
        else
        {
            lcddev.wramcmd = R34;
            lcddev.setxcmd = R32;
            lcddev.setycmd = R33;
					  lcddev.width = 240;

        }
    }
    else if (lcddev.id != 0X6804) //6804不支持横屏显示
    {
        lcddev.dir = 1; //横屏
        lcddev.width = 320;
        lcddev.height = 240;
        if (lcddev.id == 0X9341)
        {
            lcddev.wramcmd = 0X2C;
            lcddev.setxcmd = 0X2A;
            lcddev.setycmd = 0X2B;
        }
        else if (lcddev.id == 0X8989)
        {
            lcddev.wramcmd = R34;
            lcddev.setxcmd = 0X4F;
            lcddev.setycmd = 0X4E;
        }
        else
        {
            lcddev.wramcmd = R34;
            lcddev.setxcmd = R33;
            lcddev.setycmd = R32;
        }
    }
    LCD_Scan_Dir(DFT_SCAN_DIR); //默认扫描方向
}
//////////FSMC 接口设置///////////////////////////////////////////
void FSMC_LCD_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    //使能FSMC外设时钟
    RCC->AHBENR |= 1 << 8;                  //使能FSMC时钟
    //PORTD复用推挽输出
    GPIOD_Init(14, 0x0B, PU); //D0
    GPIOD_Init(15, 0x0B, PU); //D1
    GPIOD_Init(0 , 0x0B, PU); //D2
    GPIOD_Init(1 , 0x0B, PU); //D3
    GPIOE_Init(7 , 0x0B, PU); //D4
    GPIOE_Init(8 , 0x0B, PU); //D5
    GPIOE_Init(9 , 0x0B, PU); //D6
    GPIOE_Init(10, 0x0B, PU); //D7
    GPIOE_Init(11, 0x0B, PU); //D8
    GPIOE_Init(12, 0x0B, PU); //D9
    GPIOE_Init(13, 0x0B, PU); //D10
    GPIOE_Init(14, 0x0B, PU); //D11
    GPIOE_Init(15, 0x0B, PU); //D12
    GPIOD_Init( 8, 0x0B, PU); //D13
    GPIOD_Init( 9, 0x0B, PU); //D14
    GPIOD_Init(10, 0x0B, PU); //D16
	
    //RST    GPIOE_Init(1, 0x03, PU);
		RCC_APB2PeriphClockCmd(RCC_LCD_RST, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_RST_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(LCD_RST_GPIO, &GPIO_InitStructure);

    //LIGHT    GPIOC_Init(0, 3, PU); //推挽输出    //PDout(13) = 1;
		RCC_APB2PeriphClockCmd(RCC_LCD_LIGHT, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_LIGHT_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_LIGHT_GPIO, &GPIO_InitStructure);
		
    //nOE //RD    GPIOD_Init(4, 0x0B, PU);
		RCC_APB2PeriphClockCmd(RCC_LCD_RD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_RD_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LCD_RD_GPIO, &GPIO_InitStructure);
		
    //nWE //WR    GPIOD_Init(5, 0x0B, PU);
		RCC_APB2PeriphClockCmd(RCC_LCD_WR, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_WR_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LCD_WR_GPIO, &GPIO_InitStructure);
		
    //CS //ne1    GPIOD_Init(7, 0x0B, PU);
		RCC_APB2PeriphClockCmd(RCC_LCD_CS, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_CS_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LCD_CS_GPIO, &GPIO_InitStructure);
		
    //RS    GPIOD_Init(12, 0x0B, PU);
		RCC_APB2PeriphClockCmd(RCC_LCD_RS, ENABLE);
    GPIO_InitStructure.GPIO_Pin = LCD_RS_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(LCD_RS_GPIO, &GPIO_InitStructure);
		
		GPIO_SetBits(LCD_LIGHT_GPIO,LCD_LIGHT_Pin);//开背光

		GPIO_SetBits(LCD_RS_GPIO,LCD_RS_Pin);
		GPIO_SetBits(LCD_RST_GPIO,LCD_RS_Pin);  //复位
		delay_ms(1);
		GPIO_ResetBits(LCD_RST_GPIO,LCD_RS_Pin);//复位
		delay_ms(10);
		GPIO_SetBits(LCD_RST_GPIO,LCD_RS_Pin);  //复位
		delay_ms(120);

    //寄存器清零
    //bank1有NE1~4,每一个有一个BCR+TCR，所以总共八个寄存器。
    //这里我们使用NE0 ，也就对应BTCR[0],[1]。
    FSMC_Bank1->BTCR[0] = 0X00000000;
    FSMC_Bank1->BTCR[1] = 0X00000000;
    FSMC_Bank1E->BWTR[0] = 0X00000000;
    //操作BCR寄存器    使用异步模式
    FSMC_Bank1->BTCR[0] |= 1 << 12; //存储器写使能
    FSMC_Bank1->BTCR[0] |= 1 << 14; //读写使用不同的时序
    FSMC_Bank1->BTCR[0] |= 1 << 4;  //存储器数据宽度为16bit
    //操作BTR寄存器
    //读时序控制寄存器
    FSMC_Bank1->BTCR[1] |= 0 << 28; //模式A
    FSMC_Bank1->BTCR[1] |= 1 << 0;  //地址建立时间（ADDSET）为2个HCLK 1/36M=27ns
    //因为液晶驱动IC的读数据的时候，速度不能太快，尤其对1289这个IC。
    FSMC_Bank1->BTCR[1] |= 0XF << 8; //数据保存时间为16个HCLK
    //写时序控制寄存器
    FSMC_Bank1E->BWTR[0] |= 0 << 28; //模式A
    FSMC_Bank1E->BWTR[0] |= 0 << 0; //地址建立时间（ADDSET）为1个HCLK
    //4个HCLK（HCLK=72M）因为液晶驱动IC的写信号脉宽，最少也得50ns。72M/4=24M=55ns
    FSMC_Bank1E->BWTR[0] |= 3 << 8; //数据保存时间为4个HCLK
    //使能BANK1,区域1
    FSMC_Bank1->BTCR[0] |= 1 << 0;  //使能BANK1，区域4
}

//初始化函数
void LCD_Init(void)
{
    char Usart_Buf[20]={0};
    FSMC_LCD_Init();
    delay_ms(50); // delay 50 ms 

    LCD_WriteReg(0x0000, 0x0001);
    delay_ms(50); // delay 50 ms
    lcddev.id = LCD_ReadReg(0x0000);
    if (lcddev.id < 0XFF || lcddev.id == 0XFFFF || lcddev.id == 0X9300) //读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
    {
        //尝试9341 ID的读取
        LCD_WR_REG(0XD3);
        LCD_RD_DATA();              //dummy read
        LCD_RD_DATA();              //读到0X00
        lcddev.id = 0x9328;//LCD_RD_DATA();  //读取93
        lcddev.id <<= 8;
        lcddev.id |= LCD_RD_DATA(); //读取41
        if (lcddev.id != 0X9341)    //非9341,尝试是不是6804
        {
            LCD_WR_REG(0XBF);
            LCD_RD_DATA();          //dummy read
            LCD_RD_DATA();          //读回0X01
            LCD_RD_DATA();          //读回0XD0
            lcddev.id = LCD_RD_DATA(); //这里读回0X68
            lcddev.id <<= 8;
            lcddev.id |= LCD_RD_DATA(); //这里读回0X04
        }
        if (lcddev.id != 0X9341 && lcddev.id != 0X6804)lcddev.id = 0x9341; //新增，用于识别9341
    }
    /*
    ILI9325 读出值为$9325
    SSD1289 读出值为$8989
    R61505  读出值为$1505
    R61509  读出值为$B509
    SPFD5408读出值为$5408
    SPFD5420读出值为$5420
    LGDP4531读出值为$4531
    HX8352-A读出值为$52
    SSD2119 读出值为$9919
    ILI9320 读出值为$9320
    ILI9328 读出值为$9328
    ILI9331 读出值为$9331
    ST7781  读出值为$7783
    HX8347需要读$0067寄存器,值为$0047

    HX8347-A/-B兼容ILI9325/OTM3225/RM68021
    HX8347-D兼容IL9338/SSD1266
    HX8367-A兼容R61505/ILI9335
    HX8347-G兼容S6D04H0
    HX8368-A兼容ILI9342/S6D05K1
    HX8357-B兼容IL9481/OTM4B01/R61581
    HX8371-A兼容S6D05A1
    HX8376-A兼容IL9640/S6D05B0
    HX8369-A兼容LG4572
    HX8353-C/-D兼容NT39121/SPFD54124/ST7735/IL9163/S6D02A1
    */
    sprintf(Usart_Buf, "LCD ID:%x\r\n", lcddev.id);
    USART_OUT(DEBUG_USART, Usart_Buf); //打印LCD ID
    //{time, 0010 ,ms}
    switch (lcddev.id)
    {
    case 0X9341:
        LCD_WR_REG(0xCF);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0xC1);
        LCD_WR_DATA(0X30);
        LCD_WR_REG(0xED);
        LCD_WR_DATA(0x64);
        LCD_WR_DATA(0x03);
        LCD_WR_DATA(0X12);
        LCD_WR_DATA(0X81);
        LCD_WR_REG(0xE8);
        LCD_WR_DATA(0x85);
        LCD_WR_DATA(0x10);
        LCD_WR_DATA(0x7A);
        LCD_WR_REG(0xCB);
        LCD_WR_DATA(0x39);
        LCD_WR_DATA(0x2C);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x34);
        LCD_WR_DATA(0x02);
        LCD_WR_REG(0xF7);
        LCD_WR_DATA(0x20);
        LCD_WR_REG(0xEA);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_REG(0xC0);    //Power control
        LCD_WR_DATA(0x1B);   //VRH[5:0]
        LCD_WR_REG(0xC1);    //Power control
        LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0]
        LCD_WR_REG(0xC5);    //VCM control
        LCD_WR_DATA(0x30);   //3F
        LCD_WR_DATA(0x30);   //3C
        LCD_WR_REG(0xC7);    //VCM control2
        LCD_WR_DATA(0XB7);
        LCD_WR_REG(0x36);    // Memory Access Control
        LCD_WR_DATA(0x48);
        LCD_WR_REG(0x3A);
        LCD_WR_DATA(0x55);
        LCD_WR_REG(0xB1);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x1A);
        LCD_WR_REG(0xB6);    // Display Function Control
        LCD_WR_DATA(0x0A);
        LCD_WR_DATA(0xA2);
        LCD_WR_REG(0xF2);    // 3Gamma Function Disable
        LCD_WR_DATA(0x00);
        LCD_WR_REG(0x26);    //Gamma curve selected
        LCD_WR_DATA(0x01);
        LCD_WR_REG(0xE0);    //Set Gamma
        LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x2A);
        LCD_WR_DATA(0x28);
        LCD_WR_DATA(0x08);
        LCD_WR_DATA(0x0E);
        LCD_WR_DATA(0x08);
        LCD_WR_DATA(0x54);
        LCD_WR_DATA(0XA9);
        LCD_WR_DATA(0x43);
        LCD_WR_DATA(0x0A);
        LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_REG(0XE1);    //Set Gamma
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x15);
        LCD_WR_DATA(0x17);
        LCD_WR_DATA(0x07);
        LCD_WR_DATA(0x11);
        LCD_WR_DATA(0x06);
        LCD_WR_DATA(0x2B);
        LCD_WR_DATA(0x56);
        LCD_WR_DATA(0x3C);
        LCD_WR_DATA(0x05);
        LCD_WR_DATA(0x10);
        LCD_WR_DATA(0x0F);
        LCD_WR_DATA(0x3F);
        LCD_WR_DATA(0x3F);
        LCD_WR_DATA(0x0F);
        LCD_WR_REG(0x2B);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x01);
        LCD_WR_DATA(0x3f);
        LCD_WR_REG(0x2A);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0x00);
        LCD_WR_DATA(0xef);
        LCD_WR_REG(0x11); //Exit Sleep
        delay_ms(120);
        LCD_WR_REG(0x29); //display on
        break;
    case 0x6804:
        LCD_WR_REG(0X11);
        delay_ms(20);
        LCD_WR_REG(0XD0);//VCI1  VCL  VGH  VGL DDVDH VREG1OUT power amplitude setting
        LCD_WR_DATA(0X07);
        LCD_WR_DATA(0X42);
        LCD_WR_DATA(0X1D);
        LCD_WR_REG(0XD1);//VCOMH VCOM_AC amplitude setting
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X1a);
        LCD_WR_DATA(0X09);
        LCD_WR_REG(0XD2);//Operational Amplifier Circuit Constant Current Adjust , charge pump frequency setting
        LCD_WR_DATA(0X01);
        LCD_WR_DATA(0X22);
        LCD_WR_REG(0XC0);//REV SM GS
        LCD_WR_DATA(0X10);
        LCD_WR_DATA(0X3B);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X02);
        LCD_WR_DATA(0X11);

        LCD_WR_REG(0XC5);// Frame rate setting = 72HZ  when setting 0x03
        LCD_WR_DATA(0X03);

        LCD_WR_REG(0XC8);//Gamma setting
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X25);
        LCD_WR_DATA(0X21);
        LCD_WR_DATA(0X05);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X0a);
        LCD_WR_DATA(0X65);
        LCD_WR_DATA(0X25);
        LCD_WR_DATA(0X77);
        LCD_WR_DATA(0X50);
        LCD_WR_DATA(0X0f);
        LCD_WR_DATA(0X00);

        LCD_WR_REG(0XF8);
        LCD_WR_DATA(0X01);

        LCD_WR_REG(0XFE);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X02);

        LCD_WR_REG(0X20);//Exit invert mode

        LCD_WR_REG(0X36);
        LCD_WR_DATA(0X08);//原来是a

        LCD_WR_REG(0X3A);
        LCD_WR_DATA(0X55);//16位模式
        LCD_WR_REG(0X2B);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X01);
        LCD_WR_DATA(0X3F);

        LCD_WR_REG(0X2A);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X00);
        LCD_WR_DATA(0X01);
        LCD_WR_DATA(0XDF);
        delay_ms(120);
        LCD_WR_REG(0X29);
        break;
    case 0x9325:
        LCD_WriteReg(0x00E5, 0x78F0);
        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700);
        LCD_WriteReg(0x0003, 0x1030);
        LCD_WriteReg(0x0004, 0x0000);
        LCD_WriteReg(0x0008, 0x0202);
        LCD_WriteReg(0x0009, 0x0000);
        LCD_WriteReg(0x000A, 0x0000);
        LCD_WriteReg(0x000C, 0x0000);
        LCD_WriteReg(0x000D, 0x0000);
        LCD_WriteReg(0x000F, 0x0000);
        //power on sequence VGHVGL
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x0007);
        LCD_WriteReg(0x0012, 0x0000);
        LCD_WriteReg(0x0013, 0x0000);
        LCD_WriteReg(0x0007, 0x0000);
        //vgh
        LCD_WriteReg(0x0010, 0x1690);
        LCD_WriteReg(0x0011, 0x0227);
        //delayms(100);
        //vregiout
        LCD_WriteReg(0x0012, 0x009D); //0x001b
        //delayms(100);
        //vom amplitude
        LCD_WriteReg(0x0013, 0x1900);
        //delayms(100);
        //vom H
        LCD_WriteReg(0x0029, 0x0025);
        LCD_WriteReg(0x002B, 0x000D);
        //gamma
        LCD_WriteReg(0x0030, 0x0007);
        LCD_WriteReg(0x0031, 0x0303);
        LCD_WriteReg(0x0032, 0x0003); // 0006
        LCD_WriteReg(0x0035, 0x0206);
        LCD_WriteReg(0x0036, 0x0008);
        LCD_WriteReg(0x0037, 0x0406);
        LCD_WriteReg(0x0038, 0x0304); //0200
        LCD_WriteReg(0x0039, 0x0007);
        LCD_WriteReg(0x003C, 0x0602); // 0504
        LCD_WriteReg(0x003D, 0x0008);
        //ram
        LCD_WriteReg(0x0050, 0x0000);
        LCD_WriteReg(0x0051, 0x00EF);
        LCD_WriteReg(0x0052, 0x0000);
        LCD_WriteReg(0x0053, 0x013F);
        LCD_WriteReg(0x0060, 0xA700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006A, 0x0000);
        //
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        //
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0600);

        LCD_WriteReg(0x0007, 0x0133);
        LCD_WriteReg(0x00, 0x0022); //
        break;
    case 0x9328:
        LCD_WriteReg(0x00EC, 0x108F); // internal timeing
        LCD_WriteReg(0x00EF, 0x1234); // ADD
        //LCD_WriteReg(0x00e7,0x0010);
        //LCD_WriteReg(0x0000,0x0001);//开启内部时钟
        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700); //电源开启
        //LCD_WriteReg(0x0003,(1<<3)|(1<<4) );  //65K  RGB
        //DRIVE TABLE(寄存器 03H)
        //BIT3=AM BIT4:5=ID0:1
        //AM ID0 ID1   FUNCATION
        // 0  0   0    R->L D->U
        // 1  0   0    D->U R->L
        // 0  1   0    L->R D->U
        // 1  1   0    D->U L->R
        // 0  0   1    R->L U->D
        // 1  0   1    U->D R->L
        // 0  1   1    L->R U->D 正常就用这个.
        // 1  1   1    U->D L->R
        LCD_WriteReg(0x0003, (1 << 12) | (3 << 4) | (0 << 3) ); //65K
        LCD_WriteReg(0x0004, 0x0000);
        LCD_WriteReg(0x0008, 0x0202);
        LCD_WriteReg(0x0009, 0x0000);
        LCD_WriteReg(0x000a, 0x0000); //display setting
        LCD_WriteReg(0x000c, 0x0001); //display setting
        LCD_WriteReg(0x000d, 0x0000); //0f3c
        LCD_WriteReg(0x000f, 0x0000);
        //电源配置
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x0007);
        LCD_WriteReg(0x0012, 0x0000);
        LCD_WriteReg(0x0013, 0x0000);
        LCD_WriteReg(0x0007, 0x0001);
        delay_ms(50);
        LCD_WriteReg(0x0010, 0x1490);
        LCD_WriteReg(0x0011, 0x0227);
        delay_ms(50);
        LCD_WriteReg(0x0012, 0x008A);
        delay_ms(50);
        LCD_WriteReg(0x0013, 0x1a00);
        LCD_WriteReg(0x0029, 0x0006);
        LCD_WriteReg(0x002b, 0x000d);
        delay_ms(50);
        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x0000);
        delay_ms(50);
        //伽马校正
        LCD_WriteReg(0x0030, 0x0000);
        LCD_WriteReg(0x0031, 0x0604);
        LCD_WriteReg(0x0032, 0x0305);
        LCD_WriteReg(0x0035, 0x0000);
        LCD_WriteReg(0x0036, 0x0C09);
        LCD_WriteReg(0x0037, 0x0204);
        LCD_WriteReg(0x0038, 0x0301);
        LCD_WriteReg(0x0039, 0x0707);
        LCD_WriteReg(0x003c, 0x0000);
        LCD_WriteReg(0x003d, 0x0a0a);
        delay_ms(50);
        LCD_WriteReg(0x0050, 0x0000); //水平GRAM起始位置
        LCD_WriteReg(0x0051, 0x00ef); //水平GRAM终止位置
        LCD_WriteReg(0x0052, 0x0000); //垂直GRAM起始位置
        LCD_WriteReg(0x0053, 0x013f); //垂直GRAM终止位置

        LCD_WriteReg(0x0060, 0xa700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006a, 0x0000);
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);

        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0600);
        //开启显示设置
        LCD_WriteReg(0x0007, 0x0133);
        break;
    case 0x9320:
        LCD_WriteReg(0x00, 0x0000);
        LCD_WriteReg(0x01, 0x0100); //Driver Output Contral.
        LCD_WriteReg(0x02, 0x0700); //LCD Driver Waveform Contral.
        LCD_WriteReg(0x03, 0x1030); //Entry Mode Set.
        //LCD_WriteReg(0x03,0x1018);    //Entry Mode Set.

        LCD_WriteReg(0x04, 0x0000); //Scalling Contral.
        LCD_WriteReg(0x08, 0x0202); //Display Contral 2.(0x0207)
        LCD_WriteReg(0x09, 0x0000); //Display Contral 3.(0x0000)
        LCD_WriteReg(0x0a, 0x0000); //Frame Cycle Contal.(0x0000)
        LCD_WriteReg(0x0c, (1 << 0)); //Extern Display Interface Contral 1.(0x0000)
        LCD_WriteReg(0x0d, 0x0000); //Frame Maker Position.
        LCD_WriteReg(0x0f, 0x0000); //Extern Display Interface Contral 2.
        delay_ms(50);
        LCD_WriteReg(0x07, 0x0101); //Display Contral.
        delay_ms(50);
        LCD_WriteReg(0x10, (1 << 12) | (0 << 8) | (1 << 7) | (1 << 6) | (0 << 4)); //Power Control 1.(0x16b0)
        LCD_WriteReg(0x11, 0x0007);                             //Power Control 2.(0x0001)
        LCD_WriteReg(0x12, (1 << 8) | (1 << 4) | (0 << 0));     //Power Control 3.(0x0138)
        LCD_WriteReg(0x13, 0x0b00);                             //Power Control 4.
        LCD_WriteReg(0x29, 0x0000);                             //Power Control 7.

        LCD_WriteReg(0x2b, (1 << 14) | (1 << 4));
        LCD_WriteReg(0x50, 0);  //Set X Star
        //水平GRAM终止位置Set X End.
        LCD_WriteReg(0x51, 239); //Set Y Star
        LCD_WriteReg(0x52, 0);  //Set Y End.t.
        LCD_WriteReg(0x53, 319); //

        LCD_WriteReg(0x60, 0x2700); //Driver Output Control.
        LCD_WriteReg(0x61, 0x0001); //Driver Output Control.
        LCD_WriteReg(0x6a, 0x0000); //Vertical Srcoll Control.

        LCD_WriteReg(0x80, 0x0000); //Display Position? Partial Display 1.
        LCD_WriteReg(0x81, 0x0000); //RAM Address Start? Partial Display 1.
        LCD_WriteReg(0x82, 0x0000); //RAM Address End-Partial Display 1.
        LCD_WriteReg(0x83, 0x0000); //Displsy Position? Partial Display 2.
        LCD_WriteReg(0x84, 0x0000); //RAM Address Start? Partial Display 2.
        LCD_WriteReg(0x85, 0x0000); //RAM Address End? Partial Display 2.

        LCD_WriteReg(0x90, (0 << 7) | (16 << 0)); //Frame Cycle Contral.(0x0013)
        LCD_WriteReg(0x92, 0x0000); //Panel Interface Contral 2.(0x0000)
        LCD_WriteReg(0x93, 0x0001); //Panel Interface Contral 3.
        LCD_WriteReg(0x95, 0x0110); //Frame Cycle Contral.(0x0110)
        LCD_WriteReg(0x97, (0 << 8)); //
        LCD_WriteReg(0x98, 0x0000); //Frame Cycle Contral.
        LCD_WriteReg(0x07, 0x0173); //(0x0173)
        break;
    case 0X9331:
        LCD_WriteReg(0x00E7, 0x1014);
        LCD_WriteReg(0x0001, 0x0100); // set SS and SM bit
        LCD_WriteReg(0x0002, 0x0200); // set 1 line inversion
        LCD_WriteReg(0x0003, (1 << 12) | (3 << 4) | (1 << 3)); //65K
        //LCD_WriteReg(0x0003, 0x1030); // set GRAM write direction and BGR=1.
        LCD_WriteReg(0x0008, 0x0202); // set the back porch and front porch
        LCD_WriteReg(0x0009, 0x0000); // set non-display area refresh cycle ISC[3:0]
        LCD_WriteReg(0x000A, 0x0000); // FMARK function
        LCD_WriteReg(0x000C, 0x0000); // RGB interface setting
        LCD_WriteReg(0x000D, 0x0000); // Frame marker Position
        LCD_WriteReg(0x000F, 0x0000); // RGB interface polarity
        //*************Power On sequence ****************//
        LCD_WriteReg(0x0010, 0x0000); // SAP, BT[3:0], AP, DSTB, SLP, STB
        LCD_WriteReg(0x0011, 0x0007); // DC1[2:0], DC0[2:0], VC[2:0]
        LCD_WriteReg(0x0012, 0x0000); // VREG1OUT voltage
        LCD_WriteReg(0x0013, 0x0000); // VDV[4:0] for VCOM amplitude
        delay_ms(200); // Dis-charge capacitor power voltage
        LCD_WriteReg(0x0010, 0x1690); // SAP, BT[3:0], AP, DSTB, SLP, STB
        LCD_WriteReg(0x0011, 0x0227); // DC1[2:0], DC0[2:0], VC[2:0]
        delay_ms(50); // Delay 50ms
        LCD_WriteReg(0x0012, 0x000C); // Internal reference voltage= Vci;
        delay_ms(50); // Delay 50ms
        LCD_WriteReg(0x0013, 0x0800); // Set VDV[4:0] for VCOM amplitude
        LCD_WriteReg(0x0029, 0x0011); // Set VCM[5:0] for VCOMH
        LCD_WriteReg(0x002B, 0x000B); // Set Frame Rate
        delay_ms(50); // Delay 50ms
        LCD_WriteReg(0x0020, 0x0000); // GRAM horizontal Address
        LCD_WriteReg(0x0021, 0x013f); // GRAM Vertical Address
        // ----------- Adjust the Gamma Curve ----------//
        LCD_WriteReg(0x0030, 0x0000);
        LCD_WriteReg(0x0031, 0x0106);
        LCD_WriteReg(0x0032, 0x0000);
        LCD_WriteReg(0x0035, 0x0204);
        LCD_WriteReg(0x0036, 0x160A);
        LCD_WriteReg(0x0037, 0x0707);
        LCD_WriteReg(0x0038, 0x0106);
        LCD_WriteReg(0x0039, 0x0707);
        LCD_WriteReg(0x003C, 0x0402);
        LCD_WriteReg(0x003D, 0x0C0F);
        //------------------ Set GRAM area ---------------//
        LCD_WriteReg(0x0050, 0x0000); // Horizontal GRAM Start Address
        LCD_WriteReg(0x0051, 0x00EF); // Horizontal GRAM End Address
        LCD_WriteReg(0x0052, 0x0000); // Vertical GRAM Start Address
        LCD_WriteReg(0x0053, 0x013F); // Vertical GRAM Start Address
        LCD_WriteReg(0x0060, 0x2700); // Gate Scan Line
        LCD_WriteReg(0x0061, 0x0001); // NDL,VLE, REV
        LCD_WriteReg(0x006A, 0x0000); // set scrolling line
        //-------------- Partial Display Control ---------//
        LCD_WriteReg(0x0080, 0x0000);
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        //-------------- Panel Control -------------------//
        LCD_WriteReg(0x0090, 0x0010);
        LCD_WriteReg(0x0092, 0x0600);
        LCD_WriteReg(0x0007, 0x0133); // 262K color and display ON
        break;
    case 0x5408:
        LCD_WriteReg(0x01, 0x0100);
        LCD_WriteReg(0x02, 0x0700); //LCD Driving Waveform Contral
        LCD_WriteReg(0x03, 0x1030); //Entry Mode设置
        //指针从左至右自上而下的自动增模式
        //Normal Mode(Window Mode disable)
        //RGB格式
        //16位数据2次传输的8总线设置
        LCD_WriteReg(0x04, 0x0000); //Scalling Control register
        LCD_WriteReg(0x08, 0x0207); //Display Control 2
        LCD_WriteReg(0x09, 0x0000); //Display Control 3
        LCD_WriteReg(0x0A, 0x0000); //Frame Cycle Control
        LCD_WriteReg(0x0C, 0x0000); //External Display Interface Control 1
        LCD_WriteReg(0x0D, 0x0000); //Frame Maker Position
        LCD_WriteReg(0x0F, 0x0000); //External Display Interface Control 2
        delay_ms(20);
        //TFT 液晶彩色图像显示方法14
        LCD_WriteReg(0x10, 0x16B0); //0x14B0 //Power Control 1
        LCD_WriteReg(0x11, 0x0001); //0x0007 //Power Control 2
        LCD_WriteReg(0x17, 0x0001); //0x0000 //Power Control 3
        LCD_WriteReg(0x12, 0x0138); //0x013B //Power Control 4
        LCD_WriteReg(0x13, 0x0800); //0x0800 //Power Control 5
        LCD_WriteReg(0x29, 0x0009); //NVM read data 2
        LCD_WriteReg(0x2a, 0x0009); //NVM read data 3
        LCD_WriteReg(0xa4, 0x0000);
        LCD_WriteReg(0x50, 0x0000); //设置操作窗口的X轴开始列
        LCD_WriteReg(0x51, 0x00EF); //设置操作窗口的X轴结束列
        LCD_WriteReg(0x52, 0x0000); //设置操作窗口的Y轴开始行
        LCD_WriteReg(0x53, 0x013F); //设置操作窗口的Y轴结束行
        LCD_WriteReg(0x60, 0x2700); //Driver Output Control
        //设置屏幕的点数以及扫描的起始行
        LCD_WriteReg(0x61, 0x0001); //Driver Output Control
        LCD_WriteReg(0x6A, 0x0000); //Vertical Scroll Control
        LCD_WriteReg(0x80, 0x0000); //Display Position – Partial Display 1
        LCD_WriteReg(0x81, 0x0000); //RAM Address Start – Partial Display 1
        LCD_WriteReg(0x82, 0x0000); //RAM address End - Partial Display 1
        LCD_WriteReg(0x83, 0x0000); //Display Position – Partial Display 2
        LCD_WriteReg(0x84, 0x0000); //RAM Address Start – Partial Display 2
        LCD_WriteReg(0x85, 0x0000); //RAM address End – Partail Display2
        LCD_WriteReg(0x90, 0x0013); //Frame Cycle Control
        LCD_WriteReg(0x92, 0x0000); //Panel Interface Control 2
        LCD_WriteReg(0x93, 0x0003); //Panel Interface control 3
        LCD_WriteReg(0x95, 0x0110); //Frame Cycle Control
        LCD_WriteReg(0x07, 0x0173);
        delay_ms(50);
        break;
    case 0x1505:
        // second release on 3/5  ,luminance is acceptable,water wave appear during camera preview
        LCD_WriteReg(0x0007, 0x0000);
        delay_ms(50);
        LCD_WriteReg(0x0012, 0x011C); //0x011A   why need to set several times?
        LCD_WriteReg(0x00A4, 0x0001); //NVM
        LCD_WriteReg(0x0008, 0x000F);
        LCD_WriteReg(0x000A, 0x0008);
        LCD_WriteReg(0x000D, 0x0008);
        //伽马校正
        LCD_WriteReg(0x0030, 0x0707);
        LCD_WriteReg(0x0031, 0x0007); //0x0707
        LCD_WriteReg(0x0032, 0x0603);
        LCD_WriteReg(0x0033, 0x0700);
        LCD_WriteReg(0x0034, 0x0202);
        LCD_WriteReg(0x0035, 0x0002); //?0x0606
        LCD_WriteReg(0x0036, 0x1F0F);
        LCD_WriteReg(0x0037, 0x0707); //0x0f0f  0x0105
        LCD_WriteReg(0x0038, 0x0000);
        LCD_WriteReg(0x0039, 0x0000);
        LCD_WriteReg(0x003A, 0x0707);
        LCD_WriteReg(0x003B, 0x0000); //0x0303
        LCD_WriteReg(0x003C, 0x0007); //?0x0707
        LCD_WriteReg(0x003D, 0x0000); //0x1313//0x1f08
        delay_ms(50);
        LCD_WriteReg(0x0007, 0x0001);
        LCD_WriteReg(0x0017, 0x0001); //开启电源
        delay_ms(50);
        //电源配置
        LCD_WriteReg(0x0010, 0x17A0);
        LCD_WriteReg(0x0011, 0x0217); //reference voltage VC[2:0]   Vciout = 1.00*Vcivl
        LCD_WriteReg(0x0012, 0x011E); //0x011c  //Vreg1out = Vcilvl*1.80   is it the same as Vgama1out ?
        LCD_WriteReg(0x0013, 0x0F00); //VDV[4:0]-->VCOM Amplitude VcomL = VcomH - Vcom Ampl
        LCD_WriteReg(0x002A, 0x0000);
        LCD_WriteReg(0x0029, 0x000A); //0x0001F  Vcomh = VCM1[4:0]*Vreg1out    gate source voltage??
        LCD_WriteReg(0x0012, 0x013E); // 0x013C  power supply on
        //Coordinates Control//
        LCD_WriteReg(0x0050, 0x0000); //0x0e00
        LCD_WriteReg(0x0051, 0x00EF);
        LCD_WriteReg(0x0052, 0x0000);
        LCD_WriteReg(0x0053, 0x013F);
        //Pannel Image Control//
        LCD_WriteReg(0x0060, 0x2700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006A, 0x0000);
        LCD_WriteReg(0x0080, 0x0000);
        //Partial Image Control//
        LCD_WriteReg(0x0081, 0x0000);
        LCD_WriteReg(0x0082, 0x0000);
        LCD_WriteReg(0x0083, 0x0000);
        LCD_WriteReg(0x0084, 0x0000);
        LCD_WriteReg(0x0085, 0x0000);
        //Panel Interface Control//
        LCD_WriteReg(0x0090, 0x0013); //0x0010 frenqucy
        LCD_WriteReg(0x0092, 0x0300);
        LCD_WriteReg(0x0093, 0x0005);
        LCD_WriteReg(0x0095, 0x0000);
        LCD_WriteReg(0x0097, 0x0000);
        LCD_WriteReg(0x0098, 0x0000);

        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700);
        LCD_WriteReg(0x0003, 0x1038); //扫描方向 上->下  左->右
        LCD_WriteReg(0x0004, 0x0000);
        LCD_WriteReg(0x000C, 0x0000);
        LCD_WriteReg(0x000F, 0x0000);
        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x0000);
        LCD_WriteReg(0x0007, 0x0021);
        delay_ms(20);
        LCD_WriteReg(0x0007, 0x0061);
        delay_ms(20);
        LCD_WriteReg(0x0007, 0x0173);
        delay_ms(20);
        break;
    case 0xB505:
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);

        LCD_WriteReg(0x00a4, 0x0001);
        delay_ms(20);
        LCD_WriteReg(0x0060, 0x2700);
        LCD_WriteReg(0x0008, 0x0202);

        LCD_WriteReg(0x0030, 0x0214);
        LCD_WriteReg(0x0031, 0x3715);
        LCD_WriteReg(0x0032, 0x0604);
        LCD_WriteReg(0x0033, 0x0e16);
        LCD_WriteReg(0x0034, 0x2211);
        LCD_WriteReg(0x0035, 0x1500);
        LCD_WriteReg(0x0036, 0x8507);
        LCD_WriteReg(0x0037, 0x1407);
        LCD_WriteReg(0x0038, 0x1403);
        LCD_WriteReg(0x0039, 0x0020);

        LCD_WriteReg(0x0090, 0x001a);
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x0007);
        LCD_WriteReg(0x0012, 0x0000);
        LCD_WriteReg(0x0013, 0x0000);
        delay_ms(20);

        LCD_WriteReg(0x0010, 0x0730);
        LCD_WriteReg(0x0011, 0x0137);
        delay_ms(20);

        LCD_WriteReg(0x0012, 0x01b8);
        delay_ms(20);

        LCD_WriteReg(0x0013, 0x0f00);
        LCD_WriteReg(0x002a, 0x0080);
        LCD_WriteReg(0x0029, 0x0048);
        delay_ms(20);

        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0700);
        LCD_WriteReg(0x0003, 0x1038); //扫描方向 上->下  左->右
        LCD_WriteReg(0x0008, 0x0202);
        LCD_WriteReg(0x000a, 0x0000);
        LCD_WriteReg(0x000c, 0x0000);
        LCD_WriteReg(0x000d, 0x0000);
        LCD_WriteReg(0x000e, 0x0030);
        LCD_WriteReg(0x0050, 0x0000);
        LCD_WriteReg(0x0051, 0x00ef);
        LCD_WriteReg(0x0052, 0x0000);
        LCD_WriteReg(0x0053, 0x013f);
        LCD_WriteReg(0x0060, 0x2700);
        LCD_WriteReg(0x0061, 0x0001);
        LCD_WriteReg(0x006a, 0x0000);
        //LCD_WriteReg(0x0080,0x0000);
        //LCD_WriteReg(0x0081,0x0000);
        LCD_WriteReg(0x0090, 0X0011);
        LCD_WriteReg(0x0092, 0x0600);
        LCD_WriteReg(0x0093, 0x0402);
        LCD_WriteReg(0x0094, 0x0002);
        delay_ms(20);

        LCD_WriteReg(0x0007, 0x0001);
        delay_ms(20);
        LCD_WriteReg(0x0007, 0x0061);
        LCD_WriteReg(0x0007, 0x0173);

        LCD_WriteReg(0x0020, 0x0000);
        LCD_WriteReg(0x0021, 0x0000);
        LCD_WriteReg(0x00, 0x22);
        break;
    case 0xC505:

        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);
        delay_ms(20);
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x0000, 0x0000);
        LCD_WriteReg(0x00a4, 0x0001);
        delay_ms(20);
        LCD_WriteReg(0x0060, 0x2700);
        LCD_WriteReg(0x0008, 0x0806);

        LCD_WriteReg(0x0030, 0x0703); //gamma setting
        LCD_WriteReg(0x0031, 0x0001);
        LCD_WriteReg(0x0032, 0x0004);
        LCD_WriteReg(0x0033, 0x0102);
        LCD_WriteReg(0x0034, 0x0300);
        LCD_WriteReg(0x0035, 0x0103);
        LCD_WriteReg(0x0036, 0x001F);
        LCD_WriteReg(0x0037, 0x0703);
        LCD_WriteReg(0x0038, 0x0001);
        LCD_WriteReg(0x0039, 0x0004);



        LCD_WriteReg(0x0090, 0x0015);   //80Hz
        LCD_WriteReg(0x0010, 0X0410);   //BT,AP
        LCD_WriteReg(0x0011, 0x0247);   //DC1,DC0,VC
        LCD_WriteReg(0x0012, 0x01BC);
        LCD_WriteReg(0x0013, 0x0e00);
        delay_ms(120);
        LCD_WriteReg(0x0001, 0x0100);
        LCD_WriteReg(0x0002, 0x0200);
        LCD_WriteReg(0x0003, 0x1030);

        LCD_WriteReg(0x000A, 0x0008);
        LCD_WriteReg(0x000C, 0x0000);

        LCD_WriteReg(0x000E, 0x0020);
        LCD_WriteReg(0x000F, 0x0000);
        LCD_WriteReg(0x0020, 0x0000);   //H Start
        LCD_WriteReg(0x0021, 0x0000);   //V Start
        LCD_WriteReg(0x002A, 0x003D);   //vcom2
        delay_ms(20);
        LCD_WriteReg(0x0029, 0x002d);
        LCD_WriteReg(0x0050, 0x0000);
        LCD_WriteReg(0x0051, 0xD0EF);
        LCD_WriteReg(0x0052, 0x0000);
        LCD_WriteReg(0x0053, 0x013F);
        LCD_WriteReg(0x0061, 0x0000);
        LCD_WriteReg(0x006A, 0x0000);
        LCD_WriteReg(0x0092, 0x0300);

        LCD_WriteReg(0x0093, 0x0005);
        LCD_WriteReg(0x0007, 0x0100);
        break;
    case 0x8989:
        LCD_WriteReg(0x0000, 0x0001); //打开晶振
        LCD_WriteReg(0x0003, 0xA8A4); //0xA8A4
        LCD_WriteReg(0x000C, 0x0000);
        LCD_WriteReg(0x000D, 0x080C);
        LCD_WriteReg(0x000E, 0x2B00);
        LCD_WriteReg(0x001E, 0x00B0);
        LCD_WriteReg(0x0001, 0x2B3F); //驱动输出控制320*240  0x6B3F
        LCD_WriteReg(0x0002, 0x0600);
        LCD_WriteReg(0x0010, 0x0000);
        LCD_WriteReg(0x0011, 0x6078); //定义数据格式  16位色         横屏 0x6058
        LCD_WriteReg(0x0005, 0x0000);
        LCD_WriteReg(0x0006, 0x0000);
        LCD_WriteReg(0x0016, 0xEF1C);
        LCD_WriteReg(0x0017, 0x0003);
        LCD_WriteReg(0x0007, 0x0233); //0x0233
        LCD_WriteReg(0x000B, 0x0000);
        LCD_WriteReg(0x000F, 0x0000); //扫描开始地址
        LCD_WriteReg(0x0041, 0x0000);
        LCD_WriteReg(0x0042, 0x0000);
        LCD_WriteReg(0x0048, 0x0000);
        LCD_WriteReg(0x0049, 0x013F);
        LCD_WriteReg(0x004A, 0x0000);
        LCD_WriteReg(0x004B, 0x0000);
        LCD_WriteReg(0x0044, 0xEF00);
        LCD_WriteReg(0x0045, 0x0000);
        LCD_WriteReg(0x0046, 0x013F);
        LCD_WriteReg(0x0030, 0x0707);
        LCD_WriteReg(0x0031, 0x0204);
        LCD_WriteReg(0x0032, 0x0204);
        LCD_WriteReg(0x0033, 0x0502);
        LCD_WriteReg(0x0034, 0x0507);
        LCD_WriteReg(0x0035, 0x0204);
        LCD_WriteReg(0x0036, 0x0204);
        LCD_WriteReg(0x0037, 0x0502);
        LCD_WriteReg(0x003A, 0x0302);
        LCD_WriteReg(0x003B, 0x0302);
        LCD_WriteReg(0x0023, 0x0000);
        LCD_WriteReg(0x0024, 0x0000);
        LCD_WriteReg(0x0025, 0x8000);
        LCD_WriteReg(0x004f, 0);       //行首址0
        LCD_WriteReg(0x004e, 0);       //列首址0
        break;
    case 0x4531:
        LCD_WriteReg(0X00, 0X0001);
        delay_ms(10);
        LCD_WriteReg(0X10, 0X1628);
        LCD_WriteReg(0X12, 0X000e); //0x0006
        LCD_WriteReg(0X13, 0X0A39);
        delay_ms(10);
        LCD_WriteReg(0X11, 0X0040);
        LCD_WriteReg(0X15, 0X0050);
        delay_ms(10);
        LCD_WriteReg(0X12, 0X001e); //16
        delay_ms(10);
        LCD_WriteReg(0X10, 0X1620);
        LCD_WriteReg(0X13, 0X2A39);
        delay_ms(10);
        LCD_WriteReg(0X01, 0X0100);
        LCD_WriteReg(0X02, 0X0300);
        LCD_WriteReg(0X03, 0X1038); //改变方向的
        LCD_WriteReg(0X08, 0X0202);
        LCD_WriteReg(0X0A, 0X0008);
        LCD_WriteReg(0X30, 0X0000);
        LCD_WriteReg(0X31, 0X0402);
        LCD_WriteReg(0X32, 0X0106);
        LCD_WriteReg(0X33, 0X0503);
        LCD_WriteReg(0X34, 0X0104);
        LCD_WriteReg(0X35, 0X0301);
        LCD_WriteReg(0X36, 0X0707);
        LCD_WriteReg(0X37, 0X0305);
        LCD_WriteReg(0X38, 0X0208);
        LCD_WriteReg(0X39, 0X0F0B);
        LCD_WriteReg(0X41, 0X0002);
        LCD_WriteReg(0X60, 0X2700);
        LCD_WriteReg(0X61, 0X0001);
        LCD_WriteReg(0X90, 0X0210);
        LCD_WriteReg(0X92, 0X010A);
        LCD_WriteReg(0X93, 0X0004);
        LCD_WriteReg(0XA0, 0X0100);
        LCD_WriteReg(0X07, 0X0001);
        LCD_WriteReg(0X07, 0X0021);
        LCD_WriteReg(0X07, 0X0023);
        LCD_WriteReg(0X07, 0X0033);
        LCD_WriteReg(0X07, 0X0133);
        LCD_WriteReg(0XA0, 0X0000);
        break;
    case 0x4535:
        LCD_WriteReg(0X15, 0X0030);
        LCD_WriteReg(0X9A, 0X0010);
        LCD_WriteReg(0X11, 0X0020);
        LCD_WriteReg(0X10, 0X3428);
        LCD_WriteReg(0X12, 0X0002); //16
        LCD_WriteReg(0X13, 0X1038);
        delay_ms(40);
        LCD_WriteReg(0X12, 0X0012); //16
        delay_ms(40);
        LCD_WriteReg(0X10, 0X3420);
        LCD_WriteReg(0X13, 0X3038);
        delay_ms(70);
        LCD_WriteReg(0X30, 0X0000);
        LCD_WriteReg(0X31, 0X0402);
        LCD_WriteReg(0X32, 0X0307);
        LCD_WriteReg(0X33, 0X0304);
        LCD_WriteReg(0X34, 0X0004);
        LCD_WriteReg(0X35, 0X0401);
        LCD_WriteReg(0X36, 0X0707);
        LCD_WriteReg(0X37, 0X0305);
        LCD_WriteReg(0X38, 0X0610);
        LCD_WriteReg(0X39, 0X0610);

        LCD_WriteReg(0X01, 0X0100);
        LCD_WriteReg(0X02, 0X0300);
        LCD_WriteReg(0X03, 0X1030); //改变方向的
        LCD_WriteReg(0X08, 0X0808);
        LCD_WriteReg(0X0A, 0X0008);
        LCD_WriteReg(0X60, 0X2700);
        LCD_WriteReg(0X61, 0X0001);
        LCD_WriteReg(0X90, 0X013E);
        LCD_WriteReg(0X92, 0X0100);
        LCD_WriteReg(0X93, 0X0100);
        LCD_WriteReg(0XA0, 0X3000);
        LCD_WriteReg(0XA3, 0X0010);
        LCD_WriteReg(0X07, 0X0001);
        LCD_WriteReg(0X07, 0X0021);
        LCD_WriteReg(0X07, 0X0023);
        LCD_WriteReg(0X07, 0X0033);
        LCD_WriteReg(0X07, 0X0133);
        break;
    case 0xb509:
        LCD_WR_CMD(0x0400, 0x06200);
        LCD_WR_CMD(0x0008, 0x00808);
        LCD_WR_CMD(0x0300, 0x00C00);//gamma
        LCD_WR_CMD(0x0301, 0x05A0B);
        LCD_WR_CMD(0x0302, 0x00906);
        LCD_WR_CMD(0x0303, 0x01017);
        LCD_WR_CMD(0x0304, 0x02300);
        LCD_WR_CMD(0x0305, 0x01700);
        LCD_WR_CMD(0x0306, 0x06309);
        LCD_WR_CMD(0x0307, 0x00C09);
        LCD_WR_CMD(0x0308, 0x0100C);
        LCD_WR_CMD(0x0309, 0x02232);
        LCD_WR_CMD(0x0010, 0x00016);//69.5Hz
        LCD_WR_CMD(0x0011, 0x00101);//
        LCD_WR_CMD(0x0012, 0x00000);//
        LCD_WR_CMD(0x0013, 0x00001);//
        LCD_WR_CMD(0x0100, 0x00330);//BT,AP
        LCD_WR_CMD(0x0101, 0x00237);//DC0,DC1,VC
        LCD_WR_CMD(0x0103, 0x00F00);//VDV
        LCD_WR_CMD(0x0280, 0x06100);//VCM
        LCD_WR_CMD(0x0102, 0x0C1B0);//VRH,VCMR,PSON,PON
        //{time, 0100 ,ms}
        LCD_WR_CMD(0x0001, 0x00100);
        LCD_WR_CMD(0x0002, 0x00100);
        LCD_WR_CMD(0x0003, 0x01030); //显示方向模式寄存器  横向
        ////18//30//10//28//08//00//20////图像显示方向为左下起  行递增  列递减
        LCD_WR_CMD(0x0009, 0x00001);
        LCD_WR_CMD(0x000C, 0x00000);
        LCD_WR_CMD(0x0090, 0x08000);
        LCD_WR_CMD(0x000F, 0x00000);
        LCD_WR_CMD(0x0210, 0x00000);
        LCD_WR_CMD(0x0211, 0x000EF);
        LCD_WR_CMD(0x0212, 0x00000);
        LCD_WR_CMD(0x0213, 0x0018F);//432=1AF, 400=18F
        LCD_WR_CMD(0x0500, 0x00000);
        LCD_WR_CMD(0x0501, 0x00000);
        LCD_WR_CMD(0x0502, 0x0005F);
        LCD_WR_CMD(0x0401, 0x00001);
        LCD_WR_CMD(0x0404, 0x00000);
        //{time, 0100 ,ms}
        LCD_WR_CMD(0x0007, 0x00100);//BASEE
        //{time, 0100 ,ms}
        LCD_WR_CMD(0x0200, 0x00000);
        LCD_WR_CMD(0x0201, 0x00000);
        break;
    }
    LCD_Display_Dir(1);         //默认为竖屏
    LCD_LED = 1;                //点亮背光
    LCD_Clear(WHITE);
   // POINT_COLOR = 0;
//		上下左右颠倒 9328
//		LCD_WR_CMD(0x0001, 0x00001);
//		LCD_WR_CMD(0x0400, 0x00000);
}

//++++++++根据数值获取各位的数字字符数据首地址+++++++++++++++++++++++++++++++++++++++++++
// unsigned char* num_pub(unsigned  int a)
// {
//     unsigned char* b;
//     switch (a)
//     {
//     case 0x01: b = &zm1[0]; break;
//     case 0x02: b = &zm2[0]; break;
//     case 0x03: b = &zm3[0]; break;
//     case 0x04: b = &zm4[0]; break;
//     case 0x05: b = &zm5[0]; break;
//     case 0x06: b = &zm6[0]; break;
//     case 0x07: b = &zm7[0]; break;
//     case 0x08: b = &zm8[0]; break;
//     case 0x09: b = &zm9[0]; break;
//     case 0x00: b = &zm0[0]; break;
//     default: b = &zm0[0]; break;
//     }
//     return (b);
// }

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

