/*
================================================================================
Function : Operation for SI446x
http://shop57165217.taobao.com
================================================================================
*/
#include "./si446x.h"
#include "./radio_config_Si4461.h"
#include "si446x_config.h"
/*
=================================================================================
----------------------------PROPERTY fast setting macros-------------------------
=================================================================================
*/
// GOLBAL(0x00)
#define GLOBAL_XO_TUNE( x )                 SI446X_SET_PROPERTY_1( 0x0000, x )
#define GLOBAL_CLK_CFG( x )                 SI446X_SET_PROPERTY_1( 0x0001, x )
#define GLOBAL_LOW_BATT_THRESH( x )         SI446X_SET_PROPERTY_1( 0x0002, x )
#define GLOBAL_CONFIG( x )                  SI446X_SET_PROPERTY_1( 0x0003, x )
#define GLOBAL_WUT_CONFIG( x )              SI446X_SET_PROPERTY_1( 0x0004, x )
#define GLOBAL_WUT_M_15_8( x )              SI446X_SET_PROPERTY_1( 0x0005, x )
#define GLOBAL_WUT_M_7_0( x )               SI446X_SET_PROPERTY_1( 0x0006, x )
#define GLOBAL_WUT_R( x )                   SI446X_SET_PROPERTY_1( 0x0007, x )
#define GLOBAL_WUT_LDC( x )                 SI446X_SET_PROPERTY_1( 0x0008, x )
#define GLOBAL_WUT_CAL( x )                 SI446X_SET_PROPERTY_1( 0x0009, x )

// INT_CTL(0x01)
#define INT_CTL_ENABLE( x )                 SI446X_SET_PROPERTY_1( 0x0100, x )
#define INT_CTL_PH_ENABLE( x )              SI446X_SET_PROPERTY_1( 0x0101, x )
#define INT_CTL_MODEM_ENABLE( x )           SI446X_SET_PROPERTY_1( 0x0102, x )
#define INT_CTL_CHIP_ENABLE( x )            SI446X_SET_PROPERTY_1( 0x0103, x )

//group 0x02, FRR_CTL
#define FRR_CTL_A_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0200, x )
#define FRR_CTL_B_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0201, x )
#define FRR_CTL_C_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0202, x )
#define FRR_CTL_D_MODE( x )                 SI446X_SET_PROPERTY_1( 0x0203, x )

// PREAMBLE (0x10)




extern u8 SPI0_ReadWriteByte(u8 TxData);
#define SPI_ExchangeByte SPI0_ReadWriteByte


const static u8 config_table[] = RADIO_CONFIGURATION_DATA_ARRAY;

/*read a array of command response*/
void SI446X_READ_RESPONSE( u8 *buffer, u8 size );

/*wait the device ready to response a command*/
void SI446X_WAIT_CTS( void );

/*write data to TX fifo*/
void SI446X_W_TX_FIFO( u8 *txbuffer, u8 size );


