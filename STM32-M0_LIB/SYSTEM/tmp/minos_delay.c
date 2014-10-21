#include "minos_delay.h"
#include "sys_os.h"
#include "sys.h"

/////////////////////////////////////////////////////////////////////////////////////
static u8  fac_us = 0; //us延时倍乘数

//初始化延迟函数
//当使用ucos的时候,此函数会初始化ucos的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
#ifdef OS_CRITICAL_METHOD   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    u32 reload;//重装载值
#endif
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);   //选择外部时钟  HCLK/8 //为系统时钟的1/8
    fac_us = SystemCoreClock / 8000000; 

#ifdef OS_CRITICAL_METHOD   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
    reload = SystemCoreClock / 8000000; //每秒钟的计数次数 单位为K
    reload *= 1000000 / OS_TICKS_PER_SEC; //根据OS_TICKS_PER_SEC设定溢出时间
    //reload为24位寄存器,最大值:16777216,在72M下,约合1.86s左右
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;  //开启SYSTICK中断
    SysTick->LOAD = reload; //每1/OS_TICKS_PER_SEC秒中断一次
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;   //开启SYSTICK
#else
    fac_ms = (u16)fac_us * 1000; //非ucos下,代表每个ms需要的systick时钟数
#endif
}

#ifdef OS_CRITICAL_METHOD   //使用了ucos
//延时nus
//nus为要延时的us数.
void delay_us(u32 nus)
{
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD; //LOAD的值
    ticks = nus * fac_us;       //需要的节拍数
    tcnt = 0;
    told = SysTick->VAL;        //刚进入时的计数器值
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
            else tcnt += reload - tnow + told;
            told = tnow;
            if (tcnt >= ticks)break; //时间超过/等于要延迟的时间,则退出.
        }
    };
}
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{
    for (int i = 0; i < nms; i++)
    {
        delay_us((u32)(1000)); //普通方式延时,此时ucos无法启动调度.
    }
}
extern u32 SysTick_Time;
int SysTick_Clock(void)
{
    return (((SysTick_Time + 1) * SysTick->LOAD) - SysTick->VAL) / (fac_us*1000 / OS_TICKS_PER_SEC);
}

#else//不用ucos时
//延时nus
//nus为要延时的us数.
void delay_us(u32 nus)
{
    u32 temp;
    SysTick->LOAD = nus * fac_us; //时间加载
    SysTick->VAL = 0x00;      //清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;        //开始倒数
    do
    {
        temp = SysTick->CTRL;
    }
    while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;     //关闭计数器
    SysTick->VAL = 0X00;      //清空计数器
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864
void delay_ms(u16 nms)
{
    u32 temp;
    SysTick->LOAD = (u32)nms * fac_ms; //时间加载(SysTick->LOAD为24bit)
    SysTick->VAL = 0x00;          //清空计数器
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;        //开始倒数
    do
    {
        temp = SysTick->CTRL;
    }
    while (temp & 0x01 && !(temp & (1 << 16))); //等待时间到达
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;     //关闭计数器
    SysTick->VAL = 0X00;      //清空计数器
}
#endif
