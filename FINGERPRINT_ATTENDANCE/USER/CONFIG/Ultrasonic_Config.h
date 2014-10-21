#ifndef _ULTRASONIC_CONFIG_H_
#define _ULTRASONIC_CONFIG_H_
///***Ultrasonic***/
//启动 检测
#define RCC_ULTRASONIC_TRIG     RCC_APB2Periph_GPIOA
#define ULTRASONIC_TRIG_GPIO    GPIOA
#define ULTRASONIC_TRIG_PIN     GPIO_Pin_11
//返回 信号
#define RCC_ULTRASONIC_ECHO     RCC_APB2Periph_GPIOA
#define ULTRASONIC_ECHO_GPIO    GPIOA
#define ULTRASONIC_ECHO_PIN     GPIO_Pin_8

#endif


