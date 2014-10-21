#ifndef __SYS_USART_H_
#define __SYS_USART_H_
#include "sys.h"
#include "usart.h"
void Sys_Printf(USART_TypeDef *USARTx, char *Data, ...);
extern uint8_t *Sys_sPrintf(USART_TypeDef *USARTx, unsigned char *DataToSend, unsigned char num);
extern uint8_t SYS_USART_SendData(USART_TypeDef *USARTx, unsigned char DataToSend);

void Uart_CheckEvent(void);
#endif
