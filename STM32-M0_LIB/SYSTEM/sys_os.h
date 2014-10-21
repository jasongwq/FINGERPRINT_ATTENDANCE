#ifndef __SYS_OS_H
#define __SYS_OS_H
#define MAXTASKS 5
#define TIME unsigned int
extern volatile TIME timers[];
extern int minoserror;
#define _SS static int _lc=0; switch(_lc){default:
#define _EE ;}; _lc=0; return 255;
#define WaitX(tickets)  if(minoserror)break;do {_lc=(__LINE__+((__LINE__%256)==0))%256; return tickets ;} while(0); case (__LINE__+((__LINE__%256)==0))%256:
#define RunTask(TaskName,TaskID) do { if (timers[TaskID]==0) timers[TaskID]=TaskName(); }  while(0);
#define RunTaskA(TaskName,TaskID) { if (timers[TaskID]==0) {timers[TaskID]=TaskName(); continue;}}//前面的任务优先保证执行
#define UpdateTimers() {unsigned char i; for(i=MAXTASKS;i>0 ;i--){if((timers[i-1]!=0)&&(timers[i-1]!=65535)) timers[i-1]--;}}


#define OS_CRITICAL_METHOD
#define OS_TICKS_PER_SEC 2000

extern int task0(void);
//int SysTick_Clock(void);
#endif
