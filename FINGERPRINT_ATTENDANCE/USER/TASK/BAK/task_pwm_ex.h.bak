
#include "data_transfer.h"
#include "control.h"
#include "pwm.h"
#define EX_BaudRate 2
int task_pwm_ex(void)
{
    _SS
    TIM2_PWM_Init(5000, 36);
    TIM2_PWM_Init(5000, 36);
    TIM8_PWM_Init(5000, 36);
    TIM8_PWM_Init(5000, 36);
    TIM_SetCompare1(TIM2, 2300);
    TIM_SetCompare2(TIM2, 2300);
    TIM_SetCompare1(TIM8, 2300);
    TIM_SetCompare2(TIM8, 2300);
    while (1)
    {
        if (Ex_ON_OFF.Status)
        {
            WaitX(EX_BaudRate);
            Send.Status = 1;
            Data_Exchange();
        }
        if (Ex_ON_OFF.Senser)
        {
            WaitX(EX_BaudRate);
            Send.Senser = 1;
            Data_Exchange();
        }
        if (Ex_ON_OFF.RCData)
        {
            WaitX(EX_BaudRate);
            Data_Exchange();
        }
        if (Ex_ON_OFF.MotoPwm)
        {
            WaitX(EX_BaudRate);
            Send.MotoPwm = 1;
            Data_Exchange();
        }
        if (Ex_ON_OFF.DataF4)
        {
            WaitX(EX_BaudRate);
            Send.DataF4 = 1;
            Data_Exchange();
        }
    }
    _EE
}
