#include "HMC5883L.h"
#define PI 3.14159265


int task_hmc5883l(void)
{
    extern S_INT16_XYZ Mag;
		extern float compass_yaw;
    _SS
    HMC5883L_Init();
    HMC5883L_Calibrate();
    WaitX(20);
    while (1)
    {
        HMC5883L_Start();//7us
        WaitX(10);
        /**********430us*************/
        HMC5883L_MultRead(&hmc5883l);//8us
        //HMC5883L_Printf(&hmc5883l);
        Mag.x = hmc5883l.hx;
        Mag.y = hmc5883l.hy;
        Mag.z = hmc5883l.hz;
        {
            //From APM
            float angle1, angle2,
                  sin_angle1, sin_angle2,
                  cos_angle1, cos_angle2,
                  Xr, Yr;

            angle1 = (Att_Angle.rol + 180) / 180 * PI;
            angle2 = (-Att_Angle.pit + 180) / 180 * PI;
            sin_angle1 = sin(angle1);
            cos_angle1 = cos(angle1);
            sin_angle2 = sin(angle2);
            cos_angle2 = cos(angle2);
						
            Xr = Mag.x * cos_angle2 * cos_angle2 + Mag.y * sin_angle2 * sin_angle1 - Mag.z * cos_angle1 * sin_angle2;
            Yr = Mag.y * cos_angle1 + Mag.z * sin_angle1;
            compass_yaw = atan2(Yr, Xr) * (180 / PI); //160us
        }
        WaitX(10);
    }
    _EE
}
