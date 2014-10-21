#include "../../../STM32-M0_LIB/PERIPHERAL/NRF24L01/G32-M0_Nrf24l01.h"
#include "user.h"

void Nrf_Check_Event(void)
{
    u8 sta = Nrf_Get_Sta();
    ////////////////////////////////////////////////////////////////
    if (sta & (1 << RX_DR))
    {
        int sum = 0;
        u8 rx_len = NRF_Read_Reg(R_RX_PL_WID);
        NRF_Read_Buf(RD_RX_PLOAD, NRF24L01_RXDATA, rx_len);
        NRF_Write_Reg(FLUSH_RX, 0xff);
        for (int i = 0; i < 32; i++)nrf_test[i] = NRF24L01_RXDATA[i];
        if ((nrf_test[0] & 0xa0) == 0xa0)
        {
            for (int i = 2; i < (nrf_test[0] & 0x1f) + 2; i++)
                sum += nrf_test[i];
            if ((sum & 0xff) == nrf_test[1])
						{
                Sys_sPrintf(USART1, nrf_test + 2, (nrf_test[0] & 0x1f));
								LED_STA=LED_RXD;
if(ledruntime<4)ledruntime+=2;
								}
        }
    }
    ////////////////////////////////////////////////////////////////
    if (sta & (1 << TX_DS))
    {
    }
    ////////////////////////////////////////////////////////////////
    if ((sta & (1 << MAX_RT)) || (sta & 0x01)) //??????????
    {
        NRF_Write_Reg(FLUSH_TX, 0xff);
    }
    ////////////////////////////////////////////////////////////////
    NRF_Write_Reg(NRF_WRITE_REG + NRFRegSTATUS, sta);
}
////////////////////////////////////////////////////////////////////////////////
void Data_Exchange(void)
{
    Nrf_Check_Event();
    u8 sta = Nrf_Get_FIFOSta();
    if ((sta & (1 << 4)) == 0)
        return;
    cha = (u8)((u8)Rx_Adr - (u8)Rx_start);
    if (( cha > 30) || (Rx_IDLE))
    {
        if (cha > 30)
        {
            NRF24L01_TXDATA[0] = (30 | 0xa0);
            NRF24L01_TXDATA[1] = 0;
            for (int i = 2; i < 32; i++)
            {
                NRF24L01_TXDATA[1] += RxBuffer[Rx_start];
                NRF24L01_TXDATA[i] = RxBuffer[Rx_start];
                Rx_start = ((++Rx_start) & 0xff);
            }
        }
        else
        {
            Rx_IDLE = 0;
            NRF24L01_TXDATA[0] = (cha) | 0xa0;
            NRF24L01_TXDATA[1] = 0;
            for (int i = 2; i < (cha + 2); i++)
            {
                NRF24L01_TXDATA[1] += RxBuffer[Rx_start];
                NRF24L01_TXDATA[i] = RxBuffer[Rx_start];
                Rx_start = ((++Rx_start) & 0xff);
            }
        }
#if TX_RX_MODE
        NRF_TxPacket(NRF24L01_TXDATA, 32);
#else
        NRF_TxPacket_AP(NRF24L01_TXDATA, 32);
#endif
LED_STA=LED_TXD;
if(ledruntime<4)ledruntime+=2;
    }
    else
    {
		#if TX_RX_MODE
        NRF24L01_TXDATA[0] = (0 | 0x60);
        NRF24L01_TXDATA[1] = 0;
        NRF_TxPacket(NRF24L01_TXDATA, 32);
		#endif
				LED_STA=LED_NULL;
    }
}
#if TX_RX_MODE
int task_nrf24l01_t(void)
{
    _SS
    Nrf24l01_Init(MODEL_TX2, 1);
    while (Nrf24l01_Check() == ERROR)
    {
        NRF_ID = NRF_NULL;
        //printf("Can Not Find NRF24L01..!! \r\n");
				Dbs(1,"Can Not Find NRF24L01..!! \r\n");
        WaitX(100);
    }
    //printf("NRF24L01 TX\r\n");
		Dbs(1,"NRF24L01 TX\r\n");
    NRF_ID = NRF24L01;
    for (int i = 0; i < 32; i++)nrf_test[i] = i;
    while (1)
    {
        WaitX(1);
        Data_Exchange();
    }
    _EE
}
#else
int task_nrf24l01_r(void)
{
    _SS
    Nrf24l01_Init(MODEL_RX2, 1);
    while (Nrf24l01_Check() == ERROR)
    {
		    NRF_ID = NRF_NULL;
        //printf("Can Not Find NRF24L01..!! \r\n");
				Dbs(1,"Can Not Find NRF24L01..!! \r\n");
				WaitX(100);
    }
    //printf("NRF24L01 RX\r\n");
    Dbs(1,"NRF24L01 RX\r\n");
    NRF_ID = NRF24L01;
    while (1)
    {
        WaitX(1);
        Data_Exchange();
    }
    _EE
}
#endif
