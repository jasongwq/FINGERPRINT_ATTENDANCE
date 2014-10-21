/***************************************************************/
#include "stm32f0xx.h"
/***************************************************************/
#ifndef __PWM_H
#define __PWM_H 
/***************************************************************/
//Ƶ�ʼ���  �� 72M / Prescaler / Period 		  
//													   TIM8_PWM_Configuration(36,40000,1,1,1,0); Ƶ��= 72M /36 /40000 =50 Hz  ��ʼ����ǰ����ͨ��
//													   TIM8_PWM_Configuration(1,3600,0,0,0,1); Ƶ��= 72M /1 /3600 =20 KHz    ��ʼ���˺�һ��ͨ��
//���ռ�ձȼ��㹫ʽ��TIM_Pulse / Period
//
//         TIM3_PWM_Configuration(24,40000,1,1,0,0);        A6: TIM3_CH1    A7:TIM3_CH2
//         TIM1_PWM_Configuration(24,40000,0,1,1,0);        A9: TIM1_CH2    A10:TIM1_CH3
//
/***************************************************************/



void TIM1_PWM_Configuration(u16 Prescaler,u16 Period,u8 OC1 ,u8 OC2 ,u8 OC3 ,u8 OC4);
void TIM1_Change_Period(u16 NoteSet);
void Change_TIM1_OC1_Pulse(u16 Pulse);
void Change_TIM1_OC2_Pulse(u16 Pulse);
void Change_TIM1_OC3_Pulse(u16 Pulse);
void Change_TIM1_OC4_Pulse(u16 Pulse);
void Change_TIM1_OC1_Polarity(u8 i);
void Change_TIM1_OC2_Polarity(u8 i);
void Change_TIM1_OC3_Polarity(u8 i);
void Change_TIM1_OC4_Polarity(u8 i);


void TIM3_PWM_Configuration(u16 Prescaler,u16 Period,u8 OC1 ,u8 OC2 ,u8 OC3 ,u8 OC4);
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
