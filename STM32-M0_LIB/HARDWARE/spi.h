/***************************************************************/
#include "stm32f0xx.h"
/***************************************************************/
#ifndef __SPI_H
#define __SPI_H 
#include "gpio.h"

/***************************************************************/
/********************  ??SPI  ***************************************/
#if 1
#define SPI0_SCK_Init   PA7_OUT
#define Set_SPI0_SCK    Set_A7
#define Clr_SPI0_SCK    Clr_A7
#define SPI0_MISO_Init  PA0_In
#define MISO_SPI0       A0
#define SPI0_MOSI_Init  PA6_OUT
#define Set_SPI0_MOSI   Set_A6
#define Clr_SPI0_MOSI   Clr_A6
#else 
#define SPI0_SCK_Init   PA5_OUT
#define Set_SPI0_SCK    Set_A5
#define Clr_SPI0_SCK    Clr_A5
#define SPI0_MISO_Init  PA6_In
#define MISO_SPI0       A6
#define SPI0_MOSI_Init  PA7_OUT
#define Set_SPI0_MOSI   Set_A7
#define Clr_SPI0_MOSI   Clr_A7
#endif 
/********************  ??SPI  ***************************************/
//SPI3 B3 B4 B5 ±ªJTAG ’º”√
u8 SPI1_ReadWriteByte(u8 TxData);

void SPI1_Configuration(void);

void SPI0_Init(void);
u8   SPI0_ReadWriteByte(u8 TxData);





#endif


