#include "mpu6050.h"
#include "sys.h"
#include "math.h"
#include "usart.h"
#include "lcd.h"
#include <app/param/interface.h>
/***********************************************************************************/
#define MPU6050_ADDRESS 0xD0
#define MPU6050_REG_RAW_DATA 0x3B
#define MPU6050_REG_ACC 0x3B
#define MPU6050_REG_TEM 0x41
#define MPU6050_REG_GYR 0x43
//#define int16_t s16
//#define uint8_t u8

static struct
{
    int16_t acc[3];
    int16_t tem;
    int16_t gyr[3];
}mpu6050_rawData;

    // 设置MPU6050的参数。
    const uint8_t param[][2] = 
    {
        // {寄存器地址,寄存器值},
        {0x6B,1     }, // 退出睡眠模式，设取样时钟为陀螺X轴。
        {0x19,4     }, // 取样时钟4分频，1k/4，取样率为25Hz。
        {0x1A,2     }, // 低通滤波，截止频率100Hz左右。
        {0x1B,3<<3  }, // 陀螺量程，2000dps。
        {0x1C,2<<3  }, // 加速度计量程，8g。
        {0x37,0x32  }, // 中断信号为高电平，推挽输出，直到有读取操作才消失，直通辅助I2C。
        {0x38,1     }, // 使用“数据准备好”中断。
        {0x6A,0x00  }, // 不使用辅助I2C。
    };

#define i2c_read(dev,reg,data,len) (!i2cread((dev)>>1,reg,len,(uint8_t *)data))
#define i2c_write(dev,reg,data,len) (!i2cwrite((dev)>>1,reg,len,(uint8_t *)data))

s32 mpu6050_init(void)
{
	int i;
	uint8_t id = 0;
	uint8_t check;
    for(i=0;i<sizeof(mpu6050_rawData);i++)
        ((uint8_t *)&mpu6050_rawData)[i] = 0;
    // 检查ID。
    if(i2c_read(MPU6050_ADDRESS,0x75,&id,1) != 0)
        return MPU6050_FAILED;
    if(id != 0x68)
        return MPU6050_FAILED;

    for(i=0;i<sizeof(param)/2;i++)
    {
        if(i2c_write(MPU6050_ADDRESS,param[i][0],&(param[i][1]),1) != 0)
            return MPU6050_FAILED;
        check = 0;
        if(i2c_read(MPU6050_ADDRESS,param[i][0],&check,1) != 0)
            return MPU6050_FAILED;
        if(check != param[i][1])
            return MPU6050_FAILED;
    }
    return MPU6050_SUCCEED;
}
int32_t mpu6050_read(void)
{
	int i;
    if(i2c_read(MPU6050_ADDRESS,MPU6050_REG_RAW_DATA,&mpu6050_rawData
        ,sizeof(mpu6050_rawData)) != 0)
        return MPU6050_FAILED;
    //
    // 大端转小端。
    uint8_t * p = (uint8_t *)&mpu6050_rawData;
    for(i=0;i<sizeof(mpu6050_rawData)/2;i++)
    {
        uint8_t tmp = p[2*i];
        p[2*i] = p[2*i + 1];
        p[2*i + 1] = tmp;
    }
    //
    return MPU6050_SUCCEED;
}

// acc : LSB
int32_t mpu6050_getRawAcc(int16_t acc[3])
{
    for(int i=0;i<3;i++)
        acc[i] = mpu6050_rawData.acc[i];
    //
    return MPU6050_SUCCEED;
}

//gyr : LSB
int32_t mpu6050_getRawGyr(int16_t gyr[3])
{
    for(int i=0;i<3;i++)
        gyr[i] = mpu6050_rawData.gyr[i];
    //
    return MPU6050_SUCCEED;
}

// acc : m/s2
int32_t mpu6050_getCalibratedAcc(float acc[3])
{
    const param_t *p = param_getRamParam();
    //
    for(int i=0;i<3;i++)
        acc[i] = (p->acc_offset[i] + mpu6050_rawData.acc[i]) * p->acc_gain[i];
    //
    return MPU6050_SUCCEED;
}

// gyr : rad/s
int32_t mpu6050_getCalibratedGyr(float gyr[3])
{
    const param_t *p = param_getRamParam();
    //
    for(int i=0;i<3;i++)
        gyr[i] = (p->gyr_offset[i] + mpu6050_rawData.gyr[i]) * p->gyr_gain[i];
    //
    return MPU6050_SUCCEED;
}

/***********************************************************************************/
#define q30  1073741824.0f
#define DEFAULT_MPU_HZ  (100)

float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
void get_ms(unsigned long* time)
{
}
struct rx_s
{
    unsigned char header[3];
    unsigned char cmd;
};
struct hal_s
{
    unsigned char sensors;
    unsigned char dmp_on;
    unsigned char wait_for_tap;
    volatile unsigned char new_gyro;
    unsigned short report;
    unsigned short dmp_features;
    unsigned char motion_int_mode;
    struct rx_s rx;
};
static struct hal_s hal = {0};
volatile unsigned char rx_new;
static signed char gyro_orientation[9] = { -1, 0, 0,
                                           0, -1, 0,
                                           0, 0, 1
                                         };

enum packet_type_e
{
    PACKET_TYPE_ACCEL,
    PACKET_TYPE_GYRO,
    PACKET_TYPE_QUAT,
    PACKET_TYPE_TAP,
    PACKET_TYPE_ANDROID_ORIENT,
    PACKET_TYPE_PEDO,
    PACKET_TYPE_MISC
};

