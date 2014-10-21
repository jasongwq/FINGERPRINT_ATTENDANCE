#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "font.h"

//清屏
void LCD_Clear(u16 color);
//设置光标位置
void LCD_SetCursor(u16 Xpos, u16 Ypos);
//画点
void LCD_DrawPoint(u16 x, u16 y);
//读取个某点的颜色值
u16 LCD_ReadPoint(u16 x, u16 y);
//显示字符串        
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);
//显示数字,高位为0,还是显示
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
//显示数字,高位为0,则不显示 
void LCD_ShowNum(u16 x,u16 y,s32 num,u8 len,u8 size);
//在指定位置画一个指定大小的圆
void Draw_Circle(u16 x0, u16 y0, u8 r);
//画矩形
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
//画线
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
//在指定区域内填充单个颜色
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);



//LCD的画笔颜色和背景色    
u16 POINT_COLOR = 0x0000; //画笔颜色
u16 BACK_COLOR = 0xFFFF; //背景色

//管理LCD重要参数
//默认为竖屏
_lcd_dev lcddev;

#define CURVEL_LONG 280

/***画曲线***/
//color :颜色
//cu		:数据
void LCD_DrawCurve(u8 num,u16 color, s8 cu)
{
    u32 index = 0;
		static s8 curve[3][CURVEL_LONG] = {0};
    curve[num][CURVEL_LONG - 1] = cu;
    for (index = 0; index < CURVEL_LONG - 1; index++)
        curve[num][index] = curve[num][index + 1];
    for (index = 1; index < CURVEL_LONG + 1; index++)
    {
        if (curve[num][index - 1] != 0)
        {
           LCD_WR_CMD(lcddev.setxcmd, 20 + 100 + curve[num][index - 1]);
           LCD_WR_CMD(lcddev.setycmd, 20 + (CURVEL_LONG - 2 - index));
           LCD_WR_REG(lcddev.wramcmd);                 				//准备写数据显示区
           LCD->LCD_RAM = WHITE;

           LCD_WR_CMD(lcddev.setxcmd, 20 + 100 + curve[num][index]);   		
           LCD_WR_CMD(lcddev.setycmd, 20 + (CURVEL_LONG - 2 - index));
           LCD_WR_REG(lcddev.wramcmd);                 				//准备写数据显示区
           LCD->LCD_RAM = color;
        }
    }
}
///***曲线初始化***/
//void LCD_DrawCurve_Init(void)
//{
//    u32 i;
//    LCD_WR_CMD(0x0003, 0x1018);     ////18//30//10//28//08//00//20////图像显示方向为左下起  行递增  列递减
//    LCD_WR_CMD(0x0210, 20);         //水平显示区起始地址 0-239
//    LCD_WR_CMD(0x0211, 20 + 200 - 2); //水平显示区结束地址 0-239
//    LCD_WR_CMD(0x0212, 20);         //垂直显示区起始地址 0-399
//    LCD_WR_CMD(0x0213, 20 + CURVEL_LONG - 2); //垂直显示区结束地址 0-399
//    LCD_WR_CMD(0x0200, 20);         //水平显示区地址
//    LCD_WR_CMD(0x0201, 20);         //垂直显示区地址
//    LCD_WR_REG(lcddev.wramcmd);     //准备写数据显示区
//    for (i = 0; i < 200 * CURVEL_LONG; i++)
//        LCD->LCD_RAM = YELLOW;
//}
//清屏
void LCD_Clear(u16 color)
{
    //LCD_Fill(0, 0, lcddev.width, lcddev.height, color);
    u32 index=0;      
    u32 totalpoint=lcddev.width;
    totalpoint*=lcddev.height;  //得到总点数
    LCD_SetCursor(0x00,0x0000); //设置光标位置 
    LCD_WriteRAM_Prepare();     //开始写入GRAM        
    for(index=0;index<totalpoint;index++)
    {
        LCD->LCD_RAM=color;    
    }
}
//设置光标位置
//Xpos:横坐标
//Ypos:纵坐标
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{    
    if(lcddev.id==0X9341||lcddev.id==0X6804)
    {           
        LCD_WR_REG(lcddev.setxcmd);
        LCD_WR_DATA(Xpos>>8);
        LCD_WR_DATA(Xpos&0XFF);
        LCD_WR_REG(lcddev.setycmd);
        LCD_WR_DATA(Ypos>>8);
        LCD_WR_DATA(Ypos&0XFF);
    }else
    {
        if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//横屏其实就是调转x,y坐标
        LCD_WriteReg(lcddev.setxcmd, Xpos);
        LCD_WriteReg(lcddev.setycmd, Ypos);
    }
}
//画点
void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_SetCursor(x, y);    //设置光标位置
    LCD_WR_REG(lcddev.wramcmd);//准备写数据显示区//开始写入GRAM
    LCD->LCD_RAM = POINT_COLOR;
}
//快速画点
//x,y:坐标
//color:颜色
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color)
{
    LCD_WR_CMD(lcddev.setxcmd, x);
    LCD_WR_CMD(lcddev.setycmd, y);
    LCD->LCD_REG = lcddev.wramcmd;
    LCD->LCD_RAM = color;
}
// //快速画点
// //x,y:坐标
// //color:颜色
// void LCD_Fast_DrawPoint(u16 x,u16 y,u16 color)
// {      
//     if(lcddev.id==0X9341||lcddev.id==0X6804)
//     {           
//         LCD_WR_REG(lcddev.setxcmd); 
//         LCD_WR_DATA(x>>8); 
//         LCD_WR_DATA(x&0XFF);     
//         LCD_WR_REG(lcddev.setycmd); 
//         LCD_WR_DATA(y>>8); 
//         LCD_WR_DATA(y&0XFF);
//     }else
//     {
//         if(lcddev.dir==1)x=lcddev.width-1-x;//横屏其实就是调转x,y坐标
//         LCD_WriteReg(lcddev.setxcmd,x);
//         LCD_WriteReg(lcddev.setycmd,y);
//     }            
//     LCD->LCD_REG=lcddev.wramcmd; 
//     LCD->LCD_RAM=color; 
// }    
//LCD开启显示
void LCD_DisplayOn(void)
{                      
    if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X29);   //开启显示
    else LCD_WriteReg(R7,0x0173);           //开启显示
}    