/*
=================================================================================
SI446X_CMD();
Function : Send a command to the device
INTPUT   : cmd, the buffer stores the command array
           cmdsize, the size of the command array
OUTPUT   : NONE
=================================================================================
*/
void SI446X_CMD(const u8 *cmd, u8 cmdsize )
{
    SI446X_WAIT_CTS();
    SI_CSN_LOW();
    while ( cmdsize -- )
    {
        SPI_ExchangeByte( *cmd++ );
    }
    SI_CSN_HIGH();
}
/*
=================================================================================
SI446X_POWER_UP();
Function : Power up the device
INTPUT   : f_xtal, the frequency of the external high-speed crystal
OUTPUT   : NONE
=================================================================================
*/
void SI446X_POWER_UP( u32 f_xtal )
{
    u8 cmd[7];
    cmd[0] = POWER_UP;
    cmd[1] = 0x01;
    cmd[2] = 0x00;
    cmd[3] = f_xtal >> 24;
    cmd[4] = f_xtal >> 16;
    cmd[5] = f_xtal >> 8;
    cmd[6] = f_xtal;
    SI446X_CMD( cmd, 7 );
}
/*
=================================================================================
SI446X_READ_RESPONSE();
Function : read a array of command response
INTPUT   : buffer,  a buffer, stores the data responsed
           size,    How many bytes should be read
OUTPUT   : NONE
=================================================================================
*/
void SI446X_READ_RESPONSE( u8 *buffer, u8 size )
{
    SI446X_WAIT_CTS();
    SI_CSN_LOW();
    SPI_ExchangeByte( READ_CMD_BUFF );
    while ( size -- )
    {
        *buffer++ = SPI_ExchangeByte( 0xFF );
    }
    SI_CSN_HIGH();

}
/*
=================================================================================
SI446X_WAIT_CTS();
Function : wait the device ready to response a command
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
void SI446X_WAIT_CTS( void )
{
    u8 cts;
    do
    {
        SI_CSN_LOW();
        SPI_ExchangeByte( READ_CMD_BUFF );
        cts = SPI_ExchangeByte( 0xFF );
        SI_CSN_HIGH();
    }
    while ( cts != 0xFF );
}
/*
=================================================================================
SI446X_NOP();
Function : NO Operation command
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
u8 SI446X_NOP( void )
{
    u8 cts;
    SI_CSN_LOW();
    cts = SPI_ExchangeByte( NOP );
    SI_CSN_HIGH();
    return cts;
}

/*
=================================================================================
SI446X_PART_INFO();
Function : Read the PART_INFO of the device, 8 bytes needed
INTPUT   : buffer, the buffer stores the part information
OUTPUT   : NONE
=================================================================================
*/
void SI446X_PART_INFO( u8 *buffer )
{
    u8 cmd = PART_INFO;

    SI446X_CMD( &cmd, 1 );
    SI446X_READ_RESPONSE( buffer, 8 );

}
/*
=================================================================================
SI446X_FUNC_INFO();
Function : Read the FUNC_INFO of the device, 7 bytes needed
INTPUT   : buffer, the buffer stores the FUNC information
OUTPUT   : NONE
=================================================================================
*/
void SI446X_FUNC_INFO( u8 *buffer )
{
    u8 cmd = FUNC_INFO;

    SI446X_CMD( &cmd, 1 );
    SI446X_READ_RESPONSE( buffer, 7 );
}
/*
=================================================================================
SI446X_INT_STATUS();
Function : Read the INT status of the device, 9 bytes needed
INTPUT   : buffer, the buffer stores the int status
OUTPUT   : NONE
=================================================================================
*/
void SI446X_INT_STATUS( u8 *buffer )
{
    u8 cmd[4];
    cmd[0] = GET_INT_STATUS;
    cmd[1] = 0;
    cmd[2] = 0;
    cmd[3] = 0;

    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( buffer, 9 );

}
/*
=================================================================================
SI446X_GET_PROPERTY();
Function : Read the PROPERTY of the device
INTPUT   : buffer, the buffer stores the PROPERTY value
           GROUP_NUM, the group and number of the parameter
           NUM_GROUP, number of the group
OUTPUT   : NONE
=================================================================================
*/
void SI446X_GET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, u8 NUM_PROPS, u8 *buffer  )
{
    u8 cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM >> 8;
    cmd[2] = NUM_PROPS;
    cmd[3] = GROUP_NUM;

    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( buffer, NUM_PROPS + 1 );
}
/*
=================================================================================
SI446X_SET_PROPERTY_X();
Function : Set the PROPERTY of the device
INTPUT   : GROUP_NUM, the group and the number of the parameter
           NUM_GROUP, number of the group
           PAR_BUFF, buffer stores parameters
OUTPUT   : NONE
=================================================================================
*/
void SI446X_SET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, u8 NUM_PROPS, u8 *PAR_BUFF )
{
    u8 cmd[20], i = 0;
    if ( NUM_PROPS >= 16 )
    {
        return;
    }
    cmd[i++] = SET_PROPERTY;
    cmd[i++] = GROUP_NUM >> 8;
    cmd[i++] = NUM_PROPS;
    cmd[i++] = GROUP_NUM;
    while ( NUM_PROPS-- )
    {
        cmd[i++] = *PAR_BUFF++;
    }
    SI446X_CMD( cmd, i );
}
/*
=================================================================================
SI446X_SET_PROPERTY_1();
Function : Set the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
           prioriry,  the value to be set
OUTPUT   : NONE
=================================================================================
*/
void SI446X_SET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM, u8 proirity )
{
    u8 cmd[5];

    cmd[0] = SET_PROPERTY;
    cmd[1] = GROUP_NUM >> 8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    cmd[4] = proirity;
    SI446X_CMD( cmd, 5 );
}
/*
=================================================================================
SI446X_GET_PROPERTY_1();
Function : Get the PROPERTY of the device, only 1 byte
INTPUT   : GROUP_NUM, the group and number index
OUTPUT   : the PROPERTY value read from device
=================================================================================
*/
u8 SI446X_GET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM )
{
    u8 cmd[4];

    cmd[0] = GET_PROPERTY;
    cmd[1] = GROUP_NUM >> 8;
    cmd[2] = 1;
    cmd[3] = GROUP_NUM;
    SI446X_CMD( cmd, 4 );
    SI446X_READ_RESPONSE( cmd, 2 );
    return cmd[1];
}
/*
=================================================================================
SI446X_RESET();
Function : reset the SI446x device
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
void SI446X_RESET( void )
{
    u16 x = 255;
    SI_SDN_HIGH();
    while ( x-- );
    SI_SDN_LOW();
    SI_CSN_HIGH();
}
/*
=================================================================================
SI446X_CONFIG_INIT();
Function : configuration the device
INTPUT   : NONE
OUTPUT   : NONE
=================================================================================
*/
void SI446X_CONFIG_INIT( void )
{
    u8 i;
    u16 j = 0;

    while ( ( i = config_table[j] ) != 0 )
    {
        j += 1;
        SI446X_CMD( config_table + j, i );
        j += i;
    }
#if PACKET_LENGTH > 0           //fixed packet length
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, PACKET_LENGTH );
#else                           //variable packet length
    SI446X_SET_PROPERTY_1( PKT_CONFIG1, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_CRC_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_LEN_FIELD_SOURCE, 0x01 );
    SI446X_SET_PROPERTY_1( PKT_LEN, 0x2A );
    SI446X_SET_PROPERTY_1( PKT_LEN_ADJUST, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_12_8, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_LENGTH_7_0, 0x01 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_1_CRC_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_LENGTH_12_8, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_LENGTH_7_0, 0x10 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_CONFIG, 0x00 );
    SI446X_SET_PROPERTY_1( PKT_FIELD_2_CRC_CONFIG, 0x00 );
