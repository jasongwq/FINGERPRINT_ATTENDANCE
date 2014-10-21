/***************************************************************/
#include "G32_timer_cap.h"
#include "stm32f0xx.h"
/***************************************************************/
//            输出占空比计算公式：TIM_Pulse/(TIM_Period)
/***************************************************************/


#if CAP_TIM1_CH1
u8  TIM1_CAPTURE_STA_CH1 = 0;
u16 TIM1_CAPTURE_VAL_CH1 = 0;
#endif
#if CAP_TIM1_CH2
u8  TIM1_CAPTURE_STA_CH2 = 0;
u16 TIM1_CAPTURE_VAL_CH2 = 0;
#endif
#if CAP_TIM1_CH3
u8  TIM1_CAPTURE_STA_CH3 = 0;
u16 TIM1_CAPTURE_VAL_CH3 = 0;
#endif
#if CAP_TIM1_CH4
u8  TIM1_CAPTURE_STA_CH4 = 0;
u16 TIM1_CAPTURE_VAL_CH4 = 0;
#endif
#if CAP_TIM3_CH1
u8  TIM3_CAPTURE_STA_CH1 = 0;
u16 TIM3_CAPTURE_VAL_CH1 = 0;
#endif
#if CAP_TIM3_CH2
u8  TIM3_CAPTURE_STA_CH2 = 0;
u16 TIM3_CAPTURE_VAL_CH2 = 0;
#endif
#if CAP_TIM3_CH3
u8  TIM3_CAPTURE_STA_CH3 = 0;
u16 TIM3_CAPTURE_VAL_CH3 = 0;
#endif
#if CAP_TIM3_CH4
u8  TIM3_CAPTURE_STA_CH4 = 0;
u16 TIM3_CAPTURE_VAL_CH4 = 0;
#endif
#if CAP_TIM14_CH1
u8  TIM14_CAPTURE_STA_CH1 = 0;
u16 TIM14_CAPTURE_VAL_CH1 = 0;
#endif
#if CAP_TIM14_CH2
u8  TIM14_CAPTURE_STA_CH2 = 0;
u16 TIM14_CAPTURE_VAL_CH2 = 0;
#endif
#if CAP_TIM14_CH3
u8  TIM14_CAPTURE_STA_CH3 = 0;
u16 TIM14_CAPTURE_VAL_CH3 = 0;
#endif
#if CAP_TIM14_CH4
u8  TIM14_CAPTURE_STA_CH4 = 0;
u16 TIM14_CAPTURE_VAL_CH4 = 0;
#endif
/*******************************************************************************
*                   TIM1的函数
*******************************************************************************/
void TIM1_Cap_Init(u16 Period,u16 Prescaler)                          // PA8    PA9    PA10    PA11
{
		TIM_ICInitTypeDef        TIM_ICInitStructure;
		TIM_TimeBaseInitTypeDef  TIM1_TimeBaseStructure;
		GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

#if CAP_TIM1_CH1
        /* GPIOC Configuration: TIM1 Channel 1 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif
#if CAP_TIM1_CH2
        /* GPIOC Configuration: TIM1 Channel 2 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_2);
#endif
#if CAP_TIM1_CH3
        /* GPIOC Configuration: TIM1 Channel 3 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_2);
#endif
#if CAP_TIM1_CH4
        /* GPIOC Configuration: TIM1 Channel 4 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
#endif

    TIM_DeInit(TIM1);                                                //复位定时器8所有寄存器

    /* Time Base configuration */
    TIM1_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;            //预分频数为0,不分频
    TIM1_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计娄方式为顺序计数模式，增大型
    TIM1_TimeBaseStructure.TIM_Period = Period - 1;                  //设置计数器溢出后的重载初值
    TIM1_TimeBaseStructure.TIM_ClockDivision =  0x00;                //配置时钟分隔值
    TIM1_TimeBaseStructure.TIM_RepetitionCounter = 0x0;              //循环计数次数值

    TIM_TimeBaseInit(TIM1, &TIM1_TimeBaseStructure);                 //用以上参数初始化定时器时间基础模块

    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;                     //TIM3中断
		NVIC_InitStructure.NVIC_IRQChannelPriority = 2;      //先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断

    //初始化TIM输入捕获参数
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
#if CAP_TIM1_CH1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM1_CH2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM1_CH3
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_CC3, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断#endif#if CAP_TIM1_CH4
#endif
#if CAP_TIM1_CH4
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(TIM1, &TIM_ICInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_CC4, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
    /* TIM1 counter enable */
    TIM_Cmd(TIM1, ENABLE);                                           //使能定时器8
}
#define TIM_CAPTURE_STA_CH(x,y) TIM##x##_CAPTURE_STA_CH##y
#define TIM_CAPTURE_VAL_CH(x,y) TIM##x##_CAPTURE_VAL_CH##y
#define DEFTIM_CAP_CH(timx,ch) \
    if ((TIM_CAPTURE_STA_CH(timx,ch) & 0X80) == 0)\
    {\
        if (TIM_GetITStatus(TIMx, TIM_IT_CC##ch) != RESET)\
        {\
            if(TIM_CAPTURE_STA_CH(timx,ch) & 0X40)\
            {\
                TIM_CAPTURE_STA_CH(timx,ch) |= 0X80;\
                TIM_CAPTURE_VAL_CH(timx,ch) = TIM_GetCapture##ch(TIMx);\
                TIM_OC##ch##PolarityConfig(TIMx, TIM_ICPolarity_Rising);\
            }else{\
                TIM_CAPTURE_STA_CH(timx,ch) = 0;\
                TIM_CAPTURE_VAL_CH(timx,ch) = 0;\
                TIM_SetCounter(TIMx, 0);\
                TIM_CAPTURE_STA_CH(timx,ch) |= 0X40;\
                TIM_OC##ch##PolarityConfig(TIMx, TIM_ICPolarity_Falling);\
            }\
        }\
    }
//定时器中断服务程序
void TIM1_Cap_IRQ(void)
{
#define TIMx TIM1
#if CAP_TIM1_CH1
    DEFTIM_CAP_CH(1, 1)
#endif
#if CAP_TIM1_CH2
    DEFTIM_CAP_CH(1, 2)
#endif
#if CAP_TIM1_CH3
    DEFTIM_CAP_CH(1, 3)
#endif
#if CAP_TIM1_CH4
    DEFTIM_CAP_CH(1, 4)
#endif
    TIM_ClearITPendingBit(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //清除中断标志位
#undef TIMx
}

//*******************************************************************************
//*                   TIM3的函数
//*******************************************************************************/
void TIM3_Cap_Init(u16 Period,u16 Prescaler)                          // PA6    PA7    PB0    PB1
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM3_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
#if CAP_TIM3_CH1
        /* GPIOA Configuration: TIM3 Channel 1 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
#endif
#if CAP_TIM3_CH2
        /* GPIOA Configuration: TIM3 Channel 2 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
#endif
#if CAP_TIM3_CH3
        /* GPIOB Configuration: TIM3 Channel 3 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);

#endif
#if CAP_TIM3_CH4
        /* GPIOB Configuration: TIM3 Channel 4 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
#endif
    TIM_DeInit(TIM3);                                                //复位定时器8所有寄存器
    /* Time Base configuration */
    TIM3_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;            //预分频数为0,不分频
    TIM3_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计娄方式为顺序计数模式，增大型
    TIM3_TimeBaseStructure.TIM_Period = Period - 1;                  //设置计数器溢出后的重载初值
    TIM3_TimeBaseStructure.TIM_ClockDivision =  0x00;                //配置时钟分隔值
    TIM3_TimeBaseStructure.TIM_RepetitionCounter = 0x0;              //循环计数次数值
    TIM_TimeBaseInit(TIM3, &TIM3_TimeBaseStructure);                 //用以上参数初始化定时器时间基础模块
    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;                     //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;      //先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
    //初始化TIM输入捕获参数
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
#if CAP_TIM3_CH1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM3_CH2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM3_CH3
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM3_CH4
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
    TIM_Cmd(TIM3, ENABLE);                                           //使能定时器8
}

void TIM3_Cap_IRQ(void)
{
#define TIMx TIM3
#if CAP_TIM3_CH1
    DEFTIM_CAP_CH(3, 1)
#endif
#if CAP_TIM3_CH2
    DEFTIM_CAP_CH(3, 2)
#endif
#if CAP_TIM3_CH3
    DEFTIM_CAP_CH(3, 3)
#endif
#if CAP_TIM3_CH4
    DEFTIM_CAP_CH(3, 4)
#endif
    TIM_ClearITPendingBit(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //清除中断标志位
#undef TIMx
}
//*******************************************************************************
//*                   TIM14的函数
//*******************************************************************************/
void TIM14_Cap_Init(u16 Period,u16 Prescaler)                          // PA6    PA7    PB0    PB1
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM14_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
#if CAP_TIM14_CH1
        /* GPIOA Configuration: TIM14 Channel 1 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);
#endif
#if CAP_TIM14_CH2
        /* GPIOA Configuration: TIM14 Channel 2 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOA, &GPIO_InitStructure);
        GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
#endif
#if CAP_TIM14_CH3
        /* GPIOB Configuration: TIM14 Channel 3 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);

#endif
#if CAP_TIM14_CH4
        /* GPIOB Configuration: TIM14 Channel 4 Output */
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
				GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
				GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_1);
#endif
    TIM_DeInit(TIM14);                                                //复位定时器8所有寄存器
    /* Time Base configuration */
    TIM14_TimeBaseStructure.TIM_Prescaler = Prescaler - 1;            //预分频数为0,不分频
    TIM14_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计娄方式为顺序计数模式，增大型
    TIM14_TimeBaseStructure.TIM_Period = Period - 1;                  //设置计数器溢出后的重载初值
    TIM14_TimeBaseStructure.TIM_ClockDivision =  0x00;                //配置时钟分隔值
    TIM14_TimeBaseStructure.TIM_RepetitionCounter = 0x0;              //循环计数次数值
    TIM_TimeBaseInit(TIM14, &TIM14_TimeBaseStructure);                 //用以上参数初始化定时器时间基础模块
    //中断分组初始化
    NVIC_InitStructure.NVIC_IRQChannel = TIM14_IRQn;                     //TIM14中断
    NVIC_InitStructure.NVIC_IRQChannelPriority = 2;      //先占优先级2级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                     //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
    TIM_ITConfig(TIM14, TIM_IT_Update, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
    //初始化TIM输入捕获参数
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;    //上升沿捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;   //配置输入分频,不分频
    TIM_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
#if CAP_TIM14_CH1
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
    TIM_ICInit(TIM14, &TIM_ICInitStructure);
    TIM_ITConfig(TIM14, TIM_IT_CC1, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM14_CH2
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInit(TIM14, &TIM_ICInitStructure);
    TIM_ITConfig(TIM14, TIM_IT_CC2, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM14_CH3
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
    TIM_ICInit(TIM14, &TIM_ICInitStructure);
    TIM_ITConfig(TIM14, TIM_IT_CC3, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
#if CAP_TIM14_CH4
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInit(TIM14, &TIM_ICInitStructure);
    TIM_ITConfig(TIM14, TIM_IT_CC4, ENABLE);     //允许更新中断 ,允许CC1IE捕获中断
#endif
    TIM_Cmd(TIM14, ENABLE);                                           //使能定时器8
}
void TIM14_Cap_IRQ(void)
{
#define TIMx TIM14
#if CAP_TIM14_CH1
    DEFTIM_CAP_CH(14, 1)
#endif
#if CAP_TIM14_CH2
    DEFTIM_CAP_CH(14, 2)
#endif
#if CAP_TIM14_CH3
    DEFTIM_CAP_CH(14, 3)
#endif
#if CAP_TIM14_CH4
    DEFTIM_CAP_CH(14, 4)
#endif
    TIM_ClearITPendingBit(TIMx, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4 | TIM_IT_Update); //清除中断标志位
#undef TIMx
}




