/***************************************************************/
#include "pwm.h"
#include "stm32f0xx.h"
/***************************************************************/
//			  ���ռ�ձȼ��㹫ʽ��TIM_Pulse/(TIM_Period)
/***************************************************************/

static GPIO_InitTypeDef GPIO_InitStructure;


static TIM_TimeBaseInitTypeDef  TIM1_TimeBaseStructure,TIM3_TimeBaseStructure;
static TIM_OCInitTypeDef  		TIM1_OCInitStructure,TIM3_OCInitStructure;


/*******************************************************************************
*					TIM1�ĺ���
*******************************************************************************/
void TIM1_PWM_Configuration(u16 Prescaler,u16 Period,u8 OC1 ,u8 OC2 ,u8 OC3 ,u8 OC4)							   // PA8    PA9    PA10    PA11
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

    if(OC1==1)
    {
	    /* GPIOC Configuration: TIM1 Channel 1 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;			 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	      
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 	
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }
	if(OC2==1)
    {
	    /* GPIOC Configuration: TIM1 Channel 2 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	      
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
        GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		
        GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_2);
    }
	if(OC3==1)
    {
	    /* GPIOC Configuration: TIM1 Channel 3 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;			  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	      
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
        GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		
        GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_2);
    }
	if(OC4==1)
    {
	    /* GPIOC Configuration: TIM1 Channel 4 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;			 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	     
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
        GPIO_Init(GPIOA, &GPIO_InitStructure);
    }


    TIM_DeInit(TIM1);												 //��λ��ʱ��8���мĴ���

    /* Time Base configuration */
    TIM1_TimeBaseStructure.TIM_Prescaler = Prescaler-1;				 //Ԥ��Ƶ��Ϊ0,����Ƶ
    TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //��¦��ʽΪ˳�����ģʽ��������
    TIM1_TimeBaseStructure.TIM_Period = Period-1;					 //���ü��������������س�ֵ
    TIM1_TimeBaseStructure.TIM_ClockDivision =  0x00;				 //����ʱ�ӷָ�ֵ
    TIM1_TimeBaseStructure.TIM_RepetitionCounter = 0x0;			     //ѭ����������ֵ

    TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseStructure);				     //�����ϲ�����ʼ����ʱ��ʱ�����ģ��
  
    /* Channel 1 Configuration in PWM mode */
    TIM1_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			     //�����ʽΪPWMģʽ1
 
    TIM1_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //ʹ������Ƚ�״̬
    TIM1_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //ʹ�ܶ�ʱ���������               
    TIM1_OCInitStructure.TIM_Pulse = 20000; 						 //��������
    TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   	 //����Ƚϼ���Ϊ��
    TIM1_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;	     //�򿪿���״̬ѡ��ر�
  
    /* ��ʼ��TM1ͨ��1*/
    if(OC1==1)TIM_OC1Init(TIM1,&TIM1_OCInitStructure); 		         //�����ϲ�����ʼ��TIM8��ͨ��1
	if(OC2==1)TIM_OC2Init(TIM1,&TIM1_OCInitStructure); 		         //�����ϲ�����ʼ��TIM8��ͨ��2
	if(OC3==1)TIM_OC3Init(TIM1,&TIM1_OCInitStructure); 		         //�����ϲ�����ʼ��TIM8��ͨ��3
	if(OC4==1)TIM_OC4Init(TIM1,&TIM1_OCInitStructure); 		         //�����ϲ�����ʼ��TIM8��ͨ��4
 
    /* TIM1 counter enable */
    TIM_Cmd(TIM1,ENABLE);							   				 //ʹ�ܶ�ʱ��8

    /* Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1,ENABLE);				   				 //ʹ�ܶ�ʱ��8��PWM���	 Ƶ��

}  

void TIM1_Change_Period(u16 NoteSet)
{
     TIM1_TimeBaseStructure.TIM_Period = NoteSet ;
	 TIM_TimeBaseInit(TIM1,&TIM1_TimeBaseStructure);
}
/*******************************���Ƹ���ͨ��������*****************************************************/
void Change_TIM1_OC1_Pulse(u16 Pulse)
{
     TIM1_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC1Init(TIM1,&TIM1_OCInitStructure);
}
void Change_TIM1_OC2_Pulse(u16 Pulse)
{
     TIM1_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC2Init(TIM1,&TIM1_OCInitStructure);
}
void Change_TIM1_OC3_Pulse(u16 Pulse)
{
     TIM1_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC3Init(TIM1,&TIM1_OCInitStructure);
}
void Change_TIM1_OC4_Pulse(u16 Pulse)
{
     TIM1_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC4Init(TIM1,&TIM1_OCInitStructure);
}
/*********************************��ͨ�����Կ���*****************************************************/
void Change_TIM1_OC1_Polarity(u8 i)
{
     if(i==1)TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC1Init(TIM1,&TIM1_OCInitStructure);
}
void Change_TIM1_OC2_Polarity(u8 i)
{
     if(i==1)TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC2Init(TIM1,&TIM1_OCInitStructure);
}
void Change_TIM1_OC3_Polarity(u8 i)
{
     if(i==1)TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC3Init(TIM1,&TIM1_OCInitStructure);
}
void Change_TIM1_OC4_Polarity(u8 i)
{
     if(i==1)TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM1_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC4Init(TIM1,&TIM1_OCInitStructure);
}


