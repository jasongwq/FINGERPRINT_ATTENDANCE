/***************************************************************/
#include "stm32f0xx.h"
#include "spi.h"
/***************************************************************/

void SPI1_Configuration(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	// Enable SPI and GPIO clocks
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource5|GPIO_PinSource6|GPIO_PinSource7,GPIO_AF_0);//���ӵ�����

	GPIO_SetBits(GPIOA, GPIO_Pin_1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;			//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;	//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;				//ѡ���˴���ʱ�ӵ���̬:ʱ������ ��/��
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;			//���ݲ����ڵ�X��ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;					//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;					//CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);  							//����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���	
	
	SPI_RxFIFOThresholdConfig(SPI1,SPI_RxFIFOThreshold_QF);//���� SPI �� FIFO ������ֵ(����8bits���� SPI_I2S_FLAG_RXNE)
	SPI_Cmd(SPI1, ENABLE); 		//ʹ��SPI����
	GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

u8 SPI1_ReadWriteByte(u8 TxData)                                       //SPI��д���ݺ���
{
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
	SPI_SendData8(SPI1, TxData);
	while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_ReceiveData8(SPI1);
}



