int flag_ACC = 0;

int task_6050(void)
{
    extern S_INT16_XYZ Acc, Gyr, Mag, Average_Acc;
    _SS
    MPU6050_Init();
    if (0 == Data_Read())
    {
        StmFlash_Read();
    }
    WaitX(200);
    while (1)
    {
        WaitX(1);
        MPU6050_Read();
        MPU6050_Dataanl();//5us
        Acc.x = MPU6050_ACC_LAST.x;
        Acc.y = MPU6050_ACC_LAST.y;
        Acc.z = MPU6050_ACC_LAST.z;
        Gyr.x = MPU6050_GYRO_LAST.x;
        Gyr.y = MPU6050_GYRO_LAST.y;
        Gyr.z = MPU6050_GYRO_LAST.z;
        Prepare_Data(&Acc, &Average_Acc);//4us
        flag_ACC = 1;
    }
    _EE
}
