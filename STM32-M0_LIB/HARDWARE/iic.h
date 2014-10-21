/***************************************************************/
#include "stm32f0xx.h"
#include "gpio.h"
#include "delay.h"
/***************************************************************/
#ifndef __IIC_H
#define __IIC_H 
/***************************************************************/


//IO方向设置
#define SCL_OUT() PA0_OUT
#define SDA_IN()  PA1_In
#define SDA_OUT() PA1_OUT

//IO操作函数	 
//#define IIC_SCL    PAout(9) //SCL
//#define IIC_SDA    PAout(10) //SDA	 
//#define READ_SDA   PAin(10)  //输入SDA 

#define IIC_SCL_SET    Set_A0 //SCL
#define IIC_SCL_CLR    Clr_A0 //SCL

#define IIC_SDA_SET    Set_A1 //SDA	
#define IIC_SDA_CLR    Clr_A1 //SDA	

#define READ_SDA   GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1)  //输入SDA 


void IIC_Configuration(void);       //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(u8 txd);			//IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
u8 IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号


u8 Single_ReadI2C(u8 REG_Address);
void Single_WriteI2C(u8 REG_Address,u8 REG_data);


#define sEE_M24C08
void I2C_Configuration(void); 
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void     sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);


#endif




