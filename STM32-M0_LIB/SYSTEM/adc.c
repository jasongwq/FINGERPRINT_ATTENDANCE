/***************************************************************/
#include "stm32f0xx.h"
#include "adc.h"
/***************************************************************/
int AD_value;									
unsigned int Precent,Voltage;				
/***************************************************************/

void ADC_Configuration(u16 ADC_Channel)
{
	u8 i=0;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);

	if(ADC_Channel==0){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;i=1;}
	else if(ADC_Channel==1){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;i=1;}
	else if(ADC_Channel==2){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;i=1;}
	else if(ADC_Channel==3){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;i=1;}
	else if(ADC_Channel==4){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;i=1;}
	else if(ADC_Channel==5){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;i=1;}
	else if(ADC_Channel==6){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;i=1;}
	else if(ADC_Channel==7){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;i=1;}
	else if(ADC_Channel==9){GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;i=2;}

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	       //端口模式为模拟输入方式
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	if(i==1){GPIO_Init(GPIOA, &GPIO_InitStructure);}
	else if(i==2){GPIO_Init(GPIOB, &GPIO_InitStructure);}
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward ; 
	ADC_Init(ADC1, &ADC_InitStructure);
	
    if(ADC_Channel==0){ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_55_5Cycles);}   
	else if(ADC_Channel==1){ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==2){ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==3){ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==4){ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==5){ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==6){ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==7){ADC_ChannelConfig(ADC1, ADC_Channel_7, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==9){ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_55_5Cycles);} 
	
	
	ADC_GetCalibrationFactor(ADC1);
	ADC_Cmd(ADC1, ENABLE);
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
	ADC_StartOfConversion(ADC1);
}

int Get_ADC_Voltage()
{
	AD_value=ADC_GetConversionValue(ADC1);		//读取ADC转换出的值
	Precent = (AD_value*100/4096);			    //算出百分比
	Voltage = Precent*33;						//3.3V的电平，计算等效电平
	return(Voltage);
}

void Change_Channel(u16 ADC_Channel)
{
	ADC_InitTypeDef ADC_InitStructure;
	
	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);
//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; 
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;     
//	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; 
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
//	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward ; 
	ADC_Init(ADC1, &ADC_InitStructure);
	if(ADC_Channel==0){ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_55_5Cycles);}   
	else if(ADC_Channel==1){ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==2){ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==3){ADC_ChannelConfig(ADC1, ADC_Channel_3, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==4){ADC_ChannelConfig(ADC1, ADC_Channel_4, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==5){ADC_ChannelConfig(ADC1, ADC_Channel_5, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==6){ADC_ChannelConfig(ADC1, ADC_Channel_6, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==7){ADC_ChannelConfig(ADC1, ADC_Channel_7, ADC_SampleTime_55_5Cycles);} 
	else if(ADC_Channel==9){ADC_ChannelConfig(ADC1, ADC_Channel_9, ADC_SampleTime_55_5Cycles);} 

//	ADC_GetCalibrationFactor(ADC1);
	ADC_Cmd(ADC1, ENABLE);
//	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN)); 
	ADC_StartOfConversion(ADC1);
}

