#ifndef __NRF905_H__
#define __NRF905_H__
#include "sys.h"

/****************************************/
/***NRF905***/
/********************************************
***********************************************\
 * __________________*
 * |13 VCC | 14 TXEN | *   B1
 * |-------*         | *
 * |11 CE    12 PWR  | *F0 F1
 * |                 | *
 * | 9 UCK   10 CD   | *A0 A1
 * |                 | *
 * | 8 AM     7 DR   | *A4 A6
 * |                 | *
 * | 6 MISO   5 MOSI | *A7 A5
 * |                 | *
 * | 4 SCK    3 CSN  | *A3 A2
 * |---------------- | *
 * | 2 GND  | 1 GND  | *
 * ------------------*
\**********************************************/
extern u8 TxRxBuf[];
void GPIO_NRF905_Init(void);
void nRF905Init(void);
void Config_Nrf905(void);
void TxPacket(u8 *TxRxBuf);
void SetTxMode(void);
void SetRxMode(void);
u8 CheckDR(void);
void RxPacket(void);
int CheckRx(void);        //检查是否为接收模式
int CheckTxOver(void);        //检查是否发送完成
u8 NRF905_Check(void);

#endif

