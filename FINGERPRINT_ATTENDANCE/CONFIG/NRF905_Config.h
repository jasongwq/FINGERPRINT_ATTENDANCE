#ifndef __NRF905_CONFIG_H__
#define __NRF905_CONFIG_H__
/****************************************/
/***NRF905***/
/********************************************
***********************************************\
 * __________________*
 * |13 VCC | 14 TXEN | *   B1
 * |-------*         | *
 * |11 CE    12 PWR  | *F0 F1
 * |                 | *
 * | 9 UCK   10 CD   | *A0 A1
 * |                 | *
 * | 8 AM     7 DR   | *A4 A6
 * |                 | *
 * | 6 MISO   5 MOSI | *A7 A5
 * |                 | *
 * | 4 SCK    3 CSN  | *A3 A2
 * |---------------- | *
 * | 2 GND  | 1 GND  | *
 * ------------------*
\**********************************************/
//引脚输入模式
#define DR        A6       //PCin(6)//PC6.37
#define DR_H  Set_A6       //PCin(6)//PC6.37
#define DR_L  Clr_A6       //PCin(6)//PC6.37
#define AM        A4       //PCin(7)//PC7.38
#define AM_H  Set_A4       //PCin(7)//PC7.38
#define AM_L  Clr_A4       //PCin(7)//PC7.38
#define CD        A1       //PCin(8)//PC8.39
#define CD_H  Set_A1       //PCin(8)//PC8.39
#define CD_L  Clr_A1       //PCin(8)//PC8.39
#define MISO      A7       //PBin(15)  //PB15.36 
//引脚输出模式
#define CSN_H     Set_A2   //PBout(12)   //PB12.33 
#define CSN_L     Clr_A2   //PBout(12)   //PB12.33 
#define SCK_H     Set_A3   //PBout(13)   //PB13.34   
#define SCK_L     Clr_A3   //PBout(13)   //PB13.34   
#define MOSI_H    Set_A5   //PBout(14)  //PB14.35 
#define MOSI_L    Clr_A5   //PBout(14)  //PB14.35 
#define TRX_CE_H  Set_F0   //PCout(9)  //PC9.40
#define TRX_CE_L  Clr_F0   //PCout(9)  //PC9.40
#define PWR_UP_H  Set_F1   //PAout(8)  //PA8.41
#define PWR_UP_L  Clr_F1   //PAout(8)  //PA8.41
#define TXEN_H    Set_B1   //PAout(9)    //PA9.42
#define TXEN_L    Clr_B1   //PAout(9)    //PA9.42
#define TXEN      Get_B1   //PAout(9)    //PA9.42
//#define uCLK

#define TxRxBuf_Len 0x20

/************************************************************/
//  NRF905寄存器配置
/************************************************************/
static u8 RFConf[] =
{
    //字节0 载波频率
    0x4c,                    //CH_NO,配置频段在430MHZ
    //字节1
    0x0c,                    //输出功率为10db,不重发，节电为正常模式
    //字节2
    0x44,                    //地址宽度设置，为4字节
    //字节3 4
    TxRxBuf_Len, TxRxBuf_Len,//接收发送有效数据长度为4字节
    //字节5 6 7 8
    0xCC, 0xCC, 0xCC, 0xCC,  //接收地址
    //字节9
    0xd8,                    //4mhz ,外部时钟信号不使能，16M晶振,CRC允许，16位CRC校验，
};


#endif

