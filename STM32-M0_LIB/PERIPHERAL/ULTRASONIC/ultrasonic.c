#include "ultrasonic.h"
#include "delay.h"


extern void TIM5_Ultrasonic_Cap_Init(u16 arr, u16 psc);
extern void TIM4_Ultrasonic_Cap_Init(u16 arr, u16 psc);

static void TIM_GPIO_Init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_ULTRASONIC_ECHO, ENABLE);  //ʹ��GPIOAʱ��
	
	  GPIO_InitStructure.GPIO_Pin  = ULTRASONIC_ECHO_PIN;  //PA0 ���֮ǰ����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;				 //PA0 ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(ULTRASONIC_ECHO_GPIO, &GPIO_InitStructure);
    GPIO_ResetBits(ULTRASONIC_ECHO_GPIO, ULTRASONIC_ECHO_PIN);                       //PA0 ����
}
void Ultrasonic_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_ULTRASONIC_TRIG, ENABLE); //ʹ��Px�˿�ʱ��
	
    GPIO_InitStructure.GPIO_Pin = ULTRASONIC_TRIG_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(ULTRASONIC_TRIG_GPIO, &GPIO_InitStructure);
		TIM4_Ultrasonic_Cap_Init(0XFFFF, 72 - 1);
}

void TIM5_Ultrasonic_Cap_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM5_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);   //ʹ��TIM5ʱ��

		TIM_GPIO_Init();

    //��ʼ����ʱ��5 TIM5
    TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM5���벶�����
    TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //�����ز���
    TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
    TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //���������Ƶ,����Ƶ
    TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM5, &TIM5_ICInitStructure);

    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  					//TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			//�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE); 	//��������ж� ,����CC1IE�����ж�

    TIM_Cmd(TIM5, ENABLE ); //ʹ�ܶ�ʱ��5
}
void TIM4_Ultrasonic_Cap_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef	TIM_TimeBaseStructure;
    NVIC_InitTypeDef 				NVIC_InitStructure;
    TIM_ICInitTypeDef  			TIM4_ICInitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);   //ʹ��TIM5ʱ��

		TIM_GPIO_Init();

    //��ʼ����ʱ��5 TIM5
    TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc;  //Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    //��ʼ��TIM5���벶�����
    TIM4_ICInitStructure.TIM_Channel 			= TIM_Channel_3; //CC1S=01     ѡ������� IC1ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;    	//�����ز���
    TIM4_ICInitStructure.TIM_ICSelection 	= TIM_ICSelection_DirectTI; 	//ӳ�䵽TI1��
    TIM4_ICInitStructure.TIM_ICPrescaler 	= TIM_ICPSC_DIV1;   					//���������Ƶ,����Ƶ
    TIM4_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
    TIM_ICInit(TIM4, &TIM4_ICInitStructure);

    //�жϷ����ʼ��
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  					//TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //��ռ���ȼ�2��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  			//�����ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 					//IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_CC3, ENABLE); 	//��������ж� ,����CC1IE�����ж�

    TIM_Cmd(TIM4, ENABLE ); //ʹ�ܶ�ʱ��5
}

u8  TIM4CH1_CAPTURE_STA = 0;//���벶��״̬
u16 TIM4CH1_CAPTURE_VAL;    //���벶��ֵ
//��ʱ��4�жϷ������
//void TIM4_IRQHandler(void)
//{
//    if ((TIM4CH1_CAPTURE_STA & 0X80) == 0) //��δ�ɹ�����
//    {
//        if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
//        {
//            if (TIM4CH1_CAPTURE_STA & 0X40) //�Ѿ����񵽸ߵ�ƽ��
//            {
//                if ((TIM4CH1_CAPTURE_STA & 0X3F) == 0X3F) //�ߵ�ƽ̫����
//                {
//                    TIM4CH1_CAPTURE_STA |= 0X80; //��ǳɹ�������һ��
//                    TIM4CH1_CAPTURE_VAL = 0XFFFF;
//                }
//                else TIM4CH1_CAPTURE_STA++;
//            }
//        }
//        if (TIM_GetITStatus(TIM4, TIM_IT_CC3) != RESET)//����1���������¼�
//        {
//            if (TIM4CH1_CAPTURE_STA & 0X40)     //����һ���½���
//            {
//                TIM4CH1_CAPTURE_STA |= 0X80;    //��ǳɹ�����һ��������
//                TIM4CH1_CAPTURE_VAL = TIM_GetCapture3(TIM4);
//				//ע�⣡����
//                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
//            }
//            else                                //��δ��ʼ,��һ�β���������
//            {
//                TIM4CH1_CAPTURE_STA = 0;        //���
//                TIM4CH1_CAPTURE_VAL = 0;
//                TIM_SetCounter(TIM4, 0);
//                TIM4CH1_CAPTURE_STA |= 0X40;    //��ǲ�����������
//				//ע�⣡����
//                TIM_OC3PolarityConfig(TIM4, TIM_ICPolarity_Falling);    //CC1P=1 ����Ϊ�½��ز���
//            }
//        }
//    }
//    TIM_ClearITPendingBit(TIM4, TIM_IT_CC3 | TIM_IT_Update); //����жϱ�־λ
//}

u32 ultrasonic (void)
{
  u32 temp = 0;
	TRIG_H;
	TIM4CH1_CAPTURE_STA = 0; //������һ�β���
	delay_us(10);	
	TRIG_L;
	while(1)
	{
		if (TIM4CH1_CAPTURE_STA & 0X80) //�ɹ�������һ��������
		{
			temp = TIM4CH1_CAPTURE_STA & 0X3F;
			temp *= 65536;//���ʱ���ܺ�
			temp += TIM4CH1_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
			//USART_OUT(USART1, "TIME:%dms %dus %dM%dMM\r\n", temp/1000,temp%1000,(temp*340/1000/2)/1000,(temp*340/1000/2)%1000); //��ӡ�ܵĸߵ�ƽʱ��	
			break;
		 }
	}
	return (temp*340/1000/2);
}