#endif //PACKET_LENGTH

    //重要： 4463的GDO2，GDO3控制射频开关，  33 ,32
    //发射时必须： GDO2=0，GDO3=1
    //接收时必须： GDO2=1，GDO3=0
    //所以，以下语句对于4463是必须的，4460或4461不需要射频开关，则可以去掉此语句
    SI446X_GPIO_CONFIG( 0, 0, 33 | 0x40, 32 | 0x40, 0, 0, 0 ); //重要
    //SI446X_GPIO_CONFIG( 0, 0, 0x43, 0x42, 0, 0, 0 );

}
/*
=================================================================================
SI446X_W_TX_FIFO();
Function : write data to TX fifo
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
OUTPUT   : NONE
=================================================================================
*/
void SI446X_W_TX_FIFO( u8 *txbuffer, u8 size )
{
    SI_CSN_LOW();
    SPI_ExchangeByte( WRITE_TX_FIFO );
    while ( size -- )
    {
        SPI_ExchangeByte( *txbuffer++ );
    }
    SI_CSN_HIGH();
}
/*
=================================================================================
SI446X_SEND_PACKET();
Function : send a packet
INTPUT   : txbuffer, a buffer stores TX array
           size,  how many bytes should be written
           channel, tx channel
           condition, tx condition
OUTPUT   : NONE
=================================================================================
*/
void SI446X_SEND_PACKET( u8 *txbuffer, u8 size, u8 channel, u8 condition )
{
    u8 cmd[5];
    u8 tx_len = size;

    SI446X_TX_FIFO_RESET();

    SI_CSN_LOW();
    SPI_ExchangeByte( WRITE_TX_FIFO );
#if PACKET_LENGTH == 0
    tx_len ++;
    SPI_ExchangeByte( size );
#endif
    while ( size -- )
    {
        SPI_ExchangeByte( *txbuffer++ );
    }
    SI_CSN_HIGH();
    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = 0;
    cmd[4] = tx_len;
    SI446X_CMD( cmd, 5 );
}
/*
=================================================================================
SI446X_START_TX();
Function : start TX command
INTPUT   : channel, tx channel
           condition, tx condition
           tx_len, how many bytes to be sent
OUTPUT   : NONE
=================================================================================
*/
void SI446X_START_TX( u8 channel, u8 condition, u16 tx_len )
{
    u8 cmd[5];

    cmd[0] = START_TX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = tx_len >> 8;
    cmd[4] = tx_len;
    SI446X_CMD( cmd, 5 );
}
/*
=================================================================================
SI446X_READ_PACKET();
Function : read RX fifo
INTPUT   : buffer, a buffer to store data read
OUTPUT   : received bytes
=================================================================================
*/
u8 SI446X_READ_PACKET( u8 *buffer )
{
    u8 length, i;
    SI446X_WAIT_CTS();
    SI_CSN_LOW();

    SPI_ExchangeByte( READ_RX_FIFO );
#if PACKET_LENGTH == 0
    length = SPI_ExchangeByte( 0xFF );
#else
    length = PACKET_LENGTH;
#endif
    i = length;

    while ( length -- )
    {
        *buffer++ = SPI_ExchangeByte( 0xFF );
    }
    SI_CSN_HIGH();
    return i;
}
/*
=================================================================================
SI446X_START_RX();
Function : start RX state
INTPUT   : channel, receive channel
           condition, receive condition
           rx_len, how many bytes should be read
           n_state1, next state 1
           n_state2, next state 2
           n_state3, next state 3
OUTPUT   : NONE
=================================================================================
*/
void SI446X_START_RX( u8 channel, u8 condition, u16 rx_len,
                      u8 n_state1, u8 n_state2, u8 n_state3 )
{
    u8 cmd[8];
    SI446X_RX_FIFO_RESET();
    SI446X_TX_FIFO_RESET();
    cmd[0] = START_RX;
    cmd[1] = channel;
    cmd[2] = condition;
    cmd[3] = rx_len >> 8;
    cmd[4] = rx_len;
    cmd[5] = n_state1;
    cmd[6] = n_state2;
    cmd[7] = n_state3;
    SI446X_CMD( cmd, 8 );
}
/*
=================================================================================
SI446X_RX_FIFO_RESET();
Function : reset the RX FIFO of the device
INTPUT   : None
OUTPUT   : NONE
=================================================================================
*/
void SI446X_RX_FIFO_RESET( void )
{
    u8 cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x02;
    SI446X_CMD( cmd, 2 );
}
/*
=================================================================================
SI446X_TX_FIFO_RESET();
Function : reset the TX FIFO of the device
INTPUT   : None
OUTPUT   : NONE
=================================================================================
*/
void SI446X_TX_FIFO_RESET( void )
{
    u8 cmd[2];

    cmd[0] = FIFO_INFO;
    cmd[1] = 0x01;
    SI446X_CMD( cmd, 2 );
}
/*
=================================================================================
SI446X_PKT_INFO();
Function : read packet information
INTPUT   : buffer, stores the read information
           FIELD, feild mask
           length, the packet length
           diff_len, diffrence packet length
OUTPUT   : NONE
=================================================================================
*/
void SI446X_PKT_INFO( u8 *buffer, u8 FIELD, u16 length, u16 diff_len )
{
    u8 cmd[6];
    cmd[0] = PACKET_INFO;
    cmd[1] = FIELD;
    cmd[2] = length >> 8;
    cmd[3] = length;
    cmd[4] = diff_len >> 8;
    cmd[5] = diff_len;

    SI446X_CMD( cmd, 6 );
    SI446X_READ_RESPONSE( buffer, 3 );
}
/*
=================================================================================
SI446X_FIFO_INFO();
Function : read fifo information
INTPUT   : buffer, stores the read information
OUTPUT   : NONE
=================================================================================
*/
void SI446X_FIFO_INFO( u8 *buffer )
{
    u8 cmd[2];
    cmd[0] = FIFO_INFO;
    cmd[1] = 0x03;

    SI446X_CMD( cmd, 2 );
    SI446X_READ_RESPONSE( buffer, 3);
}
/*
=================================================================================
SI446X_GPIO_CONFIG();
Function : config the GPIOs, IRQ, SDO
INTPUT   :
OUTPUT   : NONE
=================================================================================
*/
void SI446X_GPIO_CONFIG( u8 Si664x_G0, u8 Si664x_G1, u8 Si664x_G2, u8 Si664x_G3,
                         u8 IRQ, u8 SDO, u8 GEN_CONFIG )
{
    u8 cmd[10];
    cmd[0] = GPIO_PIN_CFG;
    cmd[1] = Si664x_G0;
    cmd[2] = Si664x_G1;
    cmd[3] = Si664x_G2;
    cmd[4] = Si664x_G3;
    cmd[5] = IRQ;
    cmd[6] = SDO;
    cmd[7] = GEN_CONFIG;
    SI446X_CMD( cmd, 8 );
    SI446X_READ_RESPONSE( cmd, 8 );
}
void SI446X_CHANGE_STATE( u8 NewState )
{
    u8 cmd[2];
    cmd[0] = CHANGE_STATE;
    cmd[1] = NewState;
    SI446X_CMD( cmd, 2 );
}
u8 SI446X_GET_DEVICE_STATE( void )
{
    u8 cmd[3];

    cmd[0] = REQUEST_DEVICE_STATE;
    SI446X_CMD( cmd, 1 );
    SI446X_READ_RESPONSE( cmd, 3 );
    return cmd[1] & 0x0F;
}

void GPIO_Initial( void )
{
    Si446x_CSN_Init
		SI_CSN_HIGH();
    Si446x_SDN_Init
		SI_SDN_HIGH();
    Si446x_GPIO0_Init
    Si446x_GPIO1_Init
    Si446x_TX_1_Init
    Si446x_RX_1_Init
    Si446x_IQR_Init
}

/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
