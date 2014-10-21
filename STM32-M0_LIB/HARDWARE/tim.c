/***************************************************************/
#include "stm32f0xx.h"
#include "tim.h"
/***************************************************************/

void TIM_Configuration(TIM_TypeDef* TIMER,u16 Period,u16 Prescaler,u8 PP)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	if(TIMER==TIM1)      RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	else if(TIMER==TIM2 )RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	else if(TIMER==TIM3 )RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	
	else if(TIMER==TIM6 )RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	else if(TIMER==TIM14)RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
	else if(TIMER==TIM15)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM15, ENABLE);
	else if(TIMER==TIM16)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM16, ENABLE);
	else if(TIMER==TIM17)RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
	

	TIM_TimeBaseStructure.TIM_Period = Period-1; 	              //��������Զ�װ�صļ���ֵ�����ڼ����Ǵ�0��ʼ�ģ�����10000�κ�Ϊ9999
	TIM_TimeBaseStructure.TIM_Prescaler =Prescaler-1;             // �������Ԥ��Ƶϵ����������Ϊ0ʱ��ʾ����Ƶ����Ҫ��1  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 	              // ʹ�õĲ���Ƶ��֮��ķ�Ƶ����
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   //���ϼ���
	TIM_TimeBaseInit(TIMER, &TIM_TimeBaseStructure);              //��ʼ����ʱ��

	TIM_ClearITPendingBit(TIMER, TIM_IT_Update);             	  //Clear TIM5 update pending flag[���TIM5����жϱ�־] 
	TIM_ITConfig(TIMER, TIM_IT_Update, ENABLE);				      //������ж�
	TIM_Cmd(TIMER, ENABLE);                                       //������ʹ�ܣ���ʼ����
	
	if(TIMER==TIM2){NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;}
	else if(TIMER==TIM3){NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;}
//	else if(TIMER==TIM4){NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;}
//	else if(TIMER==TIM5){NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;}
	
	NVIC_InitStructure.NVIC_IRQChannelPriority  =PP;	 //�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //ʹ���ж�ͨ��
	NVIC_Init(&NVIC_InitStructure);								 //�����ϲ�����ʼ��10-15�����ж�ͨ��
}

