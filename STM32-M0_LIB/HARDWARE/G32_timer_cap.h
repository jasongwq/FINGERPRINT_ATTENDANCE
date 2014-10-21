#ifndef __TIMER_CAP_H
#define __TIMER_CAP_H
#include "stm32f0xx.h"

#define CAP_TIM3_CH1  1
#define CAP_TIM3_CH2  1
#define CAP_TIM3_CH3  0
#define CAP_TIM3_CH4  1
//#define CAP_TIM3_AFIO 3


#define CAP_TIM1_CH1  0
#define CAP_TIM1_CH2  1
#define CAP_TIM1_CH3  1
#define CAP_TIM1_CH4  0

#define CAP_TIM14_CH1  1
#define CAP_TIM14_CH2  0
#define CAP_TIM14_CH3  0
#define CAP_TIM14_CH4  0

//#define CAP_TIM1_AFIO 0

extern void TIM1_Cap_Init(u16  arr, u16  psc);
extern void TIM3_Cap_Init(u16  arr, u16  psc);
extern void TIM14_Cap_Init(u16  arr, u16  psc);




#if CAP_TIM1_CH1
extern u8  TIM1_CAPTURE_STA_CH1;
extern u16  TIM1_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM1_CH2
extern u8  TIM1_CAPTURE_STA_CH2;
extern u16  TIM1_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM1_CH3
extern u8  TIM1_CAPTURE_STA_CH3;
extern u16  TIM1_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM1_CH4
extern u8  TIM1_CAPTURE_STA_CH4;
extern u16  TIM1_CAPTURE_VAL_CH4;
#endif
#if CAP_TIM2_CH1
extern u8  TIM2_CAPTURE_STA_CH1;
extern u16  TIM2_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM2_CH2
extern u8  TIM2_CAPTURE_STA_CH2;
extern u16  TIM2_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM2_CH3
extern u8  TIM2_CAPTURE_STA_CH3;
extern u16  TIM2_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM2_CH4
extern u8  TIM2_CAPTURE_STA_CH4;
extern u16  TIM2_CAPTURE_VAL_CH4;
#endif
#if CAP_TIM3_CH1
extern u8  TIM3_CAPTURE_STA_CH1;
extern u16  TIM3_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM3_CH2
extern u8  TIM3_CAPTURE_STA_CH2;
extern u16  TIM3_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM3_CH3
extern u8  TIM3_CAPTURE_STA_CH3;
extern u16  TIM3_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM3_CH4
extern u8  TIM3_CAPTURE_STA_CH4;
extern u16  TIM3_CAPTURE_VAL_CH4;
#endif
#if CAP_TIM4_CH1
extern u8  TIM4_CAPTURE_STA_CH1;
extern u16  TIM4_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM4_CH2
extern u8  TIM4_CAPTURE_STA_CH2;
extern u16  TIM4_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM4_CH3
extern u8  TIM4_CAPTURE_STA_CH3;
extern u16  TIM4_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM4_CH4
extern u8  TIM4_CAPTURE_STA_CH4;
extern u16  TIM4_CAPTURE_VAL_CH4;
#endif
#if CAP_TIM5_CH1
extern u8  TIM5_CAPTURE_STA_CH1;
extern u16  TIM5_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM5_CH2
extern u8  TIM5_CAPTURE_STA_CH2;
extern u16  TIM5_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM5_CH3
extern u8  TIM5_CAPTURE_STA_CH3;
extern u16  TIM5_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM5_CH4
extern u8  TIM5_CAPTURE_STA_CH4;
extern u16  TIM5_CAPTURE_VAL_CH4;
#endif
#if CAP_TIM8_CH1
extern u8  TIM8_CAPTURE_STA_CH1;
extern u16  TIM8_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM8_CH2
extern u8  TIM8_CAPTURE_STA_CH2;
extern u16  TIM8_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM8_CH3
extern u8  TIM8_CAPTURE_STA_CH3;
extern u16  TIM8_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM8_CH4
extern u8  TIM8_CAPTURE_STA_CH4;
extern u16  TIM8_CAPTURE_VAL_CH4;
#endif
#if CAP_TIM14_CH1
extern u8  TIM14_CAPTURE_STA_CH1;
extern u16 TIM14_CAPTURE_VAL_CH1;
#endif
#if CAP_TIM14_CH2
extern u8  TIM14_CAPTURE_STA_CH2;
extern u16 TIM14_CAPTURE_VAL_CH2;
#endif
#if CAP_TIM14_CH3
extern u8  TIM14_CAPTURE_STA_CH3;
extern u16 TIM14_CAPTURE_VAL_CH3;
#endif
#if CAP_TIM14_CH4
extern u8  TIM14_CAPTURE_STA_CH4;
extern u16 TIM14_CAPTURE_VAL_CH4;
#endif
#endif

