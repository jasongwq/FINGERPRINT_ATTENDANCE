#include "stm32f0xx_it.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"
#include "stdio.h"
#include "sys_os.h"


//void TIM2_IRQHandler(void)
//{
//  if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
//  {
//      printf("This is TIM Demo !! \r\n");
//      TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//  }
//}
void SYS_UART_IQR(USART_TypeDef *USARTx);
void USART1_IRQHandler(void)
{
  SYS_UART_IQR(USART1);
}
//static u8 TxBuffer[0xff];
//static u8 count = 0;//发送尾
//u8 Rx_Ok0 = 0;      //接受ok 标志
//u8 Rx_Ok1 = 0;      //接受ok 标志
//u8 Rx_Buf[2][32];   //两个32字节的串口接收缓存

//void SYS_UART_IQR(USART_TypeDef *USARTx)
//{
//    static u8 TxCounter = 0;
//	  static u8 Rx_Act = 0;      //正在使用的buf号
//    static u8 Rx_Adr = 0;      //正在接收第几字节
//    if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
//    {
//        USARTx->TDR = TxBuffer[TxCounter++];
//        if (TxCounter == count)
//            USARTx->CR1 &= ~USART_CR1_TXEIE;
//    }
//    // 接收中断 (接收寄存器非空)
//		if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
//    {
//        u8 com_data = USARTx->RDR;
//				USARTx->TDR=com_data;
//        if (Rx_Adr == 0)    //寻找帧头0X8A
//        {
//            if (com_data == 0x8A) //接收数据如果是0X8A,则写入缓存
//            {
//                Rx_Buf[Rx_Act][0] = com_data;
//                Rx_Adr = 1;
//            }
//        }
//        else        //正在接收数据
//        {
//            Rx_Buf[Rx_Act][Rx_Adr] = com_data;
//            Rx_Adr ++;
//        }
//        if (Rx_Adr == 32)   //数据接收完毕
//        {
//            Rx_Adr = 0;
//            if (Rx_Act)
//            {
//                Rx_Act = 0;             //切换缓存
//                Rx_Ok1 = 1;
//            }
//            else
//            {
//                Rx_Act = 1;
//                Rx_Ok0 = 1;
//            }
//        }
//    }
//}
void SysTick_Handler(void)
{
    UpdateTimers();
}

