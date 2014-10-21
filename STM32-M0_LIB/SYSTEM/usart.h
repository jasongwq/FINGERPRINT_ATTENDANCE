/***************************************************************/
#ifndef __USART_H
#define	__USART_H
/***************************************************************/
#include "stm32f0xx.h"
#include <stdio.h>
/***************************************************************/
void USART_Configuration(u32 BaudRate);
void USART_Configuration2(u32 BaudRate);
int fputc(int ch, FILE *f);
#define Printf_USART USART1
/***************************************************************/
#endif
