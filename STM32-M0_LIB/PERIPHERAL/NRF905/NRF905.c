#include "./nrf905.h"
#include "nRF905_config.h"
#include "GPIO.h"


/************************************************************/
//发送数据缓冲区
/************************************************************/

u8 TxRxBuf[TxRxBuf_Len];//发送接受数据缓存
u8 TxAddress[4] = {0xcc, 0xcc, 0xcc, 0xcc};


/***********************************************************/
//nrf905控制指令
/***********************************************************/
#define WC      0x00//写配置寄存器 此部分程序属于硬性设置  一般无需改变
#define RC      0x10//读配置寄存器
#define WTP     0x20//向TX―Payload寄存器写入 发送有效数据
#define RTP     0x21//从TX―Played寄存器读取 发送有效数据
#define WTA     0x22//向TX-ADDress寄存器写入发送地址
#define RTA     0x23//从TX-ADDress寄存器读取发送地址
#define RRP     0x24//从RX―Played寄存器读取 接收道德有效数据


/***************************************************************/
// IO 初始化
/***************************************************************/
void GPIO_NRF905_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOF,  ENABLE);
    //DR   AM   CD   MISO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_1 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //CSN  SCK  MOSI  TRX_CE  PWR_UP  TXEN
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

//---------------------------------

#define BYTE_BIT0   0x01
#define BYTE_BIT7   0x80
#define DATA7   ((DATA_BUF&BYTE_BIT7)!= 0)
#define DATA0   ((DATA_BUF&BYTE_BIT0)!= 0)
/****************************************************************/
//  从NRF905读一个字节
/****************************************************************/
static u8 SPI_Read(void)
{
    u8 i, DATA_BUF;
    for (i = 0; i < 8; i++)
    {
        DATA_BUF = DATA_BUF << 1;
        SCK_H;
        if (MISO)
            DATA_BUF |= BYTE_BIT0;
        else
            DATA_BUF &= ~BYTE_BIT0;
        SCK_L;
    }
    return DATA_BUF;

}

/*******************************************************************/
//  向NRF905写入一个字节
/********************************************************************/
static void SPI_Write(u8 DATA_BUF)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (DATA7)
        {
            MOSI_H;
        }
        else
            MOSI_L;
        SCK_H;
        DATA_BUF = DATA_BUF << 1;
        SCK_L;
    }
}


//*************************************************************
//  NRF905初始化
//****************************************************************
void nRF905Init(void)
{
    GPIO_NRF905_Init();
    CSN_H;                    // Spi  disable
    SCK_L;                    // Spi clock line init low
    DR_H;                     // Init DR for input
    AM_H;                     // Init AM for input
    CD_H;                     // Init CD for input
    PWR_UP_H;                 // nRF905 power on
    TRX_CE_L;                 // Set nRF905 in standby mode
    TXEN_L;               // set radio in Rx mode
    Config_Nrf905();
}
void Read_Config_Nrf905(u8 * buf1,u8 num)
{
    u8 i;
    CSN_L;
    SPI_Write(RC);
    for (i = 0; i < num; i++)
    {
        buf1[i]=SPI_Read();
    }
    CSN_H;
}

u8 NRF905_Check(void)
{
    u8 buf1[sizeof(RFConf)];
    u8 i;
    /*写入5个字节的地址. */
    Read_Config_Nrf905(buf1,sizeof(RFConf));
    /*比较*/
    for (i = 0; i < sizeof(RFConf); i++)
    {
        if (buf1[i] != RFConf[i])
            break;
    }
    if (i == sizeof(RFConf))
        return SUCCESS ; //MCU与NRF成功连接
    else
        return ERROR ; //MCU与NRF不正常连接
}
/**************************************************************/
//  将寄存器配置指令写入NRF905
/*************************************************************/
void Config_Nrf905(void)
{
    u8 i;
    CSN_L;
    SPI_Write(WC);
    for (i = 0; i < 10; i++)
    {
        SPI_Write(RFConf[i]);
    }
    CSN_H;
}


//**************************************************************************************************
//NRF905发送数据
//**************************************************************************************************
void TxPacket(u8 *TxRxBuf)
{
    u8 i;
    CSN_L;
    TRX_CE_L;                 // Set TRX_CE low
    SPI_Write(WTP);//向TX―Payload寄存器写入 发送有效数据
    for (i = 0; i < TxRxBuf_Len; i++)
    {
        SPI_Write(TxRxBuf[i]);      // Write 32 bytes Tx data
    }
    CSN_H;
    CSN_L;
    SPI_Write(WTA);//向TX-ADDress寄存器写入发送地址
    for (i = 0; i < 4; i++)     // 写入与对方地址一样的地址
    {
        SPI_Write(TxAddress[i]);
    }
    CSN_H;                    // Spi disable
    TRX_CE_H;                 // Set TRX_CE high,start Tx data transmission
}


//**************************************************************************************************
//设置NRF905为发送模式
//**************************************************************************************************
void SetTxMode(void)
{
    TXEN_H;
    TRX_CE_L;
    //delay_ms(1);                    // delay for mode change(>=650us)
}


//**************************************************************************************************
//设置NRF905为接收模式
//**************************************************************************************************
void SetRxMode(void)
{
    TXEN_L;
    TRX_CE_H;
    //delay_ms(1);                    // delay for mode change(>=650us)
}


//**************************************************************************************************
//判断数据接收状态
//**************************************************************************************************
u8 CheckDR(void)        //检查是否有新数据传入 Data Ready
{
    if (DR == 1)
    {
        DR_L;
        return 1;
    }
    else
    {
        return 0;
    }
}
int CheckTxOver(void)        //检查是否发送完成
{
	return DR;
}

int CheckRx(void)        //检查是否为接收模式
{
		return TXEN?0:1;
}

//**************************************************************************************************
//从NRF905提取接收到的数据
//**************************************************************************************************
void RxPacket(void)
{
    u8 i;
    while (DR == 0);
    TRX_CE_L;
    CSN_L;                    // Spi enable for write a spi command
    SPI_Write(RRP);
    for (i = 0 ; i < TxRxBuf_Len ; i++)
    {
        TxRxBuf[i] = SPI_Read();// Read data and save to buffer
    }
    CSN_H;
    while (DR || AM);
    TRX_CE_H;
}
