#include "G32-M0_Nrf24l01.h"
#include "spi.h"

/****************************************/
/***NRF24L01***/
/********************************************
***********************************************\
 * _________________ *
 * |8 IQR    7 MISO| *
 * |               | *
 * |6 MOSI   5 SCK | *
 * |               | *
 * |4 CSN    3 CE  | *
 * |       --------| *
 * |2 VCC  | 1 GND | *
 * ----------------- *
\**********************************************/
uint8_t NRF24L01_RXDATA[RX_PLOAD_WIDTH];//nrf24l01接收到的数据
uint8_t NRF24L01_TXDATA[RX_PLOAD_WIDTH];//nrf24l01需要发送的数据
uint8_t NRF24L01_TXDATA_RC[RX_PLOAD_WIDTH];//nrf24l01需要发送的数据
u8  TX_ADDRESS[TX_ADR_WIDTH] = {0xE1, 0xE2, 0xE3, 0x44, 0xE5}; //本地地址
u8  RX_ADDRESS[RX_ADR_WIDTH] = {0xE1, 0xE2, 0xE3, 0x44, 0xE5}; //接收地址
//////////////////////////////////////////////////////////////////////////////////
static void NRF24L01_GPIOConfig(void)
{
    NRF24l01_CE_Init
    NRF24l01_CSN_Init
    NRF24l01_IQR_Init
}
#ifdef NRF24L01_SPI1
#define Spi_RW SPI1_ReadWriteByte
static void Nrf_Spi1_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable SPI and GPIO clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource5 | GPIO_PinSource6 | GPIO_PinSource7, GPIO_AF_0); //连接到外设

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;           //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;              //选择了串行时钟的稳态:时钟悬空 高/低
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;            //数据捕获于第X个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                   //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;     //定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;                    //CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);                             //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF); //配置 SPI 的 FIFO 接收阈值(接收8bits就置 SPI_I2S_FLAG_RXNE)
    SPI_Cmd(SPI1, ENABLE);      //使能SPI外设
    SPI_CE_L();
    SPI_CSN_H();
}
#elif defined NRF24L01_SPI2
#define Spi_RW SPI2_ReadWriteByte
static void Nrf_Spi2_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    // Enable SPI and GPIO clocks
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource13 | GPIO_PinSource14 | GPIO_PinSource15, GPIO_AF_0); //连接到外设

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;           //设置SPI工作模式:设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;   //设置SPI的数据大小:SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;              //选择了串行时钟的稳态:时钟悬空 高/低
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;            //数据捕获于第X个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                   //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;     //定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;                    //CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);                             //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_RxFIFOThresholdConfig(SPI2, SPI_RxFIFOThreshold_QF); //配置 SPI 的 FIFO 接收阈值(接收8bits就置 SPI_I2S_FLAG_RXNE)
    SPI_Cmd(SPI2, ENABLE);      //使能SPI外设
    SPI_CE_L();
    SPI_CSN_H();
}
#elif defined NRF24L01_SPISOFT
void Nrf_Spi0_Init(void)
{
    SPI0_Init();
		SPI_CE_L();
    SPI_CSN_H();
}
#define Spi_RW SPI0_ReadWriteByte
#endif
/*
*****************************************************************
* 写寄存器
*****************************************************************
*/
uint8_t NRF_Write_Reg(uint8_t reg, uint8_t value)
{
    uint8_t status;
    SPI_CSN_L();                /* 选通器件 */
    status = Spi_RW(reg);       /* 写寄存器地址 */
    Spi_RW(value);              /* 写数据 */
    SPI_CSN_H();                /* 禁止该器件 */
    return    status;
}
/*
*****************************************************************
* 读寄存器
*****************************************************************
*/
uint8_t NRF_Read_Reg(uint8_t reg)
{
    uint8_t reg_val;
    SPI_CSN_L();              /* 选通器件 */
    Spi_RW(reg);              /* 写寄存器地址 */
    reg_val = Spi_RW(0);      /* 读取该寄存器返回数据 */
    SPI_CSN_H();              /* 禁止该器件 */
    return  reg_val;
}
/*
*****************************************************************
* 写缓冲区
*****************************************************************
*/
uint8_t NRF_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
    uint8_t i;
    uint8_t status;
    SPI_CSN_L();            /* 选通器件 */
    status = Spi_RW(reg);   /* 写寄存器地址 */
    for (i = 0; i < uchars; i++)
    {
        Spi_RW(pBuf[i]);    /* 写数据 */
    }
    SPI_CSN_H();            /* 禁止该器件 */
    return  status;
}
/*
*****************************************************************
* 读缓冲区
*****************************************************************
*/
uint8_t NRF_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t uchars)
{
    uint8_t i;
    uint8_t status;
    SPI_CSN_L();            /* 选通器件 */
    status = Spi_RW(reg);   /* 写寄存器地址 */
    for (i = 0; i < uchars; i++)
    {
        pBuf[i] = Spi_RW(0); /* 读取返回数据 */
    }
    SPI_CSN_H();             /* 禁止该器件 */
    return  status;
}
/*
*****************************************************************
* 写数据包
*****************************************************************
*/
void NRF_TxPacket(uint8_t *tx_buf, uint8_t len)
{
    SPI_CE_L();      //StandBy I模式
    NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // 装载接收端地址
    NRF_Write_Buf(WR_TX_PLOAD, tx_buf, len);             // 装载数据
    SPI_CE_H();      //置高CE，激发数据发送
}
void NRF_TxPacket_AP(uint8_t *tx_buf, uint8_t len)
{
    SPI_CE_L();      //StandBy I模式
    NRF_Write_Buf(0xa8, tx_buf, len);            // 装载数据
    SPI_CE_H();      //置高CE
}
u8 Nrf24l01_Check(void)
{
    u8 buf1[5];
    u8 i;
    /*写入5个字节的地址. */
    NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, 5);
    /*读出写入的地址 */
    NRF_Read_Buf(TX_ADDR, buf1, 5);
    /*比较*/
    for (i = 0; i < 5; i++)
    {
        if (buf1[i] != TX_ADDRESS[i])
            break;
    }
    if (i == 5)
        return SUCCESS ; //MCU与NRF成功连接
    else
        return ERROR ; //MCU与NRF不正常连接
}
//MODEL_TX2,40
/*
24L01初始化
model：24L01 模式
ch：   通道
*/
void Nrf24l01_Init(u8 model, u8 ch)
{
    NRF24L01_GPIOConfig();
#ifdef NRF24L01_SPI1
    Nrf_Spi1_Init();
#elif defined NRF24L01_SPI2
    Nrf_Spi2_Init();
#else
    Nrf_Spi0_Init();
#endif
    SPI_CE_L();
    NRF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH);//写RX节点地址
    NRF_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);   //写TX节点地址
    NRF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x01);                         //使能通道0的自动应答
    NRF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);                     //使能通道0的接收地址
    NRF_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);                    //设置自动重发间隔时间:500us;最大自动重发次数:10次
    NRF_Write_Reg(NRF_WRITE_REG + RF_CH, ch);                           //设置RF通道为CHANAL
    NRF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0x0f);                      //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
    //NRF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x07);                       //设置TX发射参数,0db增益,1Mbps,低噪声增益开启
    /////////////////////////////////////////////////////////
    if (model == 1)         //RX
    {
        NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);                            //选择通道0的有效数据宽度
        NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);         // IRQ收发完成中断开启,16位CRC,主接收
    }
    else if (model == 2)    //TX
    {
        NRF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH);                            //选择通道0的有效数据宽度
        NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);         // IRQ收发完成中断开启,16位CRC,主发送
    }
    else if (model == 3)    //RX2
    {
        NRF_Write_Reg(FLUSH_TX, 0xff);
        NRF_Write_Reg(FLUSH_RX, 0xff);
        NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);         // IRQ收发完成中断开启,16位CRC,主接收

        Spi_RW(0x50);
        Spi_RW(0x73);
        NRF_Write_Reg(NRF_WRITE_REG + 0x1c, 0x01);
        NRF_Write_Reg(NRF_WRITE_REG + 0x1d, 0x07);
    }
    else if (model == 4)    //TX2
    {
        NRF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);         // IRQ收发完成中断开启,16位CRC,主发送
        NRF_Write_Reg(FLUSH_TX, 0xff);
        NRF_Write_Reg(FLUSH_RX, 0xff);

        Spi_RW(0x50);
        Spi_RW(0x73);
        NRF_Write_Reg(NRF_WRITE_REG + 0x1c, 0x01);
        NRF_Write_Reg(NRF_WRITE_REG + 0x1d, 0x07);
    }
    SPI_CE_H();
}
////////////////////////////////////////////////////////////////////////////////
u8 Nrf_Get_Sta(void)
{
    return NRF_Read_Reg(NRF_READ_REG + NRFRegSTATUS);
}
u8 Nrf_Get_FIFOSta(void)
{
    return NRF_Read_Reg(NRF_READ_REG + FIFO_STATUS);
}
