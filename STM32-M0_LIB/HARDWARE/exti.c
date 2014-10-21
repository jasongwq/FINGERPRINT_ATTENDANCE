/***************************************************************/
#include "stm32f0xx.h"
#include "exti.h"
/***************************************************************/



void EXTI_Configuration(GPIO_TypeDef* GPIO, uint16_t GPIO_Pin, u8 Trigger,u8 PP)
{
	uint8_t  GPIO_PIN_SOURCE,GPIO_PORT_SOURCE;
	uint32_t EXTI_Line,EXTIx;
	EXTI_InitTypeDef EXTI_InitStructure;	
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
	if(GPIO==GPIOA){GPIO_PORT_SOURCE=GPIO_PortSourceGPIOA;RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);}
	else if(GPIO==GPIOB){GPIO_PORT_SOURCE=GPIO_PortSourceGPIOB;RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);}
	else if(GPIO==GPIOC){GPIO_PORT_SOURCE=GPIO_PortSourceGPIOC;RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);}
	else if(GPIO==GPIOD){GPIO_PORT_SOURCE=GPIO_PortSourceGPIOD;RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);}
	else if(GPIO==GPIOF){GPIO_PORT_SOURCE=GPIO_PortSourceGPIOF;RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);}
	else {}
		
	if(GPIO_Pin==GPIO_Pin_0){EXTI_Line=EXTI_Line0;GPIO_PIN_SOURCE=GPIO_PinSource0;EXTIx=EXTI0_1_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_1 ){EXTI_Line=EXTI_Line1; GPIO_PIN_SOURCE=GPIO_PinSource1;EXTIx=EXTI0_1_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_2 ){EXTI_Line=EXTI_Line2; GPIO_PIN_SOURCE=GPIO_PinSource2;EXTIx=EXTI2_3_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_3 ){EXTI_Line=EXTI_Line3; GPIO_PIN_SOURCE=GPIO_PinSource3;EXTIx=EXTI2_3_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_4 ){EXTI_Line=EXTI_Line4; GPIO_PIN_SOURCE=GPIO_PinSource4;EXTIx=EXTI4_15_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_5 ){EXTI_Line=EXTI_Line5; GPIO_PIN_SOURCE=GPIO_PinSource5;EXTIx=EXTI4_15_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_6 ){EXTI_Line=EXTI_Line6; GPIO_PIN_SOURCE=GPIO_PinSource6;EXTIx=EXTI4_15_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_7 ){EXTI_Line=EXTI_Line7; GPIO_PIN_SOURCE=GPIO_PinSource7;EXTIx=EXTI4_15_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_8 ){EXTI_Line=EXTI_Line8; GPIO_PIN_SOURCE=GPIO_PinSource8;EXTIx=EXTI4_15_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_9 ){EXTI_Line=EXTI_Line9; GPIO_PIN_SOURCE=GPIO_PinSource9;EXTIx=EXTI4_15_IRQn; }
	else if(GPIO_Pin==GPIO_Pin_10){EXTI_Line=EXTI_Line10;GPIO_PIN_SOURCE=GPIO_PinSource10;EXTIx=EXTI4_15_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_11){EXTI_Line=EXTI_Line11;GPIO_PIN_SOURCE=GPIO_PinSource11;EXTIx=EXTI4_15_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_12){EXTI_Line=EXTI_Line12;GPIO_PIN_SOURCE=GPIO_PinSource12;EXTIx=EXTI4_15_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_13){EXTI_Line=EXTI_Line13;GPIO_PIN_SOURCE=GPIO_PinSource13;EXTIx=EXTI4_15_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_14){EXTI_Line=EXTI_Line14;GPIO_PIN_SOURCE=GPIO_PinSource14;EXTIx=EXTI4_15_IRQn;}
	else if(GPIO_Pin==GPIO_Pin_15){EXTI_Line=EXTI_Line15;GPIO_PIN_SOURCE=GPIO_PinSource15;EXTIx=EXTI4_15_IRQn;}
	else {}

	/* Select interrupt group*/
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);				 //Configure the NVIC Preemption Priority Bits
	NVIC_InitStructure.NVIC_IRQChannel = EXTIx;				 //选择外部中断y号线中断通道
	NVIC_InitStructure.NVIC_IRQChannelPriority =PP;		 //主优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能中断通道
	NVIC_Init(&NVIC_InitStructure);								 //用以上参数初始化y号线中断通道
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin;				  //选择IO端口的第y位
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;		  //端口模式为带上拉方式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_2;
	if(Trigger==0)GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;    
	else if(Trigger==1)GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;    
	GPIO_Init(GPIO, &GPIO_InitStructure);					  //用以上几个参数初始化Px口
		
	SYSCFG_EXTILineConfig(GPIO_PORT_SOURCE, GPIO_PIN_SOURCE);     //触屏输出中断
	EXTI_InitStructure.EXTI_Line = EXTI_Line;					  //选择外部中断x号线
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			  //外部线路模式为中断模式
			
	if(Trigger==0)EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
	else if(Trigger==1)EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;

		
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;						  //打开中断线路
	EXTI_Init(&EXTI_InitStructure);								  //用以上参数初始化Pxy所占中断线
	
}

