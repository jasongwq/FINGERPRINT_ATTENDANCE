/*********************************Copyright (c)*********************************
**
**
**--------------File Info-------------------------------------------------------
** File Name:               sensor.h
** Last modified Date:
** Last Version:
** Description:             配置文件
**
**------------------------------------------------------------------------------
** Created By:              wanxuncpx
** Created date:
** Version:
** Descriptions:
**
*******************************************************************************/

/******************************************************************************
更新说明:

******************************************************************************/

/******************************************************************************
*********************************  应 用 资 料 ********************************
******************************************************************************/


#ifndef _SENSOR_H_
#define _SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
********************************* 文件引用部分 ********************************
******************************************************************************/
//#include "sim_i2c.h"
//#include "ahrs_dbg.h"
//#include <stdio.h>
//#include <stdbool.h>
#include <math.h>    //Keil library 
#include "G32_I2c.h"
#include "USART.h"


//#define ADXL345_USE_
//#define L3G4200_USE_
#define HMC5883L_USE_
//#define BMP085_USE_



/******************************************************************************
********************************** 参数配置 ***********************************
******************************************************************************/
/*---------------------*
*    器件I2C地址定义   *
*----------------------*/
#ifdef ADXL345_USE_
#define ADXL345_Addr        0xA6            //加速度传感器器件地址
#endif

#ifdef L3G4200_USE_
#define L3G4200_Addr        0xD2            //陀螺仪传感器器件地址
#endif

#ifdef HMC5883L_USE_
#define HMC5883L_Addr       0x3C            //磁场传感器器件地址 默认（出厂） HMC5883LL  7位从机地址为0x3C的写入操作，或0x3D的读出操作。  
#endif

#ifdef BMP085_USE_
#define BMP085_Addr        	0xee            //气压传感器器件地址
#endif

/*---------------------*
*    中断优先级分配    *
*----------------------*/
#ifdef ADXL345_USE_
#define ADXL345_INT_PRIO    5               //数值越小优先级越高
#endif
#ifdef L3G4200_USE_
#define L3G4200D_INT_PRIO   5
#endif
#ifdef HMC5883L_USE_
#define HMC5883L_INT_PRIO   5
#endif
#ifdef BMP085_USE_
#define BMP085_INT_PRIO     5
#endif

#ifdef ADXL345_USE_
/******************************* ADX345 宏定义 *********************************/
/*   ADX345内部寄存器   */
//none
/*   ADX345 数据类型    */
typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;
} tg_ADXL345_TYPE;
#endif

#ifdef L3G4200_USE_
/***************************** L3G4200D 宏定义 *********************************/
/*  L3G4200D内部寄存器  */
#define CTRL_REG1           0x20
#define CTRL_REG2           0x21
#define CTRL_REG3           0x22
#define CTRL_REG4           0x23
#define CTRL_REG5           0x24
#define OUT_X_L             0x28
#define OUT_X_H             0x29
#define OUT_Y_L             0x2A
#define OUT_Y_H             0x2B
#define OUT_Z_L             0x2C
#define OUT_Z_H             0x2D

/*  L3G4200D 数据类型   */
typedef struct
{
    int16_t gx;
    int16_t gy;
    int16_t gz;
} tg_L3G4200D_TYPE;
#endif

#ifdef HMC5883L_USE_
/***************************** HMC5883L 宏定义 *********************************/
/*  HMC5883L内部寄存器  */
#define HMC5883L_REGA   0x00
#define HMC5883L_REGB   0x01
#define HMC5883L_MODE   0x02
#define HMC5883L_HX_H   0x03
#define HMC5883L_HX_L   0x04
#define HMC5883L_HZ_H   0x05
#define HMC5883L_HZ_L   0x06
#define HMC5883L_HY_H   0x07
#define HMC5883L_HY_L   0x08
#define HMC5883L_STATE  0x09
#define HMC5883L_IRA    0x0a    //读序列号使用的寄存器
#define HMC5883L_IRB    0x0b
#define HMC5883L_IRC    0x0c

