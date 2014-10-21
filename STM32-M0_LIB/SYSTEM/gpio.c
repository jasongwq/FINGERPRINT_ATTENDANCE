/***************************************************************/
#include "gpio.h"
#include "stm32f0xx.h"
/***************************************************************/

void My_GPIO_Init(GPIO_TypeDef* GPIOx,u16 pin,u8 mode)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	if(GPIOx==GPIOA)     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	else if(GPIOx==GPIOB)RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
	else if(GPIOx==GPIOC)RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	else if(GPIOx==GPIOD)RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);
	else if(GPIOx==GPIOF)RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	else {}

	if(mode==1)
	{
		GPIO_InitStructure.GPIO_Pin = pin;		
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 
		GPIO_Init(GPIOx, &GPIO_InitStructure);					  
	}
	else if(mode==0) 
	{
		GPIO_InitStructure.GPIO_Pin = pin;				 
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			 
		GPIO_Init(GPIOx, &GPIO_InitStructure);
	}
}
