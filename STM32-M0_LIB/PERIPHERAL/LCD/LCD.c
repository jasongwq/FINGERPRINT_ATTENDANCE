#include "lcd.h"
#include "usart.h"
#include "delay.h"
#include "font.h"

//����
void LCD_Clear(u16 color);
//���ù��λ��
void LCD_SetCursor(u16 Xpos, u16 Ypos);
//����
void LCD_DrawPoint(u16 x, u16 y);
//��ȡ��ĳ�����ɫֵ
u16 LCD_ReadPoint(u16 x, u16 y);
//��ʾ�ַ���        
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p);
//��ʾ����,��λΪ0,������ʾ
void LCD_ShowxNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);
//��ʾ����,��λΪ0,����ʾ 
void LCD_ShowNum(u16 x,u16 y,s32 num,u8 len,u8 size);
//��ָ��λ�û�һ��ָ����С��Բ
void Draw_Circle(u16 x0, u16 y0, u8 r);
//������
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2);
//����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2);
//��ָ����������䵥����ɫ
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);



//LCD�Ļ�����ɫ�ͱ���ɫ    
u16 POINT_COLOR = 0x0000; //������ɫ
u16 BACK_COLOR = 0xFFFF; //����ɫ

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;

#define CURVEL_LONG 280

/***������***/
//color :��ɫ
//cu		:����
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
           LCD_WR_REG(lcddev.wramcmd);                 				//׼��д������ʾ��
           LCD->LCD_RAM = WHITE;

           LCD_WR_CMD(lcddev.setxcmd, 20 + 100 + curve[num][index]);   		
           LCD_WR_CMD(lcddev.setycmd, 20 + (CURVEL_LONG - 2 - index));
           LCD_WR_REG(lcddev.wramcmd);                 				//׼��д������ʾ��
           LCD->LCD_RAM = color;
        }
    }
}
///***���߳�ʼ��***/
//void LCD_DrawCurve_Init(void)
//{
//    u32 i;
//    LCD_WR_CMD(0x0003, 0x1018);     ////18//30//10//28//08//00//20////ͼ����ʾ����Ϊ������  �е���  �еݼ�
//    LCD_WR_CMD(0x0210, 20);         //ˮƽ��ʾ����ʼ��ַ 0-239
//    LCD_WR_CMD(0x0211, 20 + 200 - 2); //ˮƽ��ʾ��������ַ 0-239
//    LCD_WR_CMD(0x0212, 20);         //��ֱ��ʾ����ʼ��ַ 0-399
//    LCD_WR_CMD(0x0213, 20 + CURVEL_LONG - 2); //��ֱ��ʾ��������ַ 0-399
//    LCD_WR_CMD(0x0200, 20);         //ˮƽ��ʾ����ַ
//    LCD_WR_CMD(0x0201, 20);         //��ֱ��ʾ����ַ
//    LCD_WR_REG(lcddev.wramcmd);     //׼��д������ʾ��
//    for (i = 0; i < 200 * CURVEL_LONG; i++)
//        LCD->LCD_RAM = YELLOW;
//}
//����
void LCD_Clear(u16 color)
{
    //LCD_Fill(0, 0, lcddev.width, lcddev.height, color);
    u32 index=0;      
    u32 totalpoint=lcddev.width;
    totalpoint*=lcddev.height;  //�õ��ܵ���
    LCD_SetCursor(0x00,0x0000); //���ù��λ�� 
    LCD_WriteRAM_Prepare();     //��ʼд��GRAM        
    for(index=0;index<totalpoint;index++)
    {
        LCD->LCD_RAM=color;    
    }
}
//���ù��λ��
//Xpos:������
//Ypos:������
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
        if(lcddev.dir==1)Xpos=lcddev.width-1-Xpos;//������ʵ���ǵ�תx,y����
        LCD_WriteReg(lcddev.setxcmd, Xpos);
        LCD_WriteReg(lcddev.setycmd, Ypos);
    }
}
//����
void LCD_DrawPoint(u16 x, u16 y)
{
    LCD_SetCursor(x, y);    //���ù��λ��
    LCD_WR_REG(lcddev.wramcmd);//׼��д������ʾ��//��ʼд��GRAM
    LCD->LCD_RAM = POINT_COLOR;
}
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(u16 x, u16 y, u16 color)
{
    LCD_WR_CMD(lcddev.setxcmd, x);
    LCD_WR_CMD(lcddev.setycmd, y);
    LCD->LCD_REG = lcddev.wramcmd;
    LCD->LCD_RAM = color;
}
// //���ٻ���
// //x,y:����
// //color:��ɫ
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
//         if(lcddev.dir==1)x=lcddev.width-1-x;//������ʵ���ǵ�תx,y����
//         LCD_WriteReg(lcddev.setxcmd,x);
//         LCD_WriteReg(lcddev.setycmd,y);
//     }            
//     LCD->LCD_REG=lcddev.wramcmd; 
//     LCD->LCD_RAM=color; 
// }    
//LCD������ʾ
void LCD_DisplayOn(void)
{                      
    if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X29);   //������ʾ
    else LCD_WriteReg(R7,0x0173);           //������ʾ
}    

