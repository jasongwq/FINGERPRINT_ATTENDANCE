#ifndef __USART_H
#define __USART_H
//#include <stdio.h>
#include "sys.h"
#include <stdio.h>
#include "Usart_Config.h"
#include "sys_usart.h"

/********************************************
REMAP |  00      |   01         | 11        |
      |  TX  RX  |   TX  RX     | TX  RX    |
******|**********|**************|*******    |
USART1|  A9  A10 |   B6  B7     |           |
******|**********|**************|*******    |
USART2|  A2  A3  |   (D5  D6)   |           |
      |          |  (100)(144)  |           |
******|**********|**************|*******    |
USART3|  B10 B11 |   (C10 C11)  | (D8  D9)  |
      |          | (64 100 144) | (100 144) |
*********************************************
2013/8/27                            ********
2013/7/21                            ********
********************************************/

#define SENDBUFF_SIZE 5168
void USART_DMA_Config(USART_TypeDef *USARTx, u32 SendBuff);
void USART_DMA_Enable(USART_TypeDef *USARTx);

#define USART_SEND_LEN           200     //定义最大接收字节数 200   
extern char  USART_TX_BUF[];

/*****************是否使能 串口******************************/
#if EN_USART_
#define EN_USART_RX             1       //使能（1）/禁止（0）串口1接收

void uart_init(u32 bound);
//extern u8  USART1_TX_BUF[];
#endif
#if EN_USART2_
#define EN_USART2_RX            1       //使能（1）/禁止（0）串口2接收

void uart2_init(u32 bound);
//extern u8  USART2_TX_BUF[];

#endif
#if EN_USART3_
#define EN_USART3_RX            1       //使能（1）/禁止（0）串口3接收

void uart3_init(u32 bound);
//extern u8  USART3_TX_BUF[];

#endif
/*****************是否使能 串口******************************/

/*****************是否使能 接收******************************/
#if EN_USART_RX
#define USART_REC_LEN           200     //定义最大接收字节数 200   
void ATK_Usart1_IQR(void);

extern u8  USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART_RX_STA;                //接收状态标记
#endif

#if EN_USART2_RX
#define USART2_REC_LEN          200     //定义最大接收字节数 200   
//#define USART2_MAX_RECV_LEN     800
//#define USART2_MAX_SEND_LEN     200

extern u8  USART2_RX_BUF[USART2_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART2_RX_STA;               //接收状态标记
#endif

#if EN_USART3_RX
#define USART3_REC_LEN          200     //定义最大接收字节数 200   

extern u8  USART3_RX_BUF[USART3_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
extern u16 USART3_RX_STA;               //接收状态标记
#endif
/*****************是否使能 接收******************************/

#endif
