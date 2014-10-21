#include "usr_usart.h"
#include <stdarg.h>


//#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
//#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
//#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
//#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
//u8 data_to_send[32];
//u16 up = 0, down= 0, left = 0, right = 0;

//void SendValue(void)
//{
//    u8 _cnt = 0;
//    data_to_send[_cnt++] = 0xAA;
//    data_to_send[_cnt++] = 0xAF;
//    data_to_send[_cnt++] = 0x10;
//    data_to_send[_cnt++] = 0;
//    data_to_send[_cnt++] = BYTE1(up);
//    data_to_send[_cnt++] = BYTE0(up);
//    data_to_send[_cnt++] = BYTE1(down);
//    data_to_send[_cnt++] = BYTE0(down);
//    data_to_send[_cnt++] = BYTE1(left);
//    data_to_send[_cnt++] = BYTE0(left);
//    data_to_send[_cnt++] = BYTE1(right);
//    data_to_send[_cnt++] = BYTE0(right);
//    data_to_send[3] = _cnt-4;
////    SYS_USART_SendBuf(DEBUG_USART,data_to_send, _cnt);
//}
//void Uart_DataAnl(u8 buf_num)
//{
//    if (Rx_Buf[0][buf_num][0] == 0xAA)
//    {
//          if (Rx_Buf[0][buf_num][0] == 0xAF)
//    {
//
//    }
//    }
//}
//extern u8 Rx_Ok0;
//extern u8 Rx_Ok1;

//void Uart_CheckEvent(void)
//{
//    if (Rx_Ok0)
//    {
//        Rx_Ok0 = 0;
//        u8 sum = 0;
//        for (int i = 0; i < Rx_Buf[0][3]; i++)
//            sum += Rx_Buf[0][i];
//        if (sum == Rx_Buf[0][Rx_Buf[0][3]])
//        {
//            Uart_DataAnl(0);
//        }
//    }
//    if (Rx_Ok1)
//    {
//        Rx_Ok1 = 0;
//        u8 sum = 0;
//        for (int i = 0; i < Rx_Buf[1][3]-1; i++)
//            sum += Rx_Buf[1][i];
//        if (sum == Rx_Buf[1][Rx_Buf[1][3]-1])
//        {
//            Uart_DataAnl(1);
//        }
//    }
//}
