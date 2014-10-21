#ifndef _DATA_TRANSFER_H_
#define _DATA_TRANSFER_H_
#include "sys.h"
typedef struct int16_rcget{
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
				int16_t AUX1;
				int16_t AUX2;
				int16_t AUX3;
				int16_t AUX4;
				int16_t AUX5;
				int16_t AUX6;}T_RC_Data;
				
extern u8 Data_Check,Send_Status,Send_Senser,Send_RCData,Send_GpsData,Send_Offset,Send_PID1,Send_PID2,Send_PID3,Send_MotoPwm;
extern void Data_Exchange(void);
extern void Data_Send_Senser(void);
typedef struct {float P,pout,I,iout,D,dout,IMAX,OUT;}PID;
extern PID PID_ROL, PID_PIT, PID_YAW, PID_ALT, PID_POS, PID_PID_1, PID_PID_2;

#endif

