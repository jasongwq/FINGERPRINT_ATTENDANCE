#ifndef _SI446X_H_
#define _SI446X_H_

#include "sys.h"
#include "SI446X_defs.h"

/*
=================================================================================
------------------------------INTERNAL EXPORT APIs-------------------------------
=================================================================================
*/
void GPIO_Initial( void );

/*Read the PART_INFO of the device, 8 bytes needed*/
void SI446X_PART_INFO( u8 *buffer );

/*Read the FUNC_INFO of the device, 7 bytes needed*/
void SI446X_FUNC_INFO( u8 *buffer );

/*Send a command to the device*/
void SI446X_CMD(const u8 *cmd, u8 cmdsize );

/*Read the INT status of the device, 9 bytes needed*/
void SI446X_INT_STATUS( u8 *buffer );

/*Read the PROPERTY of the device*/
void SI446X_GET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, u8 NUM_PROPS, u8 *buffer  );

/*configuration the device*/
void SI446X_CONFIG_INIT( void );

/*reset the SI446x device*/
void SI446X_RESET( void );

/*write data to TX fifo*/
void SI446X_W_TX_FIFO( u8 *txbuffer, u8 size );

/*start TX command*/
void SI446X_START_TX( u8 channel, u8 condition, u16 tx_len );

/*read RX fifo*/
u8 SI446X_READ_PACKET( u8 *buffer );

/*start RX state*/
void SI446X_START_RX( u8 channel, u8 condition, u16 rx_len,
                      u8 n_state1, u8 n_state2, u8 n_state3 );

/*read packet information*/
void SI446X_PKT_INFO( u8 *buffer, u8 FIELD, u16 length, u16 diff_len );

/*read fifo information*/
void SI446X_FIFO_INFO( u8 *buffer );

/*Power up the device*/
void SI446X_POWER_UP( u32 f_xtal );

/*send a packet*/
void SI446X_SEND_PACKET( u8 *txbuffer, u8 size, u8 channel, u8 condition );

/*Set the PROPERTY of the device*/
void SI446X_SET_PROPERTY_X( SI446X_PROPERTY GROUP_NUM, u8 NUM_PROPS, u8 *PAR_BUFF );

/*config the CRC, PROPERTY 0x1200*/
void SI446X_CRC_CONFIG( u8 PKT_CRC_CONFIG );

/*Get the PROPERTY of the device, only 1 byte*/
u8 SI446X_GET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM );

/*Set the PROPERTY of the device, only 1 byte*/
void SI446X_SET_PROPERTY_1( SI446X_PROPERTY GROUP_NUM, u8 proirity );

/*config the GPIOs, IRQ, SDO*/
void SI446X_GPIO_CONFIG( u8 Si664x_G0, u8 Si664x_G1, u8 Si664x_G2, u8 Si664x_G3,
                         u8 IRQ, u8 SDO, u8 GEN_CONFIG );

/*reset the RX FIFO of the device*/
void SI446X_RX_FIFO_RESET( void );

/*reset the TX FIFO of the device*/
void SI446X_TX_FIFO_RESET( void );

u8 SI446X_GET_DEVICE_STATE( void );

void SI446X_CHANGE_STATE( u8 NewState );


extern const int  PACKET_LENGTH;


#endif //_SI446X_H_

/*
=================================================================================
------------------------------------End of FILE----------------------------------
=================================================================================
*/
