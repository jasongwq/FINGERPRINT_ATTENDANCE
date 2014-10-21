#ifndef __USER_H_
#define __USER_H_
#include "sys_usart.h"
#include "sys_os.h"
#include "gpio.h"
/***************************************************************/
#define TX_RX_MODE 1   //(1)Tx (0)Rx
/***************************************************************/

#if TX_RX_MODE
extern int task_nrf24l01_t(void);
#else
extern int task_nrf24l01_r(void);
#endif
extern int task_nrf905(void);
/***************************************************************/
enum NRF_ID_TYPE {NRF_NULL, NRF24L01, NRF905}; //无线型号
enum LED_STATE {LED_TXD, LED_RXD, LED_ERROR,LED_START,LED_RUN,LED_NULL}; //无线型号
extern int ledruntime;
extern enum LED_STATE LED_STA;//无线型号
extern enum NRF_ID_TYPE NRF_ID;//无线型号
extern u8 nrf_test[32];
extern u8 cha;//串口已接收 但未转发完成的数据量
extern int Rx_IDLE;     //接受ok 标志
extern u8  Rx_start;    //接受ok 标志
extern u8  Rx_Adr;      //正在接收第几字节
extern u8  RxBuffer[];   //两个32字节的串口接收缓存
/***************************************************************/
void SYS_INIT(void);
/***************************************************************/

#endif

