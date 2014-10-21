#include "../../STM32-M0_LIB/PERIPHERAL/NRF905/nrf905.h"
#include "user.h"

int task_nrf905(void)
{
    _SS
    nRF905Init();           //NRF905 初始化
    while (NRF905_Check() != SUCCESS)
    {
        NRF_ID = NRF_NULL;
        printf("Can Not Find NRF905..!! \r\n");
        WaitX(100);
        nRF905Init();
    }
    printf("MODEL_TRX \r\n");
    NRF_ID = NRF905;
    SetRxMode();            //设置 模式 为 发送
    WaitX(2);
    while (1)
    {
        cha = (u8)((u8)Rx_Adr - (u8)Rx_start);
        if (( cha > 30) || (Rx_IDLE))
        {
            if (CheckRx())
            {
                SetTxMode();            //设置 模式 为 发送
                WaitX(2);
            }
            if (cha > 30)
            {
                TxRxBuf[0] = (30 | 0xa0);
                TxRxBuf[1] = 0;
                for (int i = 2; i < 32; i++)
                {
                    TxRxBuf[1] += RxBuffer[Rx_start];
                    TxRxBuf[i] = RxBuffer[Rx_start];
                    Rx_start = ((++Rx_start) & 0xff);
                }
            }
            else
            {
                Rx_IDLE = 0;
                TxRxBuf[0] = (cha) | 0xa0;
                TxRxBuf[1] = 0;
                for (int i = 2; i < (cha + 2); i++)
                {
                    TxRxBuf[1] += RxBuffer[Rx_start];
                    TxRxBuf[i] = RxBuffer[Rx_start];
                    Rx_start = ((++Rx_start) & 0xff);
                }
            }
            TxPacket(TxRxBuf);//发送 命令，数据
            while (!CheckTxOver())WaitX(1);
            while (!CheckTxOver())WaitX(1);
            while (!CheckTxOver())WaitX(1);
            cha = (u8)((u8)Rx_Adr - (u8)Rx_start);
            if (cha == 0)
            {
                SetRxMode();   //设置 模式 为 接受
                WaitX(2);
            }
        }
        if (CheckRx())
        {
            if (CheckDR() == 1)
            {
                RxPacket();
                int sum = 0;
                for (int i = 0; i < 32; i++)
                {
                    nrf_test[i] = TxRxBuf[i];
                    TxRxBuf[i] = 0;
                }
                if ((nrf_test[0] & 0xa0) == 0xa0)
                {
                    for (int i = 2; i < (nrf_test[0] & 0x1f) + 2; i++)
                        sum += nrf_test[i];
                    if ((sum & 0xff) == nrf_test[1])
                        Sys_sPrintf(Printf_USART, nrf_test + 2, (nrf_test[0] & 0x1f));
                }
            }
        }
        WaitX(1);
    }
    _EE
}
