/***************************************************************/
#include "stm32f0xx.h"
#include "iic.h"
/***************************************************************/

 
//初始化IIC
void IIC_Configuration(void)
{
	SCL_OUT()
	SDA_OUT()
}
//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT()     //sda线输出
	IIC_SDA_SET	  	  
	IIC_SCL_SET
	delay_us(4);
 	IIC_SDA_CLR//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_CLR//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT()//sda线输出
	IIC_SCL_CLR
	IIC_SDA_CLR//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_SET
	IIC_SDA_SET//发送I2C总线结束信号
	delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 IIC_Wait_Ack(void)
{
	u8 ucErrTime=0;
	SDA_IN()      //SDA设置为输入  
	IIC_SDA_SET;delay_us(1);	   
	IIC_SCL_SET;delay_us(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_CLR//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_CLR
	SDA_OUT()
	IIC_SDA_CLR
	delay_us(2);
	IIC_SCL_SET
	delay_us(2);
	IIC_SCL_CLR
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_CLR
	SDA_OUT()
	IIC_SDA_SET
	delay_us(2);
	IIC_SCL_SET
	delay_us(2);
	IIC_SCL_CLR
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
	SDA_OUT()    
    IIC_SCL_CLR//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        //IIC_SDA=(txd&0x80)>>7;
		if((txd&0x80)>>7)
			IIC_SDA_SET
		else
			IIC_SDA_CLR
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_SET
		delay_us(2); 
		IIC_SCL_CLR
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_CLR
        delay_us(2);
		IIC_SCL_SET
        receive<<=1;
        if(READ_SDA)receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}






#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))
#define sEE_HW_ADDRESS     0xA0 
/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     300
      
#define sEE_OK                    0
#define sEE_FAIL                  1   
#define sEE_I2C                          I2C1

 #define sEE_PAGESIZE           64

/* Private macro -------------------------------------------------------------*/
__IO uint16_t  sEEAddress = 0;   
__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;
__IO uint16_t  sEEDataNum;

/* Private variables ---------------------------------------------------------*/
uint16_t EEPROM_ADDRESS;



uint32_t sEE_TIMEOUT_UserCallback(void)
{
  /* Block communication and all processes */
  while (1)
  {   
  }
}


void I2C_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	I2C_InitTypeDef  I2C_InitStruct;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	
	// 	/* Configure the I2C clock source. The clock is derived from the HSI */
	//   RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);      
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1 , ENABLE); 

	/*!< GPIO configuration */  
	/*!< Configure sEE_I2C pins: SCL */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_3;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	/*!< Configure sEE_I2C pins: SDA */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOA , &GPIO_InitStruct);

	/* Connect PXx to I2C_SCL*/
	GPIO_PinAFConfig( GPIOA , GPIO_PinSource8, GPIO_AF_4); 
	/* Connect PXx to I2C_SDA*/
	GPIO_PinAFConfig( GPIOA ,GPIO_PinSource9, GPIO_AF_4);
	
	/* I2C configuration */
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	I2C_InitStruct.I2C_DigitalFilter = 0x00;
	I2C_InitStruct.I2C_OwnAddress1 =0x00;
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;	
	I2C_InitStruct.I2C_Timing = 0x00210507;
	/* I2C Peripheral Enable */
	I2C_Cmd(I2C1, ENABLE);
	/* Apply I2C configuration after enabling it */
	I2C_Init(I2C1, &I2C_InitStruct);
	sEEAddress = 0XA0;  
}


uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{  
  uint32_t NumbOfSingle = 0, Count = 0, DataNum = 0, StartCom = 0;
  
  /* Get number of reload cycles */
  Count = (*NumByteToRead) / 255;  
  NumbOfSingle = (*NumByteToRead) % 255;
  
#ifdef sEE_M24C08
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(sEE_I2C, sEEAddress, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Send memory address */
  I2C_SendData(sEE_I2C, (uint8_t)ReadAddr);
  
#elif defined(sEE_M24M01) || defined(sEE_M24C64_32) || defined (sEE_M24LR64) 
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(sEE_I2C, sEEAddress, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Send MSB of memory address */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));
  
  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Send LSB of memory address  */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */ 
  
  /* Wait until TC flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TC) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
  
  /* If number of Reload cycles is not equal to 0 */
  if (Count != 0)
  {
    /* Starting communication */
    StartCom = 1;
    
    /* Wait until all reload cycles are performed */
    while( Count != 0)
    { 
      /* If a read transfer is performed */
      if (StartCom == 0)      
      {
        /* Wait until TCR flag is set */
        sEETimeout = sEE_LONG_TIMEOUT; 
        while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TCR) == RESET)
        {
          if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
        }
      }      
      
      /* if remains one read cycle */
      if ((Count == 1) && (NumbOfSingle == 0))
      {
        /* if starting communication */
        if (StartCom != 0)
        {
          /* Configure slave address, end mode and start condition */
          I2C_TransferHandling(sEE_I2C, sEEAddress, 255, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
        }
        else
        {
          /* Configure slave address, end mode */
          I2C_TransferHandling(sEE_I2C, sEEAddress, 255, I2C_AutoEnd_Mode, I2C_No_StartStop);          
        }
      }
      else 
      {
        /* if starting communication */
        if (StartCom != 0)
        {
          /* Configure slave address, end mode and start condition */
          I2C_TransferHandling(sEE_I2C, sEEAddress, 255, I2C_Reload_Mode, I2C_Generate_Start_Read);
        }
        else
        {
          /* Configure slave address, end mode */
          I2C_TransferHandling(sEE_I2C, sEEAddress, 255, I2C_Reload_Mode, I2C_No_StartStop);          
        } 
      }
      
      /* Update local variable */
      StartCom = 0;      
      DataNum = 0;
      
      /* Wait until all data are received */
      while (DataNum != 255)
      {        
        /* Wait until RXNE flag is set */
        sEETimeout = sEE_LONG_TIMEOUT;
        while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_RXNE) == RESET)
        {
          if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
        }
        
        /* Read data from RXDR */
        pBuffer[DataNum]= I2C_ReceiveData(sEE_I2C);
        
        /* Update number of received data */
        DataNum++;
        (*NumByteToRead)--;
      }      
      /* Update Pointer of received buffer */ 
      pBuffer += DataNum;  
      
      /* update number of reload cycle */
      Count--;
    }
    
    /* If number of single data is not equal to 0 */
    if (NumbOfSingle != 0)
    {            
      /* Wait until TCR flag is set */
      sEETimeout = sEE_LONG_TIMEOUT;   
      while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TCR) == RESET)
      {
        if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
      }
      
      /* Update CR2 : set Nbytes and end mode */
      I2C_TransferHandling(sEE_I2C, sEEAddress, (uint8_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_No_StartStop);
      
      /* Reset local variable */
      DataNum = 0;
      
      /* Wait until all data are received */
      while (DataNum != NumbOfSingle)
      {        
        /* Wait until RXNE flag is set */
        sEETimeout = sEE_LONG_TIMEOUT;
        while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_RXNE) == RESET)
        {
          if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
        }
        
        /* Read data from RXDR */
        pBuffer[DataNum]= I2C_ReceiveData(sEE_I2C);
        
        /* Update number of received data */
        DataNum++;
        (*NumByteToRead)--;
      } 
    }
  }
  else
  {
    /* Update CR2 : set Slave Address , set read request, generate Start and set end mode */
    I2C_TransferHandling(sEE_I2C, sEEAddress, (uint32_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
    
    /* Reset local variable */
    DataNum = 0;
    
    /* Wait until all data are received */
    while (DataNum != NumbOfSingle)
    {
      /* Wait until RXNE flag is set */
      sEETimeout = sEE_LONG_TIMEOUT; 
      while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_RXNE) == RESET)
      {
        if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
      }
      
      /* Read data from RXDR */
      pBuffer[DataNum]= I2C_ReceiveData(sEE_I2C);
      
      /* Update number of received data */
      DataNum++;
      (*NumByteToRead)--;
    }    
  }  
  
  /* Wait until STOPF flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(sEE_I2C, I2C_ICR_STOPCF);
  
  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}

/*******************************************************************************
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  *
  * @note   The number of bytes (combined to write start address) must not 
  *         cross the EEPROM page boundary. This function can only write into
  *         the boundaries of an EEPROM page.
  * @note   This function doesn't check on boundaries condition (in this driver 
  *         the function sEE_WriteBuffer() which calls sEE_WritePage() is 
  *         responsible of checking on Page boundaries).
  * 
  * @param  pBuffer: pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: pointer to the variable holding number of bytes to 
  *         be written into the EEPROM.
  *
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
*******************************************************************************/
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{   
  uint32_t DataNum = 0;
  
#ifdef sEE_M24C08
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(sEE_I2C, sEEAddress, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Send memory address */
  I2C_SendData(sEE_I2C, (uint8_t)WriteAddr);
  
#elif defined(sEE_M24M01) || defined(sEE_M24C64_32) || defined (sEE_M24LR64)
  
  /* Configure slave address, nbytes, reload and generate start */
  I2C_TransferHandling(sEE_I2C, sEEAddress, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Send MSB of memory address */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));  
  
  /* Wait until TXIS flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Send LSB of memory address  */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */
  
  /* Wait until TCR flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TCR) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /* Update CR2 : set Slave Address , set write request, generate Start and set end mode */
  I2C_TransferHandling(sEE_I2C, sEEAddress, (uint8_t)(*NumByteToWrite), I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  while (DataNum != (*NumByteToWrite))
  {      
    /* Wait until TXIS flag is set */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_TXIS) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }  
    
    /* Write data to TXDR */
    I2C_SendData(sEE_I2C, (uint8_t)(pBuffer[DataNum]));
    
    /* Update number of transmitted data */
    DataNum++;   
  }  
  
  /* Wait until STOPF flag is set */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(sEE_I2C, I2C_ICR_STOPCF);
  
  /* If all operations OK, return sEE_OK (0) */
  return sEE_OK;
}

