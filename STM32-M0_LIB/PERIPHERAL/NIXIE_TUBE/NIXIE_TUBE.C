#include "NIXIE_TUBE.H"
#include "g32_timer.h"
#include "gpio.h"

u8 num;

void NixieTube_Init(void)
{
		TIM6_Int_Init(1,7200);//20ms//500hz
		GPIOD_Init(0,Out_PP,PU);
		GPIOD_Init(1,Out_PP,PU);
		GPIOD_Init(2,Out_PP,PU);
		GPIOD_Init(3,Out_PP,PU);
		GPIOD_Init(4,Out_PP,PU);
		GPIOD_Init(5,Out_PP,PU);
		GPIOD_Init(6,Out_PP,PU);
}

void Nixie_Tube_Display(u8 temp)
{
	PDout(0)=(temp>>0)&0x01;
	PDout(1)=(temp>>1)&0x01;
	PDout(2)=(temp>>2)&0x01;
	PDout(3)=(temp>>3)&0x01;
	PDout(4)=(temp>>4)&0x01;
	PDout(5)=(temp>>5)&0x01;
	PDout(6)=(temp>>6)&0x01;
}

unsigned char const Et_Dis_Num[]={0x7e,0x30,0x6d,0x79,0x33,0x5b,0x5f,0x70,0x7f,0x7b/*-*/\
,0x77,0x1f,0x4e,0x3d,0x4f,0x47};
unsigned char const Et_Dis_Char[]={0x77,0x1f,0x4e,0x3d,0x4f,0x47/*-*/};

u8 light=1;
void TIM6_IRQHandler(void)
{
	static u8 i;
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
		i++;if(i>100)i=0;
		if(i<light)
			Nixie_Tube_Display(Et_Dis_Num[num]);
		else if(i==1)
			Nixie_Tube_Display(0);
	}
}