//LCD关闭显示
void LCD_DisplayOff(void)
{      
    if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X28);   //关闭显示
    else LCD_WriteReg(R7,0x0);//关闭显示 
}   

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)
//color:要填充的颜色
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
    u16 i, j;
    u16 xlen = 0;
    xlen = ex - sx + 1;
    for (i = sy; i <= ey; i++)
    {
        LCD_SetCursor(sx, i);                   //设置光标位置
        LCD_WriteRAM_Prepare();                //开始写入GRAM
       for(j=0;j<xlen;j++)LCD_WR_DATA(color);  //设置光标位置     
    }
}

//在指定区域内填充指定颜色块             
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
    u16 height,width;
    u16 i,j;
    width=ex-sx+1;      //得到填充的宽度
    height=ey-sy+1;     //高度
    for(i=0;i<height;i++)
    {
        LCD_SetCursor(sx,sy+i);     //设置光标位置 
        LCD_WriteRAM_Prepare();     //开始写入GRAM
        for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//写入数据 
    }     
}  
//画线
//x1,y1:起点坐标
//x2,y2:终点坐标
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //计算坐标增量
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)incx = 1; //设置单步方向
    else if (delta_x == 0)incx = 0; //垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; //水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if ( delta_x > delta_y)distance = delta_x; //选取基本增量坐标轴
    else distance = delta_y;
    for (t = 0; t <= distance + 1; t++ ) //画线输出
    {
        LCD_DrawPoint(uRow, uCol); //画点
        xerr += delta_x ;
        yerr += delta_y ;
        if (xerr > distance)
        {
            xerr -= distance;
            uRow += incx;
        }
        if (yerr > distance)
        {
            yerr -= distance;
            uCol += incy;
        }
    }
}
//画矩形
//(x1,y1),(x2,y2):矩形的对角坐标
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}
//在指定位置画一个指定大小的圆
//(x,y):中心点
//r    :半径
void Draw_Circle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a = 0; b = r;
    di = 3 - (r << 1);       //判断下个点位置的标志
    while (a <= b)
    {
        LCD_DrawPoint(x0 + a, y0 - b);        //5
        LCD_DrawPoint(x0 + b, y0 - a);        //0
        LCD_DrawPoint(x0 + b, y0 + a);        //4
        LCD_DrawPoint(x0 + a, y0 + b);        //6
        LCD_DrawPoint(x0 - a, y0 + b);        //1
        LCD_DrawPoint(x0 - b, y0 + a);
        LCD_DrawPoint(x0 - a, y0 - b);        //2
        LCD_DrawPoint(x0 - b, y0 - a);        //7
        a++;
        //使用Bresenham算法画圆
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}
//在指定位置显示一个字符
//x,y:起始坐标     " "--->"~"
//num:要显示的字符:
//size:字体大小 12/16
//mode:叠加方式(1)还是非叠加方式(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u16 colortemp = POINT_COLOR;
    //设置窗口
    num = num - ' '; //得到偏移后的值
    if (!mode) //非叠加方式
    {
        for (t = 0; t < size; t++)
        {
            if (size == 12)temp = asc2_1206[num][t]; //调用1206字体
            else temp = asc2_1608[num][t];       //调用1608字体
            for (t1 = 0; t1 < 8; t1++)
            {
                if (temp & 0x80)POINT_COLOR = colortemp;
                else POINT_COLOR = BACK_COLOR;
                LCD_DrawPoint(x, y);
                temp <<= 1;
                y++;
                if (x >= lcddev.width)
                {
                    POINT_COLOR = colortemp;    //超区域了
                    return;
                }
                if ((y - y0) == size)
                {
                    y = y0;
                    x++;
                    if (x >= lcddev.width)
                    {
                        POINT_COLOR = colortemp;    //超区域了
                        return;
                    }
                    break;
                }
            }
        }
    }
    else //叠加方式
    {
        for (t = 0; t < size; t++)
        {
            if (size == 12)temp = asc2_1206[num][t]; //调用1206字体
            else temp = asc2_1608[num][t];       //调用1608字体
            for (t1 = 0; t1 < 8; t1++)
            {
                if (temp & 0x80)LCD_DrawPoint(x, y);
                temp <<= 1;
                y++;
                if (x >= lcddev.height)
                {
                    POINT_COLOR = colortemp;    //超区域了
                    return;
                }
                if ((y - y0) == size)
                {
                    y = y0;
                    x++;
                    if (x >= lcddev.width)
                    {
                        POINT_COLOR = colortemp;    //超区域了
                        return;
                    }
                    break;
                }
            }
        }
    }
    POINT_COLOR = colortemp;
}