//LCD�ر���ʾ
void LCD_DisplayOff(void)
{      
    if(lcddev.id==0X9341||lcddev.id==0X6804)LCD_WR_REG(0X28);   //�ر���ʾ
    else LCD_WriteReg(R7,0x0);//�ر���ʾ 
}   

//��ָ����������䵥����ɫ
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)
//color:Ҫ������ɫ
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color)
{
    u16 i, j;
    u16 xlen = 0;
    xlen = ex - sx + 1;
    for (i = sy; i <= ey; i++)
    {
        LCD_SetCursor(sx, i);                   //���ù��λ��
        LCD_WriteRAM_Prepare();                //��ʼд��GRAM
       for(j=0;j<xlen;j++)LCD_WR_DATA(color);  //���ù��λ��     
    }
}

//��ָ�����������ָ����ɫ��             
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 *color)
{  
    u16 height,width;
    u16 i,j;
    width=ex-sx+1;      //�õ����Ŀ��
    height=ey-sy+1;     //�߶�
    for(i=0;i<height;i++)
    {
        LCD_SetCursor(sx,sy+i);     //���ù��λ�� 
        LCD_WriteRAM_Prepare();     //��ʼд��GRAM
        for(j=0;j<width;j++)LCD->LCD_RAM=color[i*height+j];//д������ 
    }     
}  
//����
//x1,y1:�������
//x2,y2:�յ�����
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
{
    u16 t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, uRow, uCol;
    delta_x = x2 - x1; //������������
    delta_y = y2 - y1;
    uRow = x1;
    uCol = y1;
    if (delta_x > 0)incx = 1; //���õ�������
    else if (delta_x == 0)incx = 0; //��ֱ��
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }
    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; //ˮƽ��
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }
    if ( delta_x > delta_y)distance = delta_x; //ѡȡ��������������
    else distance = delta_y;
    for (t = 0; t <= distance + 1; t++ ) //�������
    {
        LCD_DrawPoint(uRow, uCol); //����
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
//������
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
{
    LCD_DrawLine(x1, y1, x2, y1);
    LCD_DrawLine(x1, y1, x1, y2);
    LCD_DrawLine(x1, y2, x2, y2);
    LCD_DrawLine(x2, y1, x2, y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void Draw_Circle(u16 x0, u16 y0, u8 r)
{
    int a, b;
    int di;
    a = 0; b = r;
    di = 3 - (r << 1);       //�ж��¸���λ�õı�־
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
        //ʹ��Bresenham�㷨��Բ
        if (di < 0)di += 4 * a + 6;
        else
        {
            di += 10 + 4 * (a - b);
            b--;
        }
    }
}
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����     " "--->"~"
//num:Ҫ��ʾ���ַ�:
//size:�����С 12/16
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(u16 x, u16 y, u8 num, u8 size, u8 mode)
{
    u8 temp, t1, t;
    u16 y0 = y;
    u16 colortemp = POINT_COLOR;
    //���ô���
    num = num - ' '; //�õ�ƫ�ƺ��ֵ
    if (!mode) //�ǵ��ӷ�ʽ
    {
        for (t = 0; t < size; t++)
        {
            if (size == 12)temp = asc2_1206[num][t]; //����1206����
            else temp = asc2_1608[num][t];       //����1608����
            for (t1 = 0; t1 < 8; t1++)
            {
                if (temp & 0x80)POINT_COLOR = colortemp;
                else POINT_COLOR = BACK_COLOR;
                LCD_DrawPoint(x, y);
                temp <<= 1;
                y++;
                if (x >= lcddev.width)
                {
                    POINT_COLOR = colortemp;    //��������
                    return;
                }
                if ((y - y0) == size)
                {
                    y = y0;
                    x++;
                    if (x >= lcddev.width)
                    {
                        POINT_COLOR = colortemp;    //��������
                        return;
                    }
                    break;
                }
            }
        }
    }
    else //���ӷ�ʽ
    {
        for (t = 0; t < size; t++)
        {
            if (size == 12)temp = asc2_1206[num][t]; //����1206����
            else temp = asc2_1608[num][t];       //����1608����
            for (t1 = 0; t1 < 8; t1++)
            {
                if (temp & 0x80)LCD_DrawPoint(x, y);
                temp <<= 1;
                y++;
                if (x >= lcddev.height)
                {
                    POINT_COLOR = colortemp;    //��������
                    return;
                }
                if ((y - y0) == size)
                {
                    y = y0;
                    x++;
                    if (x >= lcddev.width)
                    {
                        POINT_COLOR = colortemp;    //��������
                        return;
                    }
                    break;
                }
            }
        }
    }
    POINT_COLOR = colortemp;
}

//m^n����
//����ֵ:m^n�η�.
u32 LCD_Pow(u8 m, u8 n)
{
    u32 result = 1;
    while (n--)result *= m;
    return result;
}
//��ʾ����,��λΪ0,����ʾ
//x,y :�������  
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);  
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


//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);   
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
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
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ          
void LCD_ShowString(u16 x,u16 y,u16 width,u16 height,u8 size,char *p)
{         
    u8 x0=x;
    width+=x;
    height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }  
}



//��ILI93xx����������ΪGBR��ʽ��������д���ʱ��ΪRGB��ʽ��
//RRRRRGGGGGGBBBBB ��Ϊ BBBBBGGGGGGRRRRR ��ʽ
//ͨ���ú���ת��
//c:GBR��ʽ����ɫֵ
//����ֵ��RGB��ʽ����ɫֵ
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

//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(u8 i)
{
    while (i--);
}
//��ȡ��ĳ�����ɫֵ
//x,y:����
//����ֵ:�˵����ɫ
u16 LCD_ReadPoint(u16 x, u16 y)
{
    u16 r = 0, g = 0, b = 0;
    if (x >= lcddev.width || y >= lcddev.height)return 0; //�����˷�Χ,ֱ�ӷ���
    LCD_SetCursor(x, y);
    if (lcddev.id == 0X9341 || lcddev.id == 0X6804)LCD_WR_REG(0X2E); //9341/6804 ���Ͷ�GRAMָ��
    else LCD_WR_REG(R34);                           //����IC���Ͷ�GRAMָ��
    if (lcddev.id == 0X9320)opt_delay(2);           //FOR 9320,��ʱ2us
    if (LCD->LCD_RAM)r = 0;                         //dummy Read
    opt_delay(2);
    r = LCD->LCD_RAM;                               //ʵ��������ɫ
    if (lcddev.id == 0X9341) //9341Ҫ��2�ζ���
    {
        opt_delay(2);
        b = LCD->LCD_RAM;
        g = r & 0XFF; //����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
        g <<= 8;
    }
    else if (lcddev.id == 0X6804)r = LCD->LCD_RAM; //6804�ڶ��ζ�ȡ�Ĳ�����ʵֵ
    if (lcddev.id == 0X9325 || lcddev.id == 0X4535 || lcddev.id == 0X4531 || lcddev.id == 0X8989 || lcddev.id == 0XB505)return r; //�⼸��ICֱ�ӷ�����ɫֵ
    else if (lcddev.id == 0X9341)return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11)); //ILI9341��Ҫ��ʽת��һ��
    else return LCD_BGR2RGB(r);                                             //����IC
}
