/***************************************************************/
#include "stm32f0xx.h"
/***************************************************************/
#ifndef __PWM_H
#define __PWM_H
/***************************************************************/
//频率计算  ： 72M / Prescaler / Period
//                                                     TIM8_PWM_Configuration(36,40000,1,1,1,0); 频率= 72M /36 /40000 =50 Hz  初始化了前三个通道
//                                                     TIM8_PWM_Configuration(1,3600,0,0,0,1); 频率= 72M /1 /3600 =20 KHz    初始化了后一个通道
//输出占空比计算公式：TIM_Pulse / Period
//
//         TIM3_PWM_Configuration(24,40000,1,1,0,0);        A6: TIM3_CH1    A7:TIM3_CH2
//         TIM1_PWM_Configuration(24,40000,0,1,1,0);        A9: TIM1_CH2    A10:TIM1_CH3
//
/***************************************************************/



void TIM1_PWM_Configuration(u16 Prescaler, u16 Period, u8 OC1 , u8 OC2 , u8 OC3 , u8 OC4);
void TIM1_Change_Period(u16 NoteSet);
void Change_TIM1_OC1_Pulse(u16 Pulse);
void Change_TIM1_OC2_Pulse(u16 Pulse);
void Change_TIM1_OC3_Pulse(u16 Pulse);
void Change_TIM1_OC4_Pulse(u16 Pulse);
void Change_TIM1_OC1_Polarity(u8 i);
void Change_TIM1_OC2_Polarity(u8 i);
void Change_TIM1_OC3_Polarity(u8 i);
void Change_TIM1_OC4_Polarity(u8 i);


void TIM3_PWM_Configuration(u16 Prescaler, u16 Period, u8 OC1 , u8 OC2 , u8 OC3 , u8 OC4);
void TIM3_Change_Period(u16 NoteSet);
void Change_TIM3_OC1_Pulse(u16 Pulse);
void Change_TIM3_OC2_Pulse(u16 Pulse);
void Change_TIM3_OC3_Pulse(u16 Pulse);
void Change_TIM3_OC4_Pulse(u16 Pulse);
void Change_TIM3_OC1_Polarity(u8 i);
void Change_TIM3_OC2_Polarity(u8 i);
void Change_TIM3_OC3_Polarity(u8 i);
void Change_TIM3_OC4_Polarity(u8 i);



#endif
