/*
    ����̳��Ǳˮ�������ˣ������ѧϰ����ƽ�⳵���������������ࡣ
�ڵ��Դ��������ݺ��˲�ʱд��һ�����ڹ��ߣ��������ĩ����
��һ�£����������ϣ���ܶԳ�ѧ��������������
    ���ܱȽϼ򵥣���Ҫ�ǽ��յ�Ƭ����������3���������ݣ���ͼ����ʾ��
������Ƭ�����ͼ����ֽڣ���������Ŀ�ġ���Ƭ�����͵�������0xFF��Ϊ
֡������־��ÿ����������5���ֽڱ�ʾ�������������ĺ�����
    û�о���ȫ����ԣ���������õ���bug�����Է������� :)

                                           СƤ2005
                                           2012.12.08
                                                                    */
/*******************************************************************/
#include "sys.h"
#include "Balance_Show.h"

void uart_transmit(USART_TypeDef *USARTx, char ch)
{
    while ((USARTx->SR & 0X40) == 0); //ѭ������,ֱ���������
    USARTx->DR = (char) ch;
}

void SeriPush2Bytes(USART_TypeDef *USARTx, int value)
{
    unsigned char t_char;
    if (value < 0)
    {
        value = -value;
        uart_transmit(USARTx, 0xf0);
    }
    else
        uart_transmit(USARTx, 0xf5);
    t_char = value / 256;
    if (t_char > 127)
        uart_transmit(USARTx, 0x80);
    else
        uart_transmit(USARTx, 0x00);
    uart_transmit(USARTx, t_char & 0x7f);
    t_char = value % 256;
    if (t_char > 127)
        uart_transmit(USARTx, 0x80);
    else
        uart_transmit(USARTx, 0x00);
    uart_transmit(USARTx, t_char & 0x7f);
}



void Print_Balance(USART_TypeDef *USARTx, int data1, int data2, int data3)
{
    SeriPush2Bytes(USARTx, data1);
    SeriPush2Bytes(USARTx, data2);
    SeriPush2Bytes(USARTx, data3);
    uart_transmit(USARTx, 0xff);
}


