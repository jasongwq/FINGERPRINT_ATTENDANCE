#include "touch.h"
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "exfuns.h"
#include "ff.h"
#include "usart.h"
#include "stdio.h"

//#include "24cxx.h"
//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//触摸屏驱动（支持ADS7843/7846/UH7843/7846/XPT2046/TSC2046等） 代码
/*
资源：
SPI
*/
//////////////////////////////////////////////////////////////////////////////////

//_m_tp_dev tp_dev =
//{
//    TP_Init,
//    TP_Scan,
//    TP_Adjust,
//    0,
//    0,
//    0,
//    0,
//    0,
//    0,//0.066844,
//    0,//0.109823,
//    0,//-12,
//    0,//-22,
//    0,//1,
//};
//_m_tp_dev tp_dev =//奋斗 3寸屏
//{
//    TP_Init,
//    TP_Scan,
//    TP_Adjust,
//    0,
//    0,
//    0,
//    0,
//    0,
//    0.066844,
//    0.109823,
//    -12+4,
//    -22-4,
//    1,
//};
_m_tp_dev tp_dev =
{
    TP_Init,
    TP_Scan,
    TP_Adjust,
    0,
    0,
    0,
    0,
    0,
    0.071098,
    0.089543,
    -33,
    -12,
    0,//1,
};
//默认为touchtype=0的数据.
//u8 CMD_RDX = 0X90;
//u8 CMD_RDY = 0XD0;
u8 CMD_RDX = 0XD0;
u8 CMD_RDY = 0X90;


#ifdef LCD_TOUCH_SPI_SOFT