/*   HMC5883 数据类型   */
typedef struct
{
    int16_t  hx;
    int16_t  hy;
    int16_t  hz;
    uint16_t ha;
} tg_HMC5883L_TYPE;
extern tg_HMC5883L_TYPE hmc5883l;


/*   HMC5883 校正参数   */
// 漂移系数。单位：1单位地磁场强度
#define HMC5883L_OFFSET_X  (69)
#define HMC5883L_OFFSET_Y  (314)

//比例因子
//#define HMC5883L_GAIN_X     1f
//#define HMC5883L_GAIN_Y   1.04034582
#define HMC5883L_GAIN_Y     12275//10000//10403     //实际1.04034582,这里便于整除 
#endif

#ifdef BMP085_USE_
/****************************** BMP085 宏定义 **********************************/
/*   BMP085 内部寄存器  */
//none

/*   BMP085 数据类型    */
typedef struct
{
    int32_t temp;
    int32_t pressure;
    float   altitude;
} tg_BMP085_TYPE;

/*   BMP085 参数        */
#define P0_PRESSURE 101325.0f    //标准海拔pa
#endif

/********************************* 融合数据   **********************************/
/*---------------------*
*   融合 数据类型      *
*----------------------*/
typedef struct          //欧拉角表示方法
{
    int16_t  yaw;       //
    int16_t  pitch;
    int16_t  roll;
} tg_AHRS_TYPE;


/******************************************************************************
******************************* 自定义参数配置 ********************************
******************************************************************************/
//none

/******************************************************************************
******************************** 数据类型定义 *********************************
******************************************************************************/
//none

/******************************************************************************
********************************* 数 据 声 明 *********************************
******************************************************************************/
/*---------------------*
*    IMPORT:由外提供   *
*----------------------*/
//none

/*---------------------*
*    EXPORT:向外提供   *
*----------------------*/
//none

/******************************************************************************
********************************* 函 数 声 明 *********************************
******************************************************************************/
/*---------------------*
*    IMPORT:由外提供   *
*----------------------*/
//none

/*---------------------*
*    EXPORT:向外提供   *
*----------------------*/
//初始化传感器
extern void Sensor_GPIO_Init(void);     //传感器GPIO口初始化
extern void ADXL345_Init(void);
extern void L3G4200D_Init(void);
extern void HMC5883L_Init(void);
extern void BMP085_Init(void);

//读取传感器数据
//extern void ADXL345_Read(tg_ADXL345_TYPE* ptResult);
//extern void ADXL345_MultRead(tg_ADXL345_TYPE* ptResult);

//extern void L3G4200D_Read(tg_L3G4200D_TYPE* ptResult);
//extern void L3G4200D_MultRead(tg_L3G4200D_TYPE* ptResult);

extern void HMC5883L_Read(tg_HMC5883L_TYPE* ptResult);      //普通完整测量(有至少20ms时序延时)
extern void HMC5883L_Start(void);                           //启动-中断-读取 (启动)有10ms延时
extern void HMC5883L_MultRead(tg_HMC5883L_TYPE* ptResult);  //启动-中断-读取 (读取)

//extern void BMP085_Read(tg_BMP085_TYPE* ptResult);          //普通完整的测量温度气压(有至少10ms时序延时)

////校准传感器
//extern void ADXL345_Calibrate(void);
extern void HMC5883L_Calibrate(void);
//extern void BMP085_Calibrate(void);                         //主要校准气压对应高度信息

////打印传感器数据
//extern void ADXL345_Printf(tg_ADXL345_TYPE* ptResult);
//extern void L3G4200D_Printf(tg_L3G4200D_TYPE* ptResult);
extern void HMC5883L_Printf(tg_HMC5883L_TYPE* ptResult);
//extern void BMP085_Printf(tg_BMP085_TYPE* ptResult);

/*************************************************************
************************* 其 它 杂 项 ************************
*************************************************************/
//none

/******************************************************************************
***********************************   END  ************************************
******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif



