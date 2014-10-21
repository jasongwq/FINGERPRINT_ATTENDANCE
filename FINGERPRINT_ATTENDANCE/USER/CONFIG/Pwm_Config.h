#ifndef __PWM_CONFIG_
#define __PWM_CONFIG_

/****************************************/
/***PWM***/
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
/**
 * (1)启用 (0)不启用
 * AFIO 0-3
 */
#define PWM_TIM2_CH1  1
#define PWM_TIM2_CH2  1
#define PWM_TIM2_CH3  0
#define PWM_TIM2_CH4  0
#define PWM_TIM2_AFIO 0

#define PWM_TIM3_CH1  1
#define PWM_TIM3_CH2  1
#define PWM_TIM3_CH3  0
#define PWM_TIM3_CH4  0
#define PWM_TIM3_AFIO 0

#define PWM_TIM4_CH1  0
#define PWM_TIM4_CH2  0
#define PWM_TIM4_CH3  0
#define PWM_TIM4_CH4  0
#define PWM_TIM4_AFIO 0

#define PWM_TIM5_CH1  0
#define PWM_TIM5_CH2  0
#define PWM_TIM5_CH3  0
#define PWM_TIM5_CH4  0
#define PWM_TIM5_AFIO 0

#define PWM_TIM1_CH1  0
#define PWM_TIM1_CH2  0
#define PWM_TIM1_CH3  0
#define PWM_TIM1_CH4  0
#define PWM_TIM1_AFIO 0

#define PWM_TIM8_CH1  1
#define PWM_TIM8_CH2  1
#define PWM_TIM8_CH3  0
#define PWM_TIM8_CH4  0
#define PWM_TIM8_AFIO 0
/***PWM***/
/****************************************/

#endif