/*******************************************************************************
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  pBuffer: pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr: EEPROM's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the EEPROM.
  * @retval None
*******************************************************************************/
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint16_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;
  
  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
  
  /*!< If WriteAddr is sEE_PAGESIZE aligned  */
  if(Addr == 0) 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage == 0) 
    {
      /* Store the number of data to be written */
      sEEDataNum = NumOfSingle;
      /* Start writing data */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
      sEE_WaitEepromStandbyState();
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else  
    {
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;        
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }
      
      if(NumOfSingle!=0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
  else 
  {
    /*!< If NumByteToWrite < sEE_PAGESIZE */
    if(NumOfPage== 0) 
    {
      /*!< If the number of data to be written is more than the remaining space 
      in the current page: */
      if (NumByteToWrite > count)
      {
        /* Store the number of data to be written */
        sEEDataNum = count;        
        /*!< Write the data conained in same page */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        sEE_WaitEepromStandbyState();      
        
        /* Store the number of data to be written */
        sEEDataNum = (NumByteToWrite - count);          
        /*!< Write the remaining data in the following page */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));
        sEE_WaitEepromStandbyState();        
      }      
      else      
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        sEE_WaitEepromStandbyState();        
      }     
    }
    /*!< If NumByteToWrite > sEE_PAGESIZE */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
      if(count != 0)
      {  
        /* Store the number of data to be written */
        sEEDataNum = count;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        /* Store the number of data to be written */
        sEEDataNum = sEE_PAGESIZE;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
        sEETimeout = sEE_LONG_TIMEOUT;
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        /* Store the number of data to be written */
        sEEDataNum = NumOfSingle;           
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 
        sEE_WaitEepromStandbyState();
      }
    }
  }  
}

/*******************************************************************************
  * @brief  Wait for EEPROM Standby state.
  * 
  * @note  This function allows to wait and check that EEPROM has finished the 
  *        last operation. It is mostly used after Write operation: after receiving
  *        the buffer to be written, the EEPROM may need additional time to actually
  *        perform the write operation. During this time, it doesn't answer to
  *        I2C packets addressed to it. Once the write operation is complete
  *        the EEPROM responds to its address.
  * 
  * @param  None
  *
  * @retval sEE_OK (0) if operation is correctly performed, else return value 
  *         different from sEE_OK (0) or the timeout user callback.
*******************************************************************************/
uint32_t sEE_WaitEepromStandbyState(void)      
{
  __IO uint32_t sEETrials = 0;
  
  /* Keep looping till the slave acknowledge his address or maximum number 
  of trials is reached (this number is defined by sEE_MAX_TRIALS_NUMBER define
  in stm32373c_eval_i2c_ee.h file) */
  
  /* Configure CR2 register : set Slave Address and end mode */
  I2C_TransferHandling(sEE_I2C, sEEAddress, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);  
  
  do
  { 
    /* Initialize sEETimeout */
    sEETimeout = sEE_FLAG_TIMEOUT;
    
    /* Clear NACKF */
    I2C_ClearFlag(sEE_I2C, I2C_ICR_NACKCF | I2C_ICR_STOPCF);
    
    /* Generate start */
    I2C_GenerateSTART(sEE_I2C, ENABLE);
    
    /* Wait until timeout elapsed */
    while (sEETimeout-- != 0); 
    
    /* Check if the maximum allowed numbe of trials has bee reached */
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* If the maximum number of trials has been reached, exit the function */
      return sEE_TIMEOUT_UserCallback();
    }
  }
  while(I2C_GetFlagStatus(sEE_I2C, I2C_ISR_NACKF) != RESET);
  
  /* Clear STOPF */
  I2C_ClearFlag(sEE_I2C, I2C_ICR_STOPCF);
  
  /* Return sEE_OK if device is ready */
  return sEE_OK;
}


