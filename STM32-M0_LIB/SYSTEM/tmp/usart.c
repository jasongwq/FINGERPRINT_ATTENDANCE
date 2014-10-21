#include "sys.h"
#include "usart.h"
#include <stdio.h>
#include "stm32_config.h"
/*
//例
    if(USART2_RX_STA&0x8000)
    {
        u16 vol=0;
        u8 i;
        for(i=0;i<(USART2_RX_STA&0x3fff);i++)
        {
            vol=vol*10+(USART2_RX_BUF[i]-0x30);
        }
        USART2_RX_STA=0;
        USART_OUT(USART2,"%d\r\n",vol);
        Dac1_Set_Vol(vol);
    }
*********************************************
REMAP |  00      |   01         | 11        |
      |  TX  RX  |   TX  RX     | TX  RX    |
******|**********|**************|*******    |
USART1|  A9  A10 |   B6  B7     |           |
******|**********|**************|*******    |
USART2|  A2  A3  |   (D5  D6)   |           |
      |          |  (100)(144)  |           |
******|**********|**************|*******    |
USART3|  B10 B11 |   (C10 C11)  | (D8  D9)  |
      |          | (64 100 144) | (100 144) |
*********************************************
2013/8/27                            ********
2013/7/21                            ********
*/
//////////////////////////////////////////////////////////////////////////////////
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_UCOS
#include "includes.h"                   //ucos 使用
#endif
__align(4) char USART_TX_BUF[USART_SEND_LEN];

//uint8_t SendBuff[SENDBUFF_SIZE];

