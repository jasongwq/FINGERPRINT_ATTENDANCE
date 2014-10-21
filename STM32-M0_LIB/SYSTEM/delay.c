/***************************************************************/
#include "stm32f0xx.h"
#include "delay.h"
/***************************************************************/

static u8  fac_us=0;                                               // us The multiplier delay times 
static u16 fac_ms=0;                                               // ms The multiplier delay times 
/***************************************************************/
void delay_Configuration(u8 SYSCLK)
{
	SysTick->CTRL&=0xfffffffb;                                     // Select the internal clock HCLK/8		
	fac_us=SYSCLK/8;		    						   
	fac_ms=(u16)fac_us*1000;					 
}	
/***************************************************************/
void delay_ms(u16 nms)
{
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;                                 // Loading time
	SysTick->VAL =0x00;                                            // Empty the counter
	SysTick->CTRL=0x01 ;                                           // Start from bottom
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));                             // Wait time arrive
	SysTick->CTRL=0x00;                                            // Close the counter
	SysTick->VAL =0X00;                                            // Empty the counter	    
}		
/***************************************************************/
void delay_us(u32 Nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=Nus*fac_us;   		 
	SysTick->VAL=0x00;       
	SysTick->CTRL=0x01 ;       
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));
	SysTick->CTRL=0x00;       
	SysTick->VAL =0X00;       
}
/***************************************************************/
void delay_s(u32 Ns)
{		
	switch (Ns)
	{
		case 10: delay_ms(1000);
		case 9: delay_ms(1000);
		case 8: delay_ms(1000);
		case 7: delay_ms(1000);
		case 6: delay_ms(1000);
		case 5: delay_ms(1000);
		case 4: delay_ms(1000);
		case 3: delay_ms(1000);
		case 2: delay_ms(1000);
		case 1: delay_ms(1000);break;
	}
}


/***************************************************************/
void RCC_Configuration(void)
{
  RCC_DeInit();									    		//时钟控制寄存器全部恢复默认值
	RCC_HSEConfig(RCC_HSE_OFF);
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET){}
	 /* HCLK = SYSCLK */
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);
	RCC_PLLCmd(ENABLE);
		
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET){}
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);   
	while(RCC_GetSYSCLKSource() != 0x08){}
}
/***************************************************************/
//void SoftReset(void) 
//{ 
//	__set_FAULTMASK(1); 
//	NVIC_SystemReset();
//}

/***************************************************************/
