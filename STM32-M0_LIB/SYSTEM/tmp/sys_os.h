#ifndef __SYS_OS_H
#define __SYS_OS_H
#include "sys.h"
/*最大 256
unsigned char
 */
#define MAXTASKS 9
extern volatile unsigned int timers[MAXTASKS];
#define _SS static unsigned char _lc=0; switch(_lc){default:
#define _EE ;}; _lc=0; return 255;
#define WaitX(tickets)  do {_lc=(__LINE__+((__LINE__%256)==0))%256; return tickets ;} while(0); case (__LINE__+((__LINE__%256)==0))%256:

#define RunTask(TaskName,TaskID) do { if (timers[TaskID]==0) timers[TaskID]=TaskName(); }  while(0);
#define RunTaskA(TaskName,TaskID) { if (timers[TaskID]==0) {timers[TaskID]=TaskName(); continue;} }   //前面的任务优先保证执行

//#define InitTasks() {unsigned char i; for(i=MAXTASKS;i>0 ;i--) timers[i-1]=0; }
#define UpdateTimers() {unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=0xffffffff)) timers[i-1]--;}}


//////////////////////////////////////////////////////////////////////////////////

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


/*****小小调度器结束*******************************************************/

#define OS_CRITICAL_METHOD
//每秒钟 多少次
#define OS_TICKS_PER_SEC 1000






extern int task5(void);

#endif
