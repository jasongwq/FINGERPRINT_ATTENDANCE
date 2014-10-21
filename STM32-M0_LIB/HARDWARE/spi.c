/***************************************************************/
#include "stm32f0xx.h"
#include "./spi.h"
/***************************************************************/
void SPI0_Init(void)
{
    SPI0_SCK_Init
    SPI0_MISO_Init
    SPI0_MOSI_Init
    SPI0_ReadWriteByte(0xff);                                            //????
}
u8 SPI0_ReadWriteByte(u8 TxData)                                        //SPI??????
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (TxData & 0x80)
        {
            Set_SPI0_MOSI
        }
        else
        {
            Clr_SPI0_MOSI
        }

        TxData = (TxData << 1);                    // shift next bit into MSB..
        Set_SPI0_SCK                             // Set SCK high..
        TxData |= MISO_SPI0;                     // capture current MISO bit
        Clr_SPI0_SCK                             // ..then set SCK low again
    }
    return (TxData);                             // return read UINT8
}
void SPI1_Init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Enable SPI and GPIO clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5|GPIO_PinSource6|GPIO_PinSource7,GPIO_AF_0);//连接到外设

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				//选择了串行时钟的稳态:时钟悬空 高/低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			//数据捕获于第X个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;					//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;					//CRC值计算的多项式
	SPI_Init(SPI1, &SPI_InitStructure);  							//根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器	
	
	SPI_RxFIFOThresholdConfig(SPI1,SPI_RxFIFOThreshold_QF);//配置 SPI 的 FIFO 接收阈值(接收8bits就置 SPI_I2S_FLAG_RXNE)
	SPI_Cmd(SPI1, ENABLE); 		//使能SPI外设
}

u8 SPI1_ReadWriteByte(u8 TxData)                                       //SPI读写数据函数
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_SendData8(SPI1, TxData);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_ReceiveData8(SPI1);
}