static void TP_Write_Byte(u8 num)
{
    u8 count = 0;
    for (count = 0; count < 8; count++)
    {
        if (num & 0x80)TDIN = 1;
        else TDIN = 0;
        num <<= 1;
        TCLK = 0;
        TCLK = 1;   //上升沿有效
    }
}
#else
//SPI写数据
//向触摸屏IC写入1byte数据
//num:要写入的数据
static unsigned char SPI_WriteByte(unsigned char data)
{
    unsigned char Data = 0;
    //等待发送缓冲区空
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    // 发送一个字节
    SPI_I2S_SendData(SPI1, data);
    //等待是否接收到一个字节
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
    // 获得该字节
    Data = SPI_I2S_ReceiveData(SPI1);
    // 返回收到的字节
    return Data;
}
#endif
#ifdef LCD_TOUCH_SPI_SOFT
static u16 TP_Read_AD(u8 CMD)
{
    u8 count = 0;
    u16 Num = 0;
    TCLK = 0;   //先拉低时钟
    TDIN = 0;   //拉低数据线
    TCS = 0;    //选中触摸屏IC
    TP_Write_Byte(CMD);//发送命令字
    delay_us(6);//ADS7846的转换时间最长为6us
    TCLK = 0;
    delay_us(1);
    TCLK = 1;   //给1个时钟，清除BUSY
    TCLK = 0;
    for (count = 0; count < 16; count++) //读出16位数据,只有高12位有效
    {
        Num <<= 1;
        TCLK = 0; //下降沿有效
        TCLK = 1;
        if (DOUT)Num++;
    }
    Num >>= 4;  //只有高12位有效.
    TCS = 1;    //释放片选
    return (Num);
}
#else
//SPI读数据
//从触摸屏IC读取adc值
//CMD:指令
//返回值:读到的数据
u16 TP_Read_AD(u8 CMD)
{
    u16 x = 0;
    TCS = 0;                         //选择XPT2046
    delay_us(6);
    SPI_WriteByte(CMD);              //设置X轴读取标志
    delay_us(6);
    x = SPI_WriteByte(0x00);         //连续读取16位的数据
    x <<= 8;
    x += SPI_WriteByte(0x00);
    delay_us(6);
    TCS = 1;
    x = x >> 3;                      //移位换算成12位的有效数据0-4095
    return (x);
}
#endif
//读取一个坐标值(x或者y)
//连续读取READ_TIMES次数据,对这些数据升序排列,
//然后去掉最低和最高LOST_VAL个数,取平均值
//xy:指令（CMD_RDX/CMD_RDY）
//返回值:读到的数据
#define READ_TIMES 5    //读取次数
#define LOST_VAL 1      //丢弃值
u16 TP_Read_XOY(u8 xy)
{
    u16 i, j;
    u16 buf[READ_TIMES];
    u16 sum = 0;
    u16 temp;
    for (i = 0; i < READ_TIMES; i++)buf[i] = TP_Read_AD(xy);
    for (i = 0; i < READ_TIMES - 1; i++) //排序
    {
        for (j = i + 1; j < READ_TIMES; j++)
        {
            if (buf[i] > buf[j]) //升序排列
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }
    sum = 0;
    for (i = LOST_VAL; i < READ_TIMES - LOST_VAL; i++)sum += buf[i];
    temp = sum / (READ_TIMES - 2 * LOST_VAL);
    return temp;
}
//读取x,y坐标
//最小值不能少于100.
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
u8 TP_Read_XY(u16* x, u16* y)
{
    u16 xtemp, ytemp;
    xtemp = TP_Read_XOY(CMD_RDX);
    ytemp = TP_Read_XOY(CMD_RDY);
    //if(xtemp<100||ytemp<100)return 0;//读数失败
    *x = xtemp;
    *y = ytemp;
    return 1;//读数成功
}
//连续2次读取触摸屏IC,且这两次的偏差不能超过
//ERR_RANGE,满足条件,则认为读数正确,否则读数错误.
//该函数能大大提高准确度
//x,y:读取到的坐标值
//返回值:0,失败;1,成功。
#define ERR_RANGE 50 //误差范围 
u8 TP_Read_XY2(u16* x, u16* y)
{
    u16 x1, y1;
    u16 x2, y2;
    u8 flag;
    flag = TP_Read_XY(&x1, &y1);
    if (flag == 0)return (0);
    flag = TP_Read_XY(&x2, &y2);
    if (flag == 0)return (0);
    if (((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) //前后两次采样在+-50内
            && ((y2 <= y1 && y1 < y2 + ERR_RANGE) || (y1 <= y2 && y2 < y1 + ERR_RANGE)))
    {
        *x = (x1 + x2) / 2;
        *y = (y1 + y2) / 2;
        return 1;
    }
    else return 0;
}
//////////////////////////////////////////////////////////////////////////////////
//与LCD部分有关的函数
//画一个触摸点
//用来校准用的
//x,y:坐标
//color:颜色
void TP_Drow_Touch_Point(u16 x, u16 y, u16 color)
{
    POINT_COLOR = color;
    LCD_DrawLine(x - 12, y, x + 13, y); //横线
    LCD_DrawLine(x, y - 12, x, y + 13); //竖线
    LCD_DrawPoint(x + 1, y + 1);
    LCD_DrawPoint(x - 1, y + 1);
    LCD_DrawPoint(x + 1, y - 1);
    LCD_DrawPoint(x - 1, y - 1);
    Draw_Circle(x, y, 6); //画中心圈
}
//画一个大点(2*2的点)
//x,y:坐标
//color:颜色
void TP_Draw_Big_Point(u16 x, u16 y, u16 color)
{
    POINT_COLOR = color;
    LCD_DrawPoint(x, y); //中心点
    LCD_DrawPoint(x + 1, y);
    LCD_DrawPoint(x, y + 1);
    LCD_DrawPoint(x + 1, y + 1);
}
//////////////////////////////////////////////////////////////////////////////////
//触摸按键扫描
//tp:0,屏幕坐标;1,物理坐标(校准等特殊场合用)
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 TP_Scan(u8 tp)
{
    if (PEN == 0) //有按键按下
    {
        if (tp)TP_Read_XY2(&tp_dev.x, &tp_dev.y); //读取物理坐标
        else if (TP_Read_XY2(&tp_dev.x, &tp_dev.y)) //读取屏幕坐标
        {
            tp_dev.x = tp_dev.xfac * tp_dev.x + tp_dev.xoff; //将结果转换为屏幕坐标
            tp_dev.y = tp_dev.yfac * tp_dev.y + tp_dev.yoff;
        }
        if ((tp_dev.sta & TP_PRES_DOWN) == 0) //之前没有被按下
        {
            tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES; //按键按下
            tp_dev.x0 = tp_dev.x; //记录第一次按下时的坐标
            tp_dev.y0 = tp_dev.y;
        }
    }
    else
    {
        if (tp_dev.sta & TP_PRES_DOWN) //之前是被按下的
        {
            tp_dev.sta &= ~(1 << 7); //标记按键松开
        }
        else //之前就没有被按下
        {
            tp_dev.x0 = 0;
            tp_dev.y0 = 0;
            tp_dev.x = 0xffff;
            tp_dev.y = 0xffff;
        }
    }
    return tp_dev.sta & TP_PRES_DOWN; //返回当前的触屏状态
}
//////////////////////////////////////////////////////////////////////////
//保存在EEPROM里面的地址区间基址,占用13个字节(RANGE:SAVE_ADDR_BASE~SAVE_ADDR_BASE+12)
//#define SAVE_ADDR_BASE 40
////保存校准参数
//void TP_Save_Adjdata(void)
//{
//    s32 temp;
//    //保存校正结果!
//    f_open(file, "0://hjk.txt", FA_CREATE_ALWAYS);
//    f_close(file);
//    f_open(file, "0://hjk.txt", FA_WRITE);
//    temp = tp_dev.xfac * 1000000; //保存x校正因素
//    printf(" %d \n ", (s32)temp);
//    f_printf(file, "%f", temp);

//    //AT24CXX_WriteLenByte(SAVE_ADDR_BASE, temp, 4);
//    temp = tp_dev.yfac * 1000000; //保存y校正因素
//    printf(" %d \n ", (s32)temp);
//    f_printf(file, "%f", temp);
//    printf("%d %d %d", tp_dev.xoff, tp_dev.yoff, tp_dev.touchtype);
//    f_printf(file, "%d\n %d\n %d\n", tp_dev.xoff, tp_dev.yoff, tp_dev.touchtype);

//    //AT24CXX_WriteLenByte(SAVE_ADDR_BASE + 4, temp, 4);
//    //保存x偏移量
//    //    AT24CXX_WriteLenByte(SAVE_ADDR_BASE + 8, tp_dev.xoff, 2);
//    //    //保存y偏移量
//    //    AT24CXX_WriteLenByte(SAVE_ADDR_BASE + 10, tp_dev.yoff, 2);
//    //    //保存触屏类型
//    //    AT24CXX_WriteOneByte(SAVE_ADDR_BASE + 12, tp_dev.touchtype);
//    //    temp = 0X0A; //标记校准过了
//    //    AT24CXX_WriteOneByte(SAVE_ADDR_BASE + 13, temp);

//    //  f_printf(file,"%d",temp,);
//    f_close(file);
//}
//得到保存在EEPROM里面的校准值
//返回值：1，成功获取数据
//        0，获取失败，要重新校准
//u8 TP_Get_Adjdata(void)
//{
//    s32 tempfac;
//    tempfac = AT24CXX_ReadOneByte(SAVE_ADDR_BASE + 13); //读取标记字,看是否校准过！
//    if (tempfac == 0X0A) //触摸屏已经校准过了
//    {
//        tempfac = AT24CXX_ReadLenByte(SAVE_ADDR_BASE, 4);
//        tp_dev.xfac = (float)tempfac / 100000000; //得到x校准参数
//        tempfac = AT24CXX_ReadLenByte(SAVE_ADDR_BASE + 4, 4);
//        tp_dev.yfac = (float)tempfac / 100000000; //得到y校准参数
//        //得到x偏移量
//        tp_dev.xoff = AT24CXX_ReadLenByte(SAVE_ADDR_BASE + 8, 2);
//        //得到y偏移量
//        tp_dev.yoff = AT24CXX_ReadLenByte(SAVE_ADDR_BASE + 10, 2);
//        tp_dev.touchtype = AT24CXX_ReadOneByte(SAVE_ADDR_BASE + 12); //读取触屏类型标记
//        if (tp_dev.touchtype) //X,Y方向与屏幕相反
//        {
//            CMD_RDX = 0X90;
//            CMD_RDY = 0XD0;
//        }
//        else                   //X,Y方向与屏幕相同
//        {
//            CMD_RDX = 0XD0;
//            CMD_RDY = 0X90;
//        }
//        return 1;
//    }
//    return 0;
//}
//提示字符串
const u8* TP_REMIND_MSG_TBL = "Please use the stylus click the cross on the screen.\
The cross will always move until the screen adjustment is completed.";

//提示校准结果(各个参数)
void TP_Adj_Info_Show(u16 x0, u16 y0, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 fac)
{
    POINT_COLOR = RED;
    LCD_ShowString(40, 160, lcddev.width, lcddev.height, 16, "x1:");
    LCD_ShowString(40 + 80, 160, lcddev.width, lcddev.height, 16, "y1:");
    LCD_ShowString(40, 180, lcddev.width, lcddev.height, 16, "x2:");
    LCD_ShowString(40 + 80, 180, lcddev.width, lcddev.height, 16, "y2:");
    LCD_ShowString(40, 200, lcddev.width, lcddev.height, 16, "x3:");
    LCD_ShowString(40 + 80, 200, lcddev.width, lcddev.height, 16, "y3:");
    LCD_ShowString(40, 220, lcddev.width, lcddev.height, 16, "x4:");
    LCD_ShowString(40 + 80, 220, lcddev.width, lcddev.height, 16, "y4:");
    LCD_ShowString(40, 240, lcddev.width, lcddev.height, 16, "fac is:");
    LCD_ShowNum(40 + 24, 160, x0, 4, 16); //显示数值
    LCD_ShowNum(40 + 24 + 80, 160, y0, 4, 16); //显示数值
    LCD_ShowNum(40 + 24, 180, x1, 4, 16); //显示数值
    LCD_ShowNum(40 + 24 + 80, 180, y1, 4, 16); //显示数值
    LCD_ShowNum(40 + 24, 200, x2, 4, 16); //显示数值
    LCD_ShowNum(40 + 24 + 80, 200, y2, 4, 16); //显示数值
    LCD_ShowNum(40 + 24, 220, x3, 4, 16); //显示数值
    LCD_ShowNum(40 + 24 + 80, 220, y3, 4, 16); //显示数值
    LCD_ShowNum(40 + 56, lcddev.width, fac, 3, 16); //显示数值,该数值必须在95~105范围之内.

}

//触摸屏校准代码
//得到四个校准参数
void TP_Adjust(void)
{
    u16 pos_temp[4][2];//坐标缓存值
    u8  cnt = 0;
    u16 d1, d2;
    u32 tem1, tem2;
    float fac;
    u16 outtime = 0;
    cnt = 0;
    POINT_COLOR = BLUE;
    BACK_COLOR = WHITE;
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = RED; //红色
    LCD_Clear(WHITE);//清屏
    POINT_COLOR = BLACK;
    LCD_ShowString(40, 40, 160, 100, 16, (char*)TP_REMIND_MSG_TBL);  //显示提示信息
    TP_Drow_Touch_Point(20, 20, RED); //画点1
    tp_dev.sta = 0; //消除触发信号
    tp_dev.xfac = 0; //xfac用来标记是否校准过,所以校准之前必须清掉!以免错误
    while (1) //如果连续10秒钟没有按下,则自动退出
    {
        tp_dev.scan(1);//扫描物理坐标
        if ((tp_dev.sta & 0xc0) == TP_CATH_PRES) //按键按下了一次(此时按键松开了.)
        {
            outtime = 0;
            tp_dev.sta &= ~(1 << 6); //标记按键已经被处理过了.

            pos_temp[cnt][0] = tp_dev.x;
            pos_temp[cnt][1] = tp_dev.y;
            // LCD_ShowNum(40, 40, tp_dev.x, 10, 16);               //显示一个数字
            // LCD_ShowNum(40, 60, tp_dev.y, 10, 16);               //显示一个数字

            cnt++;
            switch (cnt)
            {
            case 1:
                TP_Drow_Touch_Point(20, 20, WHITE);             //清除点1
                TP_Drow_Touch_Point(lcddev.width - 20, 20, RED); //画点2
                break;
            case 2:
                TP_Drow_Touch_Point(lcddev.width - 20, 20, WHITE); //清除点2
                TP_Drow_Touch_Point(20, lcddev.height - 20, RED); //画点3
                break;
            case 3:
                TP_Drow_Touch_Point(20, lcddev.height - 20, WHITE);     //清除点3
                TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, RED); //画点4
                break;
            case 4:  //全部四个点已经得到
                //对边相等
                tem1 = abs(pos_temp[0][0] - pos_temp[1][0]); //x1-x2
                tem2 = abs(pos_temp[0][1] - pos_temp[1][1]); //y1-y2
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2); //得到1,2的距离

                tem1 = abs(pos_temp[2][0] - pos_temp[3][0]); //x3-x4
                tem2 = abs(pos_temp[2][1] - pos_temp[3][1]); //y3-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2); //得到3,4的距离
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05 || d1 == 0 || d2 == 0) //不合格
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                    TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                    TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
                    continue;
                }
                tem1 = abs(pos_temp[0][0] - pos_temp[2][0]); //x1-x3
                tem2 = abs(pos_temp[0][1] - pos_temp[2][1]); //y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2); //得到1,3的距离

                tem1 = abs(pos_temp[1][0] - pos_temp[3][0]); //x2-x4
                tem2 = abs(pos_temp[1][1] - pos_temp[3][1]); //y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2); //得到2,4的距离
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05) //不合格
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                    TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                    TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
                    continue;
                }//正确了

                //对角线相等
                tem1 = abs(pos_temp[1][0] - pos_temp[2][0]); //x1-x3
                tem2 = abs(pos_temp[1][1] - pos_temp[2][1]); //y1-y3
                tem1 *= tem1;
                tem2 *= tem2;
                d1 = sqrt(tem1 + tem2); //得到1,4的距离

                tem1 = abs(pos_temp[0][0] - pos_temp[3][0]); //x2-x4
                tem2 = abs(pos_temp[0][1] - pos_temp[3][1]); //y2-y4
                tem1 *= tem1;
                tem2 *= tem2;
                d2 = sqrt(tem1 + tem2); //得到2,3的距离
                fac = (float)d1 / d2;
                if (fac < 0.95 || fac > 1.05) //不合格
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                    TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                    TP_Adj_Info_Show(pos_temp[0][0], pos_temp[0][1], pos_temp[1][0], pos_temp[1][1], pos_temp[2][0], pos_temp[2][1], pos_temp[3][0], pos_temp[3][1], fac * 100); //显示数据
                    continue;
                }//正确了
                //计算结果
                tp_dev.xfac = (float)(lcddev.width - 40) / (pos_temp[1][0] - pos_temp[0][0]); //得到xfac
                //                printf("%f ", tp_dev.xfac);
                tp_dev.xoff = (lcddev.width - tp_dev.xfac * (pos_temp[1][0] + pos_temp[0][0])) / 2; //得到xoff

                tp_dev.yfac = (float)(lcddev.height - 40) / (pos_temp[2][1] - pos_temp[0][1]); //得到yfac
                //                printf("%f ", tp_dev.yfac);

                tp_dev.yoff = (lcddev.height - tp_dev.yfac * (pos_temp[2][1] + pos_temp[0][1])) / 2; //得到yoff
                if (abs(tp_dev.xfac) > 2 || abs(tp_dev.yfac) > 2) //触屏和预设的相反了.
                {
                    cnt = 0;
                    TP_Drow_Touch_Point(lcddev.width - 20, lcddev.height - 20, WHITE); //清除点4
                    TP_Drow_Touch_Point(20, 20, RED);                           //画点1
                    LCD_ShowString(40, 26, lcddev.width, lcddev.height, 16, "TP Need readjust!");
                    tp_dev.touchtype = !tp_dev.touchtype; //修改触屏类型.
                    if (tp_dev.touchtype) //X,Y方向与屏幕相反
                    {
                        CMD_RDX = 0X90;
                        CMD_RDY = 0XD0;
                    }
                    else                   //X,Y方向与屏幕相同
                    {
                        CMD_RDX = 0XD0;
                        CMD_RDY = 0X90;
                    }
                    continue;
                }
                POINT_COLOR = BLUE;
                LCD_Clear(WHITE);//清屏
                LCD_ShowString(35, 110, lcddev.width, lcddev.height, 16, "Touch Screen Adjust OK!"); //校正完成
                delay_ms(500);
                //TP_Save_Adjdata();
                LCD_Clear(WHITE);//清屏
                return;//校正完成
            }
        }
        delay_ms(10);
        outtime++;
        if (outtime > 1000)
        {
            //TP_Get_Adjdata();
            LCD_Clear(WHITE);//清屏
            break;
        }
    }
}
//触摸屏初始化
//返回值:0,没有进行校准
//       1,进行过校准
//触摸屏初始化
//返回值:0,没有进行校准
//       1,进行过校准
u8 TP_Init(void)
{
    char ubuf[256];
    //注意,时钟使能之后,对GPIO的操作才有效
    //所以上拉之前,必须使能时钟.才能实现真正的上拉输出

    GPIO_InitTypeDef  GPIO_InitStructure;

    /* SPI1 时钟使能 */

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能PB,PF端口时钟
#ifdef LCD_TOUCH_SPI_SOFT

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE); //使能PB,PF端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                // PB1,PB2端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_4); //上拉

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;         //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_5 | GPIO_Pin_7); //上拉

    //  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                // PF9端口配置 推挽输出
    //  GPIO_Init(GPIOF, &GPIO_InitStructure);
    //  GPIO_SetBits(GPIOF,GPIO_Pin_9);//P9 上拉

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6;  // PF8.PF10端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;        // 上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);
#else
    SPI_InitTypeDef   SPI_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    /* SPI1 SCK(PA5)、MISO(PA6)、MOSI(PA7) 设置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //口线速度50MHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           //复用模式
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI1 触摸芯片的片选控制设置 PB7 */
    RCC_APB2PeriphClockCmd(RCC_LCD_TOUCH_TCS, ENABLE); //使能PB,PF端口时钟
    GPIO_InitStructure.GPIO_Pin = LCD_TOUCH_TCS_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //口线速度50MHZ
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;          //推挽输出模式
    GPIO_Init(LCD_TOUCH_TCS_GPIO, &GPIO_InitStructure);

    /* SPI1总线 配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                        //主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                    //8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                           //时钟极性 空闲状态时，SCK保持低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                         //时钟相位 数据采样从第一个时钟边沿开始
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                            //软件产生NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;  //波特率控制 SYSCLK/64
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                   //数据高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;                             //CRC多项式寄存器初始值为7
    SPI_Init(SPI1, &SPI_InitStructure);

    /* SPI1 使能 */
    SPI_Cmd(SPI1, ENABLE);
#endif

//    RCC_APB2PeriphClockCmd(RCC_LCD_TOUCH_PEN, ENABLE); //使能PB,PF端口时钟
//    GPIO_InitStructure.GPIO_Pin =  LCD_TOUCH_PEN_Pin;                 //触摸检测引脚
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         //口线速度50MHZ
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(LCD_TOUCH_PEN_GPIO, &GPIO_InitStructure);

    TP_Read_XY(&tp_dev.x, &tp_dev.y); //第一次读取初始化

    if (1)
    {
        return 0; //已经校准
    }
    else               //未校准
    {
        LCD_Clear(WHITE);//清屏
        TP_Adjust();  //屏幕校准

        sprintf(ubuf, "%8f %8f %4d %4d %d",
                tp_dev.xfac,
                tp_dev.yfac,
                tp_dev.xoff,
                tp_dev.yoff,
                tp_dev.touchtype); //将LCD ID打印到lcd_id数组。

        USART_OUT(DEBUG_USART, ubuf);
        // TP_Save_Adjdata();
    }
    //    TP_Get_Adjdata();
    return 1;
}
