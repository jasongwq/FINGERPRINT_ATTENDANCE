#ifndef __UI_84_48_
#define __UI_84_48_
#include "sys.h"

extern u8(*now_func)(u8);//当前显示函数;
extern u8(*temp_func)(u8);
extern u8(*last_func[])(u8);
extern u8 dtbuf[];
extern u8 key_deep;

void UI_84_48_Init(void);
u8 UI_menu_main(u8);
#endif
