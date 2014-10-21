#ifndef __STEP_MOTOR_H
#define __STEP_MOTOR_H	 

//LED¶Ë¿Ú¶¨Òå
#define MOTOR0_0 PAout(6)// A+
#define MOTOR0_1 PAout(5)// A-
#define MOTOR0_2 PAout(4)// B+
#define MOTOR0_3 PAout(3)//	B+
extern int MOTOR0_dir;
extern int MOTOR0_Step;

#define MOTOR1_0 PBout(5)// A+
#define MOTOR1_1 PBout(6)// A- 
#define MOTOR1_2 PBout(7)// B+
#define MOTOR1_3 PBout(8)//	B+
extern int MOTOR1_dir;
extern int MOTOR1_Step;

#endif