/*******************************************************************************
*					TIM3�ĺ���
*******************************************************************************/
void TIM3_PWM_Configuration(u16 Prescaler,u16 Period,u8 OC1 ,u8 OC2 ,u8 OC3 ,u8 OC4)							   // PA6    PA7    PB0    PB1
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

    if(OC1==1)
    {
	    /* GPIOA Configuration: TIM3 Channel 1 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	    
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
        GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		
        GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_1);
    }
	if(OC2==1)
    {
	    /* GPIOA Configuration: TIM3 Channel 2 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	     
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  
        GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		
        GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_1);
    }
	if(OC3==1)
    {
	    /* GPIOB Configuration: TIM3 Channel 3 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;			  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	     
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }
	if(OC4==1)
    {
	    /* GPIOB Configuration: TIM3 Channel 4 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;			  
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;	     
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 
        GPIO_Init(GPIOB, &GPIO_InitStructure);
    }


    TIM_DeInit(TIM3);												 //��λ��ʱ��8���мĴ���

    /* Time Base configuration */
    TIM3_TimeBaseStructure.TIM_Prescaler = Prescaler-1;				 //Ԥ��Ƶ��Ϊ0,����Ƶ
    TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 //��¦��ʽΪ˳�����ģʽ��������
    TIM3_TimeBaseStructure.TIM_Period =Period-1;					 //���ü��������������س�ֵ
    TIM3_TimeBaseStructure.TIM_ClockDivision =  0x00;				 //����ʱ�ӷָ�ֵ
    TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0x0;			     //ѭ����������ֵ

    TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);				     //�����ϲ�����ʼ����ʱ��ʱ�����ģ��
  
    /* Channel 1 Configuration in PWM mode */
    TIM3_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 			     //�����ʽΪPWMģʽ1
 
    TIM3_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //ʹ������Ƚ�״̬
    TIM3_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //ʹ�ܶ�ʱ���������               
    TIM3_OCInitStructure.TIM_Pulse = 20000; 						 //��������
    TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   	 //����Ƚϼ���Ϊ��
    TIM3_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;	     //�򿪿���״̬ѡ��ر�
  
    /* ��ʼ��TM3ͨ��1*/
    if(OC1==1)TIM_OC1Init(TIM3,&TIM3_OCInitStructure); 		         //�����ϲ�����ʼ��TIM3��ͨ��1
	if(OC2==1)TIM_OC2Init(TIM3,&TIM3_OCInitStructure); 		         //�����ϲ�����ʼ��TIM3��ͨ��2
	if(OC3==1)TIM_OC3Init(TIM3,&TIM3_OCInitStructure); 		         //�����ϲ�����ʼ��TIM3��ͨ��3
	if(OC4==1)TIM_OC4Init(TIM3,&TIM3_OCInitStructure); 		         //�����ϲ�����ʼ��TIM3��ͨ��4
 
    /* TIM3 counter enable */
    TIM_Cmd(TIM3,ENABLE);							     			 //ʹ�ܶ�ʱ��8

    /* Main Output Enable */
    TIM_CtrlPWMOutputs(TIM3,ENABLE);				   				 //ʹ�ܶ�ʱ��8��PWM���	 Ƶ��

}  

void TIM3_Change_Period(u16 NoteSet)
{
     TIM3_TimeBaseStructure.TIM_Period = NoteSet ;
	 TIM_TimeBaseInit(TIM3,&TIM3_TimeBaseStructure);
}
/*******************************���Ƹ���ͨ��������*****************************************************/
void Change_TIM3_OC1_Pulse(u16 Pulse)
{
     TIM3_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC1Init(TIM3,&TIM3_OCInitStructure);
}
void Change_TIM3_OC2_Pulse(u16 Pulse)
{
     TIM3_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC2Init(TIM3,&TIM3_OCInitStructure);
}
void Change_TIM3_OC3_Pulse(u16 Pulse)
{
     TIM3_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC3Init(TIM3,&TIM3_OCInitStructure);
}
void Change_TIM3_OC4_Pulse(u16 Pulse)
{
     TIM3_OCInitStructure.TIM_Pulse = Pulse;
	 TIM_OC4Init(TIM3,&TIM3_OCInitStructure);
}
/*********************************��ͨ�����Կ���*****************************************************/
void Change_TIM3_OC1_Polarity(u8 i)
{
     if(i==1)TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC1Init(TIM3,&TIM3_OCInitStructure);
}
void Change_TIM3_OC2_Polarity(u8 i)
{
     if(i==1)TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC2Init(TIM3,&TIM3_OCInitStructure);
}
void Change_TIM3_OC3_Polarity(u8 i)
{
     if(i==1)TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC3Init(TIM3,&TIM3_OCInitStructure);
}
void Change_TIM3_OC4_Polarity(u8 i)
{
     if(i==1)TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 else 	 TIM3_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	 TIM_OC4Init(TIM3,&TIM3_OCInitStructure);
}