////////////////////////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
/*当连接器检查到用户编写了与C库函数相同名字的函数时，优先采用用户编写的函数*/
#if 0//标准库需要的支持函数
#pragma import(__use_no_semihosting)
struct __FILE
{
    int handle;
    /* Whatever you require here. If the only file you are using is */
    /* standard output using printf() for debugging, no file handling */
    /* is required. */
};
/* FILE is typedef’ d in stdio.h. */
FILE __stdout;
//定义_sys_exit()以避免使用半主机模式
_sys_exit(int x)
{
    x = x;
}
//重定义fputc函数
#if EN_USART_//串口1
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART1->DR = (u8) ch;
    return ch;
}
#elif EN_USART2_//串口2
int fputc(int ch, FILE *f)
{
    while ((USART2->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART2->DR = (u8) ch;
    return ch;
}
#elif EN_USART3_//串口3
int fputc(int ch, FILE *f)
{
    while ((USART3->SR & 0X40) == 0); //循环发送,直到发送完毕
    USART3->DR = (u8) ch;
    return ch;
}
#endif
#endif
//////////////////////////////////////////////////////////////////
#if EN_USART_RX   //如果使能了接收
//注意,读取USARTx->SR能避免莫名其妙的错误
u8 USART_RX_BUF[USART_REC_LEN]; //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，  接收完成标志
//bit14，  接收到0x0d
//bit13~0，    接收到的有效字节数目
u16 USART_RX_STA = 0;     //接收状态标记
void ATK_Usart1_IQR(void)
{
    u8 res;
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了.
    OSIntEnter();
#endif
    if (USART1->SR & (1 << 5)) //接收到数据
    {
        res = USART1->DR;
        if ((USART_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART_RX_STA & 0x4000) //接收到了0x0d
            {
                if (res != 0x0a)USART_RX_STA = 0; //接收错误,重新开始
                else USART_RX_STA |= 0x8000; //接收完成了
            }
            else  //还没收到0X0D
            {
                if (res == 0x0d)USART_RX_STA |= 0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA & 0X3FFF] = res;
                    USART_RX_STA++;
                    if (USART_RX_STA > (USART_REC_LEN - 1))USART_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了.
    OSIntExit();
#endif
}
#endif
#if EN_USART_
//初始化IO 串口1
//bound:波特率
void uart_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#if EN_USART_RX
    NVIC_InitTypeDef NVIC_InitStructure;
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1

#if AF_USART1_==1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); //GPIOA时钟以及复用功能时钟
    GPIO_PinRemapConfig(GPIO_Remap_USART1, ENABLE);//开启重映射
    //USART1_TX AF  PB.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PB.6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    //USART1_RX AF  PB.7
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#elif AF_USART1_==0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟
    //USART1_TX   PA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART1_RX   PA.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
#error Error_AF_USART1_
#endif
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART1, &USART_InitStructure); //初始化串口
    //Usart1 NVIC 配置
#if EN_USART_RX                                                 //如果使能了接收
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ;  //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;          //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;             //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);                             //根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART1, ENABLE);                    //使能串口
}
#endif

#if EN_USART2_RX   //如果使能了接收
u8 USART2_RX_BUF[USART2_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART2_RX_STA = 0;                //接收状态标记
void ATK_Usart2_IQR(void)
{
    u8 res;
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了.
    OSIntEnter();
#endif
    if (USART2->SR & (1 << 5)) //接收到数据
    {
        res = USART2->DR;
        if ((USART2_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART2_RX_STA & 0x4000) //接收到了0x0d
            {
                if (res != 0x0a)USART2_RX_STA = 0; //接收错误,重新开始
                else USART2_RX_STA |= 0x8000; //接收完成了
            }
            else  //还没收到0X0D
            {
                if (res == 0x0d)USART2_RX_STA |= 0x4000;
                else
                {
                    USART2_RX_BUF[USART2_RX_STA & 0X3FFF] = res;
                    USART2_RX_STA++;
                    if (USART2_RX_STA > (USART2_REC_LEN - 1))USART2_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}
#endif

#if EN_USART2_
void uart2_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#if EN_USART2_RX
    NVIC_InitTypeDef NVIC_InitStructure;
#endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2
#if AF_USART2_==1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE); //GPIOX时钟以及复用功能时钟
    GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);//开启重映射
    //USART2_TX AF  PD.5
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //PB.6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    //USART2_RX AF  PD.6
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#elif AF_USART2_==0

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIOA时钟
    //USART2_TX   PA.2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //USART2_RX   PA.3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
#error Error_AF_USART2_
#endif

    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART2, &USART_InitStructure); //初始化串口
    //Usart2 NVIC 配置
#if EN_USART2_RX         //如果使能了接收
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART2, ENABLE);                    //使能串口
}
#endif

#if EN_USART3_RX   //如果使能了接收
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA = 0;                //接收状态标记
void ATK_Usart3_IQR(void)
{
    u8 res;
#ifdef OS_TICKS_PER_SEC     //如果时钟节拍数定义了,说明要使用ucosII了.
    OSIntEnter();
#endif
    if (USART3->SR & (1 << 5)) //接收到数据
    {
        res = USART3->DR;
        if ((USART3_RX_STA & 0x8000) == 0) //接收未完成
        {
            if (USART3_RX_STA & 0x4000) //接收到了0x0d
            {
                if (res != 0x0a)USART3_RX_STA = 0; //接收错误,重新开始
                else USART3_RX_STA |= 0x8000; //接收完成了
            }
            else  //还没收到0X0D
            {
                if (res == 0x0d)USART3_RX_STA |= 0x4000;
                else
                {
                    USART3_RX_BUF[USART3_RX_STA & 0X3FFF] = res;
                    USART3_RX_STA++;
                    if (USART3_RX_STA > (USART3_REC_LEN - 1))USART3_RX_STA = 0; //接收数据错误,重新开始接收
                }
            }
        }
    }
}
#endif

#if EN_USART3_
void uart3_init(u32 bound)
{
    //GPIO端口设置
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
#if EN_USART3_RX
    NVIC_InitTypeDef NVIC_InitStructure;
#endif
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //使能USART3
#if AF_USART3_==1
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE); //GPIOX时钟以及复用功能时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap_USART3, ENABLE);//开启重映射
    //USART3_TX AF  PC.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    //USART3_RX AF  PC.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOC, &GPIO_InitStructure);
#elif AF_USART3_==2
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE); //GPIOX时钟以及复用功能时钟
    GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE);//开启重映射
    //USART3_TX AF  PD.8
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8; //PB.6
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    //USART3_RX AF  PD.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);
#elif AF_USART3_==0

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE); //使能GPIOA时钟以及复用功能时钟
    //USART3_TX   PB.10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //USART3_RX   PB.11
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
    GPIO_Init(GPIOB, &GPIO_InitStructure);
