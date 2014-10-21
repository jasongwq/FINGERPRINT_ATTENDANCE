/***************************************************************/
#include "stm32f0xx.h"
/***************************************************************/
#ifndef __TIM_H
#define __TIM_H 
/***************************************************************/

void TIM_Configuration(TIM_TypeDef* TIMER,u16 Period,u16 Prescaler,u8 PP); // Prescaler/8M*Period S  800/8M*10000
																			//TIM_Configuration(TIM2,10000,800,1); 
#endif


