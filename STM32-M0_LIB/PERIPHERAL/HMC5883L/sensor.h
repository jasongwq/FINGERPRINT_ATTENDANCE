/*********************************Copyright (c)*********************************
**
**
**--------------File Info-------------------------------------------------------
** File Name:               sensor.h
** Last modified Date:
** Last Version:
** Description:             �����ļ�
**
**------------------------------------------------------------------------------
** Created By:              wanxuncpx
** Created date:
** Version:
** Descriptions:
**
*******************************************************************************/

/******************************************************************************
����˵��:

******************************************************************************/

/******************************************************************************
*********************************  Ӧ �� �� �� ********************************
******************************************************************************/


#ifndef _SENSOR_H_
#define _SENSOR_H_

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************
********************************* �ļ����ò��� ********************************
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
********************************** �������� ***********************************
******************************************************************************/
/*---------------------*
*    ����I2C��ַ����   *
*----------------------*/
#ifdef ADXL345_USE_
#define ADXL345_Addr        0xA6            //���ٶȴ�����������ַ
#endif

#ifdef L3G4200_USE_
#define L3G4200_Addr        0xD2            //�����Ǵ�����������ַ
#endif

#ifdef HMC5883L_USE_
#define HMC5883L_Addr       0x3C            //�ų�������������ַ Ĭ�ϣ������� HMC5883LL  7λ�ӻ���ַΪ0x3C��д���������0x3D�Ķ���������  
#endif

#ifdef BMP085_USE_
#define BMP085_Addr        	0xee            //��ѹ������������ַ
#endif

/*---------------------*
*    �ж����ȼ�����    *
*----------------------*/
#ifdef ADXL345_USE_
#define ADXL345_INT_PRIO    5               //��ֵԽС���ȼ�Խ��
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
/******************************* ADX345 �궨�� *********************************/
/*   ADX345�ڲ��Ĵ���   */
//none
/*   ADX345 ��������    */
typedef struct
{
    int16_t ax;
    int16_t ay;
    int16_t az;
} tg_ADXL345_TYPE;
#endif

#ifdef L3G4200_USE_
/***************************** L3G4200D �궨�� *********************************/
/*  L3G4200D�ڲ��Ĵ���  */
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

/*  L3G4200D ��������   */
typedef struct
{
    int16_t gx;
    int16_t gy;
    int16_t gz;
} tg_L3G4200D_TYPE;
#endif

#ifdef HMC5883L_USE_
/***************************** HMC5883L �궨�� *********************************/
/*  HMC5883L�ڲ��Ĵ���  */
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
#define HMC5883L_IRA    0x0a    //�����к�ʹ�õļĴ���
#define HMC5883L_IRB    0x0b
#define HMC5883L_IRC    0x0c

/*   HMC5883 ��������   */
typedef struct
{
    int16_t  hx;
    int16_t  hy;
    int16_t  hz;
    uint16_t ha;
} tg_HMC5883L_TYPE;
extern tg_HMC5883L_TYPE hmc5883l;


/*   HMC5883 У������   */
// Ư��ϵ������λ��1��λ�شų�ǿ��
#define HMC5883L_OFFSET_X  (69)
#define HMC5883L_OFFSET_Y  (314)

//��������
//#define HMC5883L_GAIN_X     1f
//#define HMC5883L_GAIN_Y   1.04034582
#define HMC5883L_GAIN_Y     12275//10000//10403     //ʵ��1.04034582,����������� 
#endif

#ifdef BMP085_USE_
/****************************** BMP085 �궨�� **********************************/
/*   BMP085 �ڲ��Ĵ���  */
//none

/*   BMP085 ��������    */
typedef struct
{
    int32_t temp;
    int32_t pressure;
    float   altitude;
} tg_BMP085_TYPE;

/*   BMP085 ����        */
#define P0_PRESSURE 101325.0f    //��׼����pa
#endif

/********************************* �ں�����   **********************************/
/*---------------------*
*   �ں� ��������      *
*----------------------*/
typedef struct          //ŷ���Ǳ�ʾ����
{
    int16_t  yaw;       //
    int16_t  pitch;
    int16_t  roll;
} tg_AHRS_TYPE;


/******************************************************************************
******************************* �Զ���������� ********************************
******************************************************************************/
//none

/******************************************************************************
******************************** �������Ͷ��� *********************************
******************************************************************************/
//none

/******************************************************************************
********************************* �� �� �� �� *********************************
******************************************************************************/
/*---------------------*
*    IMPORT:�����ṩ   *
*----------------------*/
//none

/*---------------------*
*    EXPORT:�����ṩ   *
*----------------------*/
//none

/******************************************************************************
********************************* �� �� �� �� *********************************
******************************************************************************/
/*---------------------*
*    IMPORT:�����ṩ   *
*----------------------*/
//none

/*---------------------*
*    EXPORT:�����ṩ   *
*----------------------*/
//��ʼ��������
extern void Sensor_GPIO_Init(void);     //������GPIO�ڳ�ʼ��
extern void ADXL345_Init(void);
extern void L3G4200D_Init(void);
extern void HMC5883L_Init(void);
extern void BMP085_Init(void);

//��ȡ����������
//extern void ADXL345_Read(tg_ADXL345_TYPE* ptResult);
//extern void ADXL345_MultRead(tg_ADXL345_TYPE* ptResult);

//extern void L3G4200D_Read(tg_L3G4200D_TYPE* ptResult);
//extern void L3G4200D_MultRead(tg_L3G4200D_TYPE* ptResult);

extern void HMC5883L_Read(tg_HMC5883L_TYPE* ptResult);      //��ͨ��������(������20msʱ����ʱ)
extern void HMC5883L_Start(void);                           //����-�ж�-��ȡ (����)��10ms��ʱ
extern void HMC5883L_MultRead(tg_HMC5883L_TYPE* ptResult);  //����-�ж�-��ȡ (��ȡ)

//extern void BMP085_Read(tg_BMP085_TYPE* ptResult);          //��ͨ�����Ĳ����¶���ѹ(������10msʱ����ʱ)

////У׼������
//extern void ADXL345_Calibrate(void);
extern void HMC5883L_Calibrate(void);
//extern void BMP085_Calibrate(void);                         //��ҪУ׼��ѹ��Ӧ�߶���Ϣ

////��ӡ����������
//extern void ADXL345_Printf(tg_ADXL345_TYPE* ptResult);
//extern void L3G4200D_Printf(tg_L3G4200D_TYPE* ptResult);
extern void HMC5883L_Printf(tg_HMC5883L_TYPE* ptResult);
//extern void BMP085_Printf(tg_BMP085_TYPE* ptResult);

/*************************************************************
************************* �� �� �� �� ************************
*************************************************************/
//none

/******************************************************************************
***********************************   END  ************************************
******************************************************************************/
#ifdef __cplusplus
}
#endif

#endif



