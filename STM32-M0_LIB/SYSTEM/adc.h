/***************************************************************/
#include "stm32f0xx.h"
/***************************************************************/
#ifndef __ADC_H
#define __ADC_H 
/***************************************************************/
//PA0~PA7 is 0~7 Channel	
//PB1 is 9 Channel
/***************************************************************/

void ADC_Configuration(u16 ADC_Channel);
int Get_ADC_Voltage(void);
void Change_Channel(u16 ADC_Channel);


#endif


