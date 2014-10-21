#ifndef __STM32_CONFIG_
#define __STM32_CONFIG_
//#include "stm32_config.h"

/***I2C CONGIG ***/
#define RCC_I2C_SCL RCC_APB2Periph_GPIOD
#define I2C_SCL_GPIO GPIOD
#define I2C_SCL_PIN GPIO_Pin_6

#define RCC_I2C_SDA RCC_APB2Periph_GPIOD
#define I2C_SDA_GPIO GPIOD
#define I2C_SDA_PIN GPIO_Pin_3
/***I2C CONGIG ***/

///***HMC5883L   I2c ***/
//#define Single_Write I2c_Write
//#define Delayms delay_ms
/***HMC5883L***/

///***Ultrasonic***/
////启动 检测
//#define RCC_ULTRASONIC_TRIG RCC_APB2Periph_GPIOE
//#define ULTRASONIC_TRIG_GPIO    GPIOE
//#define ULTRASONIC_TRIG_PIN     GPIO_Pin_0
////返回 信号
//#define RCC_ULTRASONIC_ECHO RCC_APB2Periph_GPIOB
//#define ULTRASONIC_ECHO_GPIO    GPIOB
//#define ULTRASONIC_ECHO_PIN     GPIO_Pin_8

/***Ultrasonic***/
/***USART配置***/
#define DEBUG_USART USART2
#define EN_USART_           1       //使能（1）/禁止（0）串口1
#define EN_USART2_          1       //使能（1）/禁止（0）串口2
#define EN_USART3_          0       //使能（1）/禁止（0）串口3
/***USART配置***/

/*** LCD ***/
////RST
//#define RCC_LCD_RST 	  RCC_APB2Periph_GPIOD
//#define LCD_RST_GPIO   	GPIOD
//#define LCD_RST_Pin 	  GPIO_Pin_12
////LIGHT
//#define RCC_LCD_LIGHT 	RCC_APB2Periph_GPIOC
//#define LCD_LIGHT_GPIO 	GPIOC
//#define LCD_LIGHT_Pin 	GPIO_Pin_0
////RD
//#define RCC_LCD_RD	RCC_APB2Periph_GPIOD
//#define LCD_RD_GPIO GPIOD
//#define LCD_RD_Pin 	GPIO_Pin_4
////WR
//#define RCC_LCD_WR	RCC_APB2Periph_GPIOD
//#define LCD_WR_GPIO GPIOD
//#define LCD_WR_Pin 	GPIO_Pin_5
////CS
//#define RCC_LCD_CS	RCC_APB2Periph_GPIOD
//#define LCD_CS_GPIO GPIOD
//#define LCD_CS_Pin 	GPIO_Pin_7
////RS
//#define RCC_LCD_RS	RCC_APB2Periph_GPIOD
//#define LCD_RS_GPIO GPIOD
//#define LCD_RS_Pin 	GPIO_Pin_11
/*** LCD ***/
/***TOUCH***/
//#define LCD_TOUCH_SPI_SOFT

//#define RCC_LCD_TOUCH_PEN	 RCC_APB2Periph_GPIOA
//#define LCD_TOUCH_PEN_GPIO GPIOA
//#define LCD_TOUCH_PEN_Pin  GPIO_Pin_4

//#define RCC_LCD_TOUCH_TCS	 RCC_APB2Periph_GPIOC
//#define LCD_TOUCH_TCS_GPIO GPIOC
//#define LCD_TOUCH_TCS_Pin  GPIO_Pin_4

/***TOUCH***/
/***MPU6050***/
//#define MPU6050_USE_DMP
/***MPU6050***/
/****************************************/
/***NRF24L01***/
/********************************************
***********************************************\
 * _________________ *
 * |8 IQR    7 MISO| *A4 A6
 * |               | *
 * |6 MOSI   5 SCK | *A7 A5
 * |               | *
 * |4 CSN    3 CE  | *A3 A2
 * |       --------| *
 * |2 VCC  | 1 GND | *
 * ----------------- *
\**********************************************/
/***24L01配置***/
/*使用硬件SPI 1 or 2*/
//#define NRF24L01_SPI1
//#define NRF24L01_SPI2
/*使用软件SPI*/
#define NRF24L01_SPISOFT



/***PWM配置***/
/*****************************************************
REMAP        |   00    |   01    |   10    |   11    |
*****************************************************|
TIM2_CH1_ETR |   PA0   |PA15(JTDI)| (PA0)  |(PA15)   |
TIM2_CH2     |   PA1   |PB3(JTDO) | (PA1)  |(PB3 )   |
TIM2_CH3     |        PA2        |    (PB10)(!36)    |
TIM2_CH4     |        PA3        |    (PB11)(!36)    |
*****************************************************|
TIM3_CH1     |   PA6   |PB4(JNTRST)|       |(PC6)(64 100 144)|
TIM3_CH2     |   PA7   |   PB5   |         |(PC7)    |
TIM3_CH3     |        PB0        |         |(PC8)    |
TIM3_CH4     |        PB1        |         |(PC9)    |
*****************************************************|
TIM4_CH1     |   PB6   |(PD12)(100 144)|   |         |
TIM4_CH2     |   PB7   |(PD13)(100 144)|   |         |
TIM4_CH3     |   PB8   |(PD14)(100 144)|   |         |
TIM4_CH4     |   PB9   |(PD15)(100 144)|   |         |
*****************************************************|
TIM5_CH1     |   PA0   |                             |
TIM5_CH2     |   PA1   |                             |
TIM5_CH3     |   PA2   |                             |
TIM5_CH4     |   PA3   |                             |
*****************************************************|
TIM8_ETR     |   PA0   |                             |
TIM8_CH1     |   PC6   |                             |
TIM8_CH2     |   PC7   |                             |
TIM8_CH3     |   PC8   |                             |
TIM8_CH4     |   PC9   |                             |
TIM8_BKIN    |   PA6   |                             |
TIM8_CH1N    |   PA7   |                             |
TIM8_CH2N    |   PB0   |                             |
TIM8_CH3N    |   PB1   |                             |
*****************************************************|
TIM1_ETR     |        PA12       |   PE7 (100 144)   |
TIM1_CH1     |        PA8        |   PE9 (100 144)   |
TIM1_CH2     |        PA9 (Tx)   |   PE11(100 144)   |
TIM1_CH3     |        PA10(Rx)   |   PE13(100 144)   |
TIM1_CH4     |        PA11(CANRx)|   PE14(100 144)   |
TIM1_BKIN    |PB12(!36)|   PA6   |   PE15(100 144)   |
TIM1_CH1N    |PB13(!36)|   PA7   |   PE8 (100 144)   |
TIM1_CH2N    |PB14(!36)|   PB0   |   PE10(100 144)   |
TIM1_CH3N    |PB15(!36)|   PB1   |   PE12(100 144)   |
*****************************************************/
//#define PWM_TIM2_AFIO 0 //(0-3)
////(0-3 0:ok 1:ok 2:ok 3:ok )
//#define PWM_TIM2_CH1 //ok B3 JTDO
//#define PWM_TIM2_CH2 //ok A15JTDI
//#define PWM_TIM2_CH3 //ok
//#define PWM_TIM2_CH4 //ok

//#define PWM_TIM3_AFIO 0 //(0-2)
////(0:ok 1:ok 2:ok)
//#define PWM_TIM3_CH1 //ok B4 JNTRST
//#define PWM_TIM3_CH2 //ok 
//#define PWM_TIM3_CH3 //ok
//#define PWM_TIM3_CH4 //ok

//#define PWM_TIM4_AFIO 0 //(0-1)
//// 0:ok 1:ok)
//#define PWM_TIM4_CH1 //ok
//#define PWM_TIM4_CH2 //ok
//#define PWM_TIM4_CH3 //ok
//#define PWM_TIM4_CH4 //ok

//#define PWM_TIM5_CH1 //ok
//#define PWM_TIM5_CH2 //ok
//#define PWM_TIM5_CH3 //ok
//#define PWM_TIM5_CH4 //ok

//#define PWM_TIM1_AFIO 0 //(0-2)
////(0:ok 1:ok 2:ok)
//#define PWM_TIM1_CH1 //ok
//#define PWM_TIM1_CH2 //ok
//#define PWM_TIM1_CH3 //ok
//#define PWM_TIM1_CH4 //ok

//#define PWM_TIM8_CH1 //ok
//#define PWM_TIM8_CH2 //ok
//#define PWM_TIM8_CH3 //ok
//#define PWM_TIM8_CH4 //ok
/***PWM配置***/
#endif