#else
#error Error_AF_USART3_
#endif
    //USART 初始化设置
    USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //收发模式

    USART_Init(USART3, &USART_InitStructure); //初始化串口
    //USART3 NVIC 配置
#if EN_USART3_RX         //如果使能了接收
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3 ; //抢占优先级3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;      //子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;         //IRQ通道使能
    NVIC_Init(&NVIC_InitStructure); //根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启中断
#endif
    USART_Cmd(USART3, ENABLE);                    //使能串口
}
#endif



/*
 * 函数名：DMA_Config
 * 描述  ：DMA 串口的初始化配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：外部调用
 */

void USART_DMA_Config(USART_TypeDef *USARTx, u32 SendBuff)
{
    DMA_Channel_TypeDef *DMAy_Channelx;
    //    uint8_t DMAy_Channelx_IRQn;
    DMA_InitTypeDef DMA_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    if (USARTx == USART1)
    {
        DMAy_Channelx = DMA1_Channel4;
        //        DMAy_Channelx_IRQn = DMA1_Channel4_IRQn;
    }
    else if (USARTx == USART2)
    {
        DMAy_Channelx = DMA1_Channel7;
        //        DMAy_Channelx_IRQn = DMA1_Channel7_IRQn;
    }
    else if (USARTx == USART3)
    {
        DMAy_Channelx = DMA1_Channel2;
        //        DMAy_Channelx_IRQn = DMA1_Channel2_IRQn;
    }
    DMA_DeInit(DMAy_Channelx);   //½«DMAµÄÍ¨µÀ1¼Ä´æÆ÷ÖØÉèÎªÈ±Ê¡Öµ

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  //开启DMA时钟
    //NVIC_Config();              //配置DMA中断
    //    NVIC_InitStructure.NVIC_IRQChannel = DMAy_Channelx_IRQn;
    //    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    //    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    //    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    //    NVIC_Init(&NVIC_InitStructure);

    /*设置DMA源：内存地址&串口数据寄存器地址*/
    //DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_Base;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USARTx->DR;

    /*内存地址(要传输的变量的指针)*/
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SendBuff;

    /*方向：从内存到外设*/
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;

    /*传输大小DMA_BufferSize=SENDBUFF_SIZE*/
    DMA_InitStructure.DMA_BufferSize = sizeof(SendBuff);//SENDBUFF_SIZE;

    /*外设地址不增*/
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;

    /*内存地址自增*/
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;

    /*外设数据单位*/
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;

    /*内存数据单位 8bit*/
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;

    /*DMA模式：一次传输，循环*/
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;

    /*优先级：中*/
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;

    /*禁止内存到内存的传输    */
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    /*配置DMA1的4通道*/
    DMA_Init(DMAy_Channelx, &DMA_InitStructure);

    //DMA_Cmd (DMAy_Channelx, ENABLE);                //使能DMA
    DMA_ITConfig(DMAy_Channelx, DMA_IT_TC, ENABLE); //配置DMA发送完成后产生中断
    USART_DMACmd(USARTx, USART_DMAReq_Tx, ENABLE); //使能串口1的DMA发送

}

//开启一次DMA传输
void USART_DMA_Enable(USART_TypeDef *USARTx)
{
    DMA_Channel_TypeDef *DMAy_Channelx;
    assert_param(IS_USART_ALL_PERIPH(USARTx));
    if (USARTx == USART1)
        DMAy_Channelx = DMA1_Channel4;
    else if (USARTx == USART2)
        DMAy_Channelx = DMA1_Channel7;
    else if (USARTx == USART3)
        DMAy_Channelx = DMA1_Channel2;

    DMA_Cmd(DMAy_Channelx, DISABLE );  //关闭USART1 TX DMA1 所指示的通道
    DMA_SetCurrDataCounter(DMA1_Channel7, SENDBUFF_SIZE); //DMA通道的DMA缓存的大小
    DMA_Cmd(DMAy_Channelx, ENABLE);    //使能USART1 TX DMA1 所指示的通道
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
