T_float_angle   Att_Angle_Avg;
int loop_fast(void)//500hz
{
    extern S_INT16_XYZ Acc, Average_Acc, Gyr, Mag;
    _SS
    _LOOP_SS
    if (flag_ACC)
    {
        Prepare_Data2(&Att_Angle);//24us
        IMUupdate(&Gyr, &Average_Acc, &Att_Angle);//222us
        //Control(&Att_Angle, &Gyr, &Rc_D, &RC_Control);//17us
        Balance_Data(&Att_Angle, &Gyr,&Acc, &Rc_D, &RC_Control);//17us
        Balance();
    }
    LoopX(2);
    _EE
}
