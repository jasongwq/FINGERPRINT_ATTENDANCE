/***************************************************************/
#include "stm32f0xx.h"
#include "gpio.h"
#include "delay.h"
/***************************************************************/
#ifndef __IIC_H
#define __IIC_H 
/***************************************************************/


//IO��������
#define SCL_OUT() PA0_OUT
#define SDA_IN()  PA1_In
#define SDA_OUT() PA1_OUT

//IO��������	 
//#define IIC_SCL    PAout(9) //SCL
//#define IIC_SDA    PAout(10) //SDA	 
//#define READ_SDA   PAin(10)  //����SDA 

#define IIC_SCL_SET    Set_A0 //SCL
#define IIC_SCL_CLR    Clr_A0 //SCL

#define IIC_SDA_SET    Set_A1 //SDA	
#define IIC_SDA_CLR    Clr_A1 //SDA	

#define READ_SDA   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)  //����SDA 


void IIC_Configuration(void);       //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�


u8 Single_ReadI2C(u8 REG_Address);
void Single_WriteI2C(u8 REG_Address,u8 REG_data);


#define sEE_M24C08
void I2C_Configuration(void); 
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void     sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);


#endif




