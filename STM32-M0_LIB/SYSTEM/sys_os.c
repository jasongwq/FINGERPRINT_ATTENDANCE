#include "sys_os.h"
//////////////////////////////////////////////////////////////////////////////////
#define TIME unsigned int

volatile TIME timers[MAXTASKS] = {0};
int minoserror = 0;

#define CallSub(SubTaskName) do {unsigned char currdt; _lc=(__LINE__+((__LINE__%256)==0))%256; return 0; case (__LINE__+((__LINE__%256)==0))%256:  currdt=SubTaskName(); if(currdt!=255) return currdt;} while(0);

#define SEM unsigned int
//初始化信号量
#define InitSem(sem) sem=0;
//等待信号量
#define WaitSem(sem) do{ sem=1; WaitX(0); if (sem>0) return 1;} while(0);
//等待信号量或定时器溢出， 定时器tickets 最大为0xFFFE
#define WaitSemX(sem,tickets)  do { sem=tickets+1; WaitX(0); if(sem>1){ sem--;  return 1;} } while(0);
//发送信号量
#define SendSem(sem)  do {sem=0;} while(0);

#include "sys.h"
//extern u32 SysTick_Time;
//int SysTick_Clock(void)
//{
//    return (((SysTick_Time + 1) * SysTick->LOAD) - SysTick->VAL) / (SystemCoreClock / 8000 / OS_TICKS_PER_SEC);
//}



