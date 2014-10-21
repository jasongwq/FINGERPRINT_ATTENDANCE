

#include "G32_Timer_Cap.h"
extern int RC_flag_ok;
extern u8 RC_data_cnt;
extern u8 *p_RxBuffer;
extern void Data_Receive_Anl(u8 *data_buf, u8 num);

//int task_cap_rc(void)
//{
//    static u16 Rc_Pwm_In[10];
//    u32 temp = 0;
//    static u16 capok;
//    _SS
//    TIM2_Cap_Init(0XFFFF, 72);
//    TIM3_Cap_Init(0XFFFF, 72);
//    TIM4_Cap_Init(0XFFFF, 72);
//    TIM8_Cap_Init(0XFFFF, 72);
//    while (1)
//    {
//        static u8 utime;
//        WaitX(25 - utime * 2);
//        TIM4_CAPTURE_STA_CH1 = 0;
//        TIM4_CAPTURE_STA_CH2 = 0;
//        TIM3_CAPTURE_STA_CH1 = 0;
//        TIM3_CAPTURE_STA_CH2 = 0;
//        TIM3_CAPTURE_STA_CH3 = 0;
//        TIM3_CAPTURE_STA_CH4 = 0;
//        TIM8_CAPTURE_STA_CH1 = 0;
//        TIM8_CAPTURE_STA_CH2 = 0;
//        TIM2_CAPTURE_STA_CH1 = 0;
//        TIM2_CAPTURE_STA_CH2 = 0;
//        capok = 0;
//        utime = 0;
//        while (1)
//        {
//            WaitX(2);
//            if (TIM4_CAPTURE_STA_CH1 & 0X80)
//            {
//                temp  = TIM4_CAPTURE_STA_CH1 & 0X3F;
//                temp *= 65536;
//                temp += TIM4_CAPTURE_VAL_CH1;
//                Rc_Pwm_In[0] = temp;
//                capok |= (1 << 0);
//            }
//            if (TIM4_CAPTURE_STA_CH2 & 0X80)
//            {
//                temp  = TIM4_CAPTURE_STA_CH2 & 0X3F;
//                temp *= 65536;
//                temp += TIM4_CAPTURE_VAL_CH2;
//                Rc_Pwm_In[1] = temp;
//                capok |= (1 << 1);
//            }
//            if (TIM3_CAPTURE_STA_CH1 & 0X80)
//            {
//                temp  = TIM3_CAPTURE_STA_CH1 & 0X3F;
//                temp *= 65536;
//                temp += TIM3_CAPTURE_VAL_CH1;
//                Rc_Pwm_In[2] = temp;
//                capok |= (1 << 2);
//            }
//            if (TIM3_CAPTURE_STA_CH2 & 0X80)
//            {
//                temp  = TIM3_CAPTURE_STA_CH2 & 0X3F;
//                temp *= 65536;
//                temp += TIM3_CAPTURE_VAL_CH2;
//                Rc_Pwm_In[3] = temp;
//                capok |= (1 << 3);
//            }
//            if (TIM3_CAPTURE_STA_CH3 & 0X80)
//            {
//                temp  = TIM3_CAPTURE_STA_CH3 & 0X3F;
//                temp *= 65536;
//                temp += TIM3_CAPTURE_VAL_CH3;
//                Rc_Pwm_In[4] = temp;
//                capok |= (1 << 4);
//            }
//            if (TIM3_CAPTURE_STA_CH4 & 0X80)
//            {
//                temp  = TIM3_CAPTURE_STA_CH4 & 0X3F;
//                temp *= 65536;
//                temp += TIM3_CAPTURE_VAL_CH4;
//                Rc_Pwm_In[5] = temp;
//                capok |= (1 << 5);
//            }
//            if (TIM8_CAPTURE_STA_CH1 & 0X80)
//            {
//                temp  = TIM1_CAPTURE_STA_CH1 & 0X3F;
//                temp *= 65536;
//                temp += TIM8_CAPTURE_VAL_CH1;
//                Rc_Pwm_In[6] = temp;
//                capok |= (1 << 6);
//            }
//            if (TIM8_CAPTURE_STA_CH2 & 0X80)
//            {
//                temp  = TIM8_CAPTURE_STA_CH2 & 0X3F;
//                temp *= 65536;
//                temp += TIM8_CAPTURE_VAL_CH2;
//                Rc_Pwm_In[7] = temp;
//                capok |= (1 << 7);
//            }
//            if (TIM2_CAPTURE_STA_CH1 & 0X80)
//            {
//                temp  = TIM2_CAPTURE_STA_CH1 & 0X3F;
//                temp *= 65536;
//                temp += TIM2_CAPTURE_VAL_CH1;
//                Rc_Pwm_In[8] = temp;
//                capok |= (1 << 8);
//            }
//            if (TIM2_CAPTURE_STA_CH2 & 0X80)
//            {
//                temp  = TIM2_CAPTURE_STA_CH2 & 0X3F;
//                temp *= 65536;
//                temp += TIM2_CAPTURE_VAL_CH2;
//                Rc_Pwm_In[9] = temp;
//                capok |= (1 << 9);
//            }
//            utime++;
//            if (utime > 4 || capok == 0x3ff)
//            {
//                break;
//            }
//        }
//        Rc_DataAnl(Rc_Pwm_In);
//        Rc_GetValue(&Rc_D);
//        RC_Analyse(&Rc_D, &RC_Control);
//        Send.RCData = 1;
//        Send.RC = 1;
//        if (RC_flag_ok)
//        {
//            RC_flag_ok = 0;
//            Data_Receive_Anl(p_RxBuffer, RC_data_cnt + 5);
//        }
//        Data_Exchange();
//    }
//    _EE
//}
int task_cap_rc(void)
{
    static u16 Rc_Pwm_In[5];
    u32 temp = 0;
    static u8 capok;
    _SS
    TIM3_Cap_Init(0XFFFF, 72);
    TIM4_Cap_Init(0XFFFF, 72);
    while (1)
    {
        static u8 utime;
        WaitX(25 - utime * 2);
        TIM3_CAPTURE_STA_CH3 = 0;
        TIM4_CAPTURE_STA_CH1 = 0;
        TIM4_CAPTURE_STA_CH2 = 0;
        TIM4_CAPTURE_STA_CH3 = 0;
        TIM4_CAPTURE_STA_CH4 = 0;
        capok = 0;
        utime = 0;
        while (1)
        {
            WaitX(2);
            if (TIM3_CAPTURE_STA_CH3 & 0X80)
            {
                temp  = TIM3_CAPTURE_STA_CH3 & 0X3F;
                temp *= 65536;
                temp += TIM3_CAPTURE_VAL_CH3;
                Rc_Pwm_In[0] = temp;
                capok |= (1 << 0);
            }
            if (TIM4_CAPTURE_STA_CH1 & 0X80)
            {
                temp  = TIM4_CAPTURE_STA_CH1 & 0X3F;
                temp *= 65536;
                temp += TIM4_CAPTURE_VAL_CH1;
                Rc_Pwm_In[1] = temp;
                capok |= (1 << 1);
            }
            if (TIM4_CAPTURE_STA_CH2 & 0X80)
            {
                temp  = TIM4_CAPTURE_STA_CH2 & 0X3F;
                temp *= 65536;
                temp += TIM4_CAPTURE_VAL_CH2;
                Rc_Pwm_In[2] = temp;
                capok |= (1 << 2);
            }
            if (TIM4_CAPTURE_STA_CH3 & 0X80)
            {
                temp  = TIM4_CAPTURE_STA_CH3 & 0X3F;
                temp *= 65536;
                temp += TIM4_CAPTURE_VAL_CH3;
                Rc_Pwm_In[3] = temp;
                capok |= (1 << 3);
            }
            if (TIM4_CAPTURE_STA_CH4 & 0X80)
            {
                temp  = TIM4_CAPTURE_STA_CH4 & 0X3F;
                temp *= 65536;
                temp += TIM4_CAPTURE_VAL_CH4;
                Rc_Pwm_In[4] = temp;
                capok |= (1 << 4);
            }
            utime++;
            if (utime > 2 || capok == 0x1f)
            {
                break;
            }
        }
        Rc_DataAnl(Rc_Pwm_In);
        Rc_GetValue(&Rc_D);
        RC_Analyse(&Rc_D, &RC_Control);
        Send.RCData = 1;
    }
    _EE
}
