#include "sys.h"
#include "sys_usart.h"
#include "data_transfer.h"
//#include "../../../GLIB/PERIPHERAL/NRF24L01/G32_Nrf24l01.h"

//#include "bak.h"

#define DATA_TRANSFER_USE_USART
#define DATA_TRANSFER_USE_SPI_NRF
#define DATA_TRANSFER_USARTx USART3


u8 Data_Check, Send_Status, Send_Senser, Send_RCData, Send_GpsData, Send_Offset, Send_PID1, Send_PID2, Send_PID3, Send_MotoPwm;
PID PID_ROL, PID_PIT, PID_YAW, PID_ALT, PID_POS, PID_PID_1, PID_PID_2;

void Data_Receive_Anl(u8 *data_buf, u8 num)
{
    vs16 rc_value_temp;
    u8 sum = 0;
    for (u8 i = 0; i < (num - 1); i++)
        sum += *(data_buf + i);
    if (!(sum == *(data_buf + num - 1)))       return; //sum
    if (!(*(data_buf) == 0xAA && *(data_buf + 1) == 0xAF))     return; //
    /////////////////////////////////////////////////////////////////////////////////////
    if (*(data_buf + 2) == 0X01)
    {
        if (*(data_buf + 4) == 0X01)
            ;//MPU6050_CalOff_Acc();
        if (*(data_buf + 4) == 0X02)
            ;// MPU6050_CalOff_Gyr();
        if (*(data_buf + 4) == 0X03)
        {
            ;//MPU6050_CalOff_Acc();
            ;//MPU6050_CalOff_Gyr();
        }
        if (*(data_buf + 4) == 0X04)
            ;//Cal_Compass();
        if (*(data_buf + 4) == 0X05)
            ;//MS5611_CalOffset();
    }
}