static  unsigned short inv_row_2_scale(const signed char* row)
{
    unsigned short b;
    if (row[0] > 0)b = 0;
    else if (row[0] < 0)b = 4;
    else if (row[1] > 0)b = 1;
    else if (row[1] < 0)b = 5;
    else if (row[2] > 0)b = 2;
    else if (row[2] < 0)b = 6;
    else b = 7;      // error
    return b;
}

static  unsigned short inv_orientation_matrix_to_scalar(
    const signed char* mtx)
{
    unsigned short scalar;

    scalar = inv_row_2_scale(mtx);
    scalar |= inv_row_2_scale(mtx + 3) << 3;
    scalar |= inv_row_2_scale(mtx + 6) << 6;
    return scalar;
}


/*自测*/
static void run_self_test(void)
{
    int result;
    long gyro[3], accel[3];

    result = mpu_run_self_test(gyro, accel);
    if (result & 0x1)
    {
        float sens;
        unsigned short accel_sens;
        mpu_get_gyro_sens(&sens);
        gyro[0] = (long)(gyro[0] * sens);
        gyro[1] = (long)(gyro[1] * sens);
        gyro[2] = (long)(gyro[2] * sens);
        dmp_set_gyro_bias(gyro);
        mpu_get_accel_sens(&accel_sens);
        accel[0] *= accel_sens;
        accel[1] *= accel_sens;
        accel[2] *= accel_sens;
        dmp_set_accel_bias(accel);
        USART_OUT(DEBUG_USART,"setting bias succesfully ......\n");
    }
    else
    {
        USART_OUT(DEBUG_USART,"bias has not been modified ......\n");
    }
}

void Clock_Enable(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                           RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}


char mpu6050_dmp_init(void)
{
    int result;
    result = mpu_init();/*mpu初始化*/
    //USART_OUT(DEBUG_USART,"1.mpu initialization complete......%d\r\n ",result);
    if (!result)
    {
        //1.
        USART_OUT(DEBUG_USART, "1.mpu initialization complete......\n ");
        //2.
        if (!mpu_set_sensors(INV_XYZ_GYRO | INV_XYZ_ACCEL)) /*设置传感器 加速度|陀螺仪*/
            USART_OUT(DEBUG_USART, "2.mpu_set_sensor complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "mpu_set_sensor come across error ......\r\n");
        //3.//mpu_configure_fifo
        if (!mpu_configure_fifo(INV_XYZ_GYRO | INV_XYZ_ACCEL))
            USART_OUT(DEBUG_USART, "3.mpu_configure_fifo complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "mpu_configure_fifo come across error ......\r\n");
        //4.//mpu_set_sample_rate
        if (!mpu_set_sample_rate(DEFAULT_MPU_HZ)) /*采样频率*/
            USART_OUT(DEBUG_USART, "4.mpu_set_sample_rate complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "mpu_set_sample_rate error ......\r\n");
        //5.//dmp_load_motion_driver_firmvare
        if (!dmp_load_motion_driver_firmware()) /*加载DMP*/
            USART_OUT(DEBUG_USART, "5.dmp_load_motion_driver_firmware complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "dmp_load_motion_driver_firmware come across error ......\r\n");
        //6.//dmp_set_orientation
        if (!dmp_set_orientation(inv_orientation_matrix_to_scalar(gyro_orientation))) /*方向*/
            USART_OUT(DEBUG_USART, "6.dmp_set_orientation complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "dmp_set_orientation come across error ......\r\n");
        //7.//dmp_enable_feature
        if (!dmp_enable_feature(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_TAP |
                                DMP_FEATURE_ANDROID_ORIENT | DMP_FEATURE_SEND_RAW_ACCEL | DMP_FEATURE_SEND_CAL_GYRO |
                                DMP_FEATURE_GYRO_CAL))
            USART_OUT(DEBUG_USART, "7.dmp_enable_feature complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "dmp_enable_feature come across error ......\r\n");
        //8.//dmp_set_fifo_rate
        if (!dmp_set_fifo_rate(DEFAULT_MPU_HZ))
            USART_OUT(DEBUG_USART, "8.dmp_set_fifo_rate complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "dmp_set_fifo_rate come across error ......\r\n");

        if (!mpu_set_dmp_state(1))
            USART_OUT(DEBUG_USART, "9.mpu_set_dmp_state complete ......\r\n");
        else
            USART_OUT(DEBUG_USART, "mpu_set_dmp_state come across error ......\r\n");
        run_self_test();
    }
    //USART_OUT(DEBUG_USART,"1.mpu initialization complete......%d\r\n ",result);
return 0;
}
//float angle;
float Pitch, Roll, Yaw;
short gyro[3], accel[3], sensors;
void mpu6050_getdata(void)
{
    unsigned long sensor_timestamp;
    unsigned char more;

    long quat[4];
    dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,
                  &more);
    if (sensors & INV_WXYZ_QUAT)
    {
        q0 = quat[0] / q30;
        q1 = quat[1] / q30;
        q2 = quat[2] / q30;
        q3 = quat[3] / q30;
        Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;
        Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2 * q2 + 1) * 57.3;
        Yaw   = atan2(2 * (q1 * q2 + q0 * q3), q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 57.3;
    }
}

