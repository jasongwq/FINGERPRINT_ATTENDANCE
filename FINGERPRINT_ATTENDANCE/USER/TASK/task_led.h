#define LED1_Init PB1_OUT
#define LED1_H Set_B1
#define LED1_L Clr_B1
#define LED1_Toggle {static char sta = 0;if(sta++&0X01){LED1_H;}else{LED1_L;}}
int task_led(void)
{
    _SS
    LED1_Init;
    LED1_L;
    while (1)
    {   //static int ledruntime = 0;
        if (LED_STA == LED_RUN)
        {
            WaitX(50);
            LED1_Toggle;
        }
        else if (LED_STA == LED_TXD)
        {
            WaitX(10);
            LED1_H;
        }
        else if (LED_STA == LED_RXD)
        {
            WaitX(10);
            LED1_H;
        }
				else if (LED_STA == LED_START)//||start<101)
				{
            ledruntime=20;
        }
        else if (LED_STA == LED_NULL)
        {
            WaitX(10);
            LED1_L;
						//ledruntime=0;
        }
				if(ledruntime)
				{
				WaitX(10);
        LED1_Toggle;
        ledruntime--;
				}
				WaitX(10);
    }
    _EE
}
//int loop_led(void)
//{
//    _SS
//    LED1_Init;
//    LED1_H;
//    _LOOP_SS
//    if (RC_Control.ARMED)
//        LED1_H;
//    else
//        LED1_Toggle;
//    LoopX(500);
//    _EE
//}
