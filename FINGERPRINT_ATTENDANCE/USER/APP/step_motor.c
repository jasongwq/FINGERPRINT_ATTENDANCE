#include "step_motor.h"

//A+红色   A-蓝色   B+绿色  B-黑色
//         A     B     C     D     E     F     G     H

// A       1     1     0     0     0     0     0     1
// A-      0     0     0     1     1     1     0     0
// B       0     1     1     1     0     0     0     0
// B-      0     0     0     0     0     1     1     1

// u8 phase[2][8] =
// {
//     {0x09, 0x08, 0x0a, 0x02, 0x06, 0x04, 0x05, 0x01},
//     {0x01, 0x05, 0x04, 0x06, 0x02, 0x0a, 0x08, 0x09}
// };

int MOTOR0_dir=0;
int MOTOR1_dir=0;
int MOTOR0_Step=1;
int MOTOR1_Step=0;