//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)result *= m;
    return result;
}
//显示数字,高位为0,则不显示
//x,y :起点坐标  
//len :数字的位数
//size:字体大小
//color:颜色 
//num:数值(0~4294967295);  
void LCD_ShowNum(u16 x,u16 y,s32 num,u8 len,u8 size)
{           
    u8 t,temp;
    u8 enshow=0;                           
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
                continue;
            }else enshow=1; 
             
        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
    }
} 


//显示数字,高位为0,还是显示
//x,y:起点坐标
//num:数值(0~999999999);   
//len:长度(即要显示的位数)
//size:字体大小
//mode:
//[7]:0,不填充;1,填充0.
//[6:1]:保留
//[0]:0,非叠加显示;1,叠加显示.
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{  
    u8 t,temp;
    u8 enshow=0;                           
    for(t=0;t<len;t++)
    {
        temp=(num/LCD_Pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
                else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
                continue;
            }else enshow=1; 
             
        }
        LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
    }
}
//显示字符串
//x,y:起点坐标
//width,height:区域大小  
//size:字体大小
//*p:字符串起始地址          
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p)
{         
    u8 x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//退出
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}



//从ILI93xx读出的数据为GBR格式，而我们写入的时候为RGB格式。
//RRRRRGGGGGGBBBBB 改为 BBBBBGGGGGGRRRRR 格式
//通过该函数转换
//c:GBR格式的颜色值
//返回值：RGB格式的颜色值
#define  ili9325_BGR2RGB LCD_BGR2RGB
u16 LCD_BGR2RGB(u16 c)
{
    u16  r, g, b, rgb;
    b = (c >> 0) & 0x1f;
    g = (c >> 5) & 0x3f;
    r = (c >> 11) & 0x1f;
    rgb = (b << 11) + (g << 5) + (r << 0);
    return (rgb);
}

//当mdk -O1时间优化时需要设置
//延时i
void opt_delay(u8 i)
{
    while (i--);
}
//读取个某点的颜色值
//x,y:坐标
//返回值:此点的颜色
u16 LCD_ReadPoint(u16 x, u16 y)
{
    u16 r = 0, g = 0, b = 0;
    if (x >= lcddev.width || y >= lcddev.height)return 0; //超过了范围,直接返回
    LCD_SetCursor(x, y);
    if (lcddev.id == 0X9341 || lcddev.id == 0X6804)LCD_WR_REG(0X2E); //9341/6804 发送读GRAM指令
    else LCD_WR_REG(R34);                           //其他IC发送读GRAM指令
    if (lcddev.id == 0X9320)opt_delay(2);           //FOR 9320,延时2us
    if (LCD->LCD_RAM)r = 0;                         //dummy Read
    opt_delay(2);
    r = LCD->LCD_RAM;                               //实际坐标颜色
    if (lcddev.id == 0X9341) //9341要分2次读出
    {
        opt_delay(2);
        b = LCD->LCD_RAM;
        g = r & 0XFF; //对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
        g <<= 8;
    }
    else if (lcddev.id == 0X6804)r = LCD->LCD_RAM; //6804第二次读取的才是真实值
    if (lcddev.id == 0X9325 || lcddev.id == 0X4535 || lcddev.id == 0X4531 || lcddev.id == 0X8989 || lcddev.id == 0XB505)return r; //这几种IC直接返回颜色值
    else if (lcddev.id == 0X9341)return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); //ILI9341需要公式转换一下
    else return LCD_BGR2RGB(r);                                             //其他IC
}
