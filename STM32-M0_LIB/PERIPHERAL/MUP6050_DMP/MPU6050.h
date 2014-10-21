#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "G32_I2c.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "inv_mpu.h"

enum
{
    MPU6050_SUCCEED = 0,
    MPU6050_FAILED = 1,
};


extern float q0,q1,q2,q3;
extern float Pitch, Roll, Yaw;



extern void get_ms(unsigned long *time);
extern char mpu6050_dmp_init(void);
s32 mpu6050_init(void);

extern void mpu6050_getdata(void);
void Clock_Enable(void);

#endif

