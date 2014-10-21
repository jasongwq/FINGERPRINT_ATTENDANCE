#include "./sys_usart.h"
#include <stdarg.h>
#include "../../STM32-M0_LIB/PERIPHERAL/NRF24L01/G32-M0_Nrf24l01.h"

/******************************************************
        整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
        radix=10 标示是10进制  非十进制，转换结果为0;
        例：d=-379;
        执行  itoa(d, buf, 10); 后
        buf="-379"
**********************************************************/
char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;
    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }
    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }
    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';
        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;
        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }
    /* Null terminate the string. */
    *ptr = 0;
    return string;
} /* NCL_Itoa */
/****************************************************************************
* 名    称：void USART_OUT(USART_TypeDef* USARTx, uint8_t *Data,...)
* 功    能：格式化串口输出函数
* 入口参数：USARTx:  指定串口
            Data：   发送数组
            ...:     不定参数
* 出口参数：无
* 说    明：格式化串口输出函数
      "\r"  回车符      USART_OUT(USARTx, "abcdefg\r")
            "\n"    换行符      USART_OUT(USARTx, "abcdefg\r\n")
            "%s"    字符串      USART_OUT(USARTx, "字符串是：%s","abcdefg")
            "%d"    十进制      USART_OUT(USARTx, "a=%d",10)
* 调用方法：无
****************************************************************************/
void Sys_Printf(USART_TypeDef *USARTx, char *Data, ...)
{
    const char *s;
    int d;
    char buf[16];
    va_list ap;
    va_start(ap, Data);
    while (*Data != 0)                                        //判断是否到达字符串结束符
    {
        if (*Data == 0x5c)                                    //'\'
            switch (*++Data)
            {
            case 'r':                                     //回车符
                SYS_USART_SendData(USARTx, 0x0d);
                Data++;
                break;
            case 'n':                                     //换行符
                SYS_USART_SendData(USARTx, 0x0a);
                Data++;
                break;
            default:
                Data++;
                break;
            }
        else if (*Data == '%')
            switch (*++Data)
            {
            case 's':                                         //字符串
                s = va_arg(ap, const char *);
                for ( ; *s; s++)
                {
                    SYS_USART_SendData(USARTx, *s);
                    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                }
                Data++;
                break;
            case 'd':                                         //十进制
                d = va_arg(ap, int);
                itoa(d, buf, 10);
                for (s = buf; *s; s++)
                {
                    SYS_USART_SendData(USARTx, *s);
                    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
                }
                Data++;
                break;
            default:
                Data++;
                break;
            }
        else SYS_USART_SendData(USARTx, *Data++);
        //while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
    }
}
static u8 TxBuffer[3][256];//发送缓存
static u8 TxCount[3] = {0};//发送尾
#define RxBuffer_NUM 256
u8 RxBuffer[RxBuffer_NUM];   //两个32字节的串口接收缓存
u8 Rx_start = 0;      			//接受ok 标志
u8 Rx_Adr = 0;      //正在接收第几字节
int Rx_IDLE=0;

void SYS_UART_IQR(USART_TypeDef *USARTx)
{
    static u8 TxCounter[3] = {0};//当前发送
    if (USARTx->ISR & USART_IT_ORE)
    {
        USARTx->ISR;
    }
    //发送中断
    //if ((USARTx->SR & (1 << 7)) && (USARTx->CR1 & USART_CR1_TXEIE))
    if (USART_GetITStatus(USARTx, USART_IT_TXE) != RESET)
    {
        int USARTn;
        if (USARTx == USART1)
        {
            USARTn = 0;
        }
        else if (USARTx == USART2)
        {
            USARTn = 1;
        }
        USARTx->TDR = TxBuffer[USARTn][TxCounter[USARTn]++]; //写DR清除中断标志
        if (TxCounter[USARTn] == TxCount[USARTn])
            USARTx->CR1 &= ~USART_CR1_TXEIE;        //关闭TXE中断//USART_ITConfig(USARTx,USART_IT_TXE,DISABLE);
    }
    //接收中断 (接收寄存器非空)
    //if (USARTx->SR & (1 << 5))
    if (USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)
    {
        RxBuffer[Rx_Adr] = USARTx->RDR;
        Rx_Adr=((++Rx_Adr)&0xff);
    }
		if (USART_GetITStatus(USARTx, USART_IT_IDLE) != RESET)
    {
        USART_ClearITPendingBit(USARTx, USART_IT_IDLE);
				Rx_IDLE=1;
    }
		
    //Sys_Printf(USARTx,"2");
}
/**************************实现函数********************************************
*******************************************************************************/
uint8_t SYS_USART_SendData(USART_TypeDef *USARTx, unsigned char DataToSend)
{
    int USARTn;
    if (USARTx == USART1)
    {
        USARTn = 0;
    }
    else if (USARTx == USART2)
    {
        USARTn = 1;
    }
    TxBuffer[USARTn][TxCount[USARTn]++] = DataToSend;
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    return DataToSend;
}
int fputc(int ch, FILE *f)
{
    USART_SendData(Printf_USART, (u8) ch);
    while (USART_GetFlagStatus(Printf_USART, USART_FLAG_TC) == RESET);
    return ch;
}
//int fputc(int ch, FILE *f)                        //串口PRINTF函数配置程序
//{
//    SYS_USART_SendData(Printf_USART, (unsigned char) ch);
//    while (!(USART1->ISR & USART_FLAG_TXE));         //等待发送完成
//    return (ch);
//}
//int fputc(int ch, FILE *f)                        //串口PRINTF函数配置程序
//{
//    USART2->TDR=ch;//SYS_USART_SendData(Printf_USART, (unsigned char) ch);
//    while (!(USART2->ISR & USART_FLAG_TXE));         //等待发送完成
//    return (ch);
//}
uint8_t *Sys_sPrintf(USART_TypeDef *USARTx, unsigned char *DataToSend, unsigned char num)
{
    int USARTn;
    if (USARTx == USART1)
    {
        USARTn = 0;
    }
    else if (USARTx == USART2)
    {
        USARTn = 1;
    }
    for (int i = 0; i < num; i++)
        TxBuffer[USARTn][TxCount[USARTn]++] = *(DataToSend + i);
    USART_ITConfig(USARTx, USART_IT_TXE, ENABLE);
    return DataToSend;
}



