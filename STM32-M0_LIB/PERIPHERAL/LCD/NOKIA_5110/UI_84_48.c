#include "UI_84_48.h"
#include "nokia_5110.h"
#include "gps_driver.h"
#include "stdio.h"
#include "sensor.h"





#define KEY_UP      2
#define KEY_DOWN    3
#define KEY_SURE    4

#define FUNC_AMOUNT 4 //菜单层数
#define PRINTF_SIZE 50 //菜单层数

u8 key_deep = 0;
__align(8) u8 dtbuf[PRINTF_SIZE];        //打印缓存器
u8(*last_func[FUNC_AMOUNT])(u8);
u8(*now_func)(u8);
u8(*temp_func)(u8);

//u8 gImage_0[1];
//u8 gImage_5[1];
//u8 gImage_10[1];
//u8 gImage_15[1];
//u8 gImage_20[1];
//u8 gImage_25[1];

//u8 gImage_30[1];
//u8 gImage_35[1];
//u8 gImage_40[1];
//u8 gImage_45[1];

extern unsigned char gImage_0[];
extern unsigned char gImage_5[];
extern unsigned char gImage_10[];
extern unsigned char gImage_15[];
extern unsigned char gImage_20[];
extern unsigned char gImage_25[];
extern unsigned char gImage_30[];
extern unsigned char gImage_35[];
extern unsigned char gImage_40[];
extern unsigned char gImage_45[];

unsigned char* Dir_Image[] =
{
    gImage_0,
    gImage_5,
    gImage_10,
    gImage_15,
    gImage_20,
    gImage_25,
    gImage_30,
    gImage_35,
    gImage_40,
    gImage_45
};

void UI_84_48_Init(void)
{
    LCD_write_english_string(0, 0, "  GPS  ");
}

extern nmea_msg gpsx;      //GPS信息

//显示GPS定位信息
void Gps_Msg_Show(void)
{
    sprintf((char*)dtbuf, "L:%d.%d %1c", gpsx.longitude / 100000, gpsx.longitude % 100000, gpsx.ewhemi); //得到经度字符串
    LCD_write_english_string(0, 1, (char*)dtbuf);

    sprintf((char*)dtbuf, "T:%d.%d %1c", gpsx.latitude / 100000, gpsx.latitude % 100000, gpsx.nshemi); //得到纬度字符串
    LCD_write_english_string(0, 2, (char*)dtbuf);

    sprintf((char*)dtbuf, "A:%d.%dm ", gpsx.altitude / 10, gpsx.altitude % 10);            //得到高度字符串
    LCD_write_english_string(0, 3, (char*)dtbuf);

    sprintf((char*)dtbuf, "S:%d.%dkm/h", gpsx.speed / 1000, gpsx.speed / 1000);          //得到速度字符串
    LCD_write_english_string(0, 4, (char*)dtbuf);

    if (gpsx.fixmode == 3)LCD_write_english_string(0, 5, "MODE: 3D ");//定位状态
    else if (gpsx.fixmode == 2)LCD_write_english_string(0, 5, "MODE: 2D ");
    else LCD_write_english_string(0, 5, "MODE: Fail ");
    //USART_OUT(USART1, "\r\n状态:%d\r\n", gpsx.fixmode); //发送接收到的数据到串口1
}

void Gps_Msg_Show_Time(void)
{
    sprintf((char*)dtbuf, "Valid  :%02d", gpsx.posslnum);         //用于定位的卫星数
    LCD_write_english_string(0, 1, (char*)dtbuf);
    sprintf((char*)dtbuf, "Visible:%02d", gpsx.svnum % 100);      //可见卫星数
    LCD_write_english_string(0, 2, (char*)dtbuf);
    sprintf((char*)dtbuf, "DATA:%02d/%02d/%02d", gpsx.utc.year % 100, gpsx.utc.month, gpsx.utc.date); //显示UTC日期
    LCD_write_english_string(0, 3, (char*)dtbuf);
    sprintf((char*)dtbuf, "TIME:%02d:%02d:%02d", gpsx.utc.hour + 8, gpsx.utc.min, gpsx.utc.sec); //显示UTC时间
    LCD_write_english_string(0, 4, (char*)dtbuf);
}
#define UI_MENU_NUM 4

u8 UI_TIME(u8 key)//GPS时间
{
    NOKIA_5110_LCD_Clear();
    key_deep = 1;
    LCD_write_english_string(6, 0, "--TIME-");
    Gps_Msg_Show_Time();
    return 0;
}
u8 UI_COORD(u8 key)//GPS信息
{
    NOKIA_5110_LCD_Clear();
    key_deep = 1;
    LCD_write_english_string(6, 0, "--COORD-");
    Gps_Msg_Show();
    return 0;
}
u8 SD_Browse(u8 key)
{
    return 0;
}
u8 POI_SET(u8 key)
{
    return 0;
}
u8 START_SET(u8 key)
{
    return 0;
}
u8 USB_SET(u8 key)
{
    return 0;
}
u8 Brightness(u8 key)//调整 背光亮度
{
    static u16 light = 200;
    if (key == KEY_DOWN && light < 889)light += 10;
    if (key == KEY_UP  && light > 10) light -= 10;
    key_deep = 1;
    TIM_SetCompare3(TIM2, light);
    sprintf((char*)dtbuf, "light:%2d", light);
    LCD_write_english_string(6, 0, (char*)dtbuf);
    return 0;
}
u8 Timeout(u8 key)
{
    return 0;
}
u8 contrast(u8 key)//背光对比度
{
    static u16 Contrast = 4 ;
    if (key == KEY_DOWN && Contrast < 5)Contrast++;
    if (key == KEY_UP  && Contrast > 0)Contrast--;
    Set_Nokia_Contrast(Contrast);
    key_deep = 1;
    sprintf((char*)dtbuf, "contrast:%2d", Contrast);
    LCD_write_english_string(6, 0, (char*)dtbuf);
    return 0;
}

u8 LCD_SET(u8 key)
{
#undef UI_MENU_NUM_
#define UI_MENU_NUM_ 3
    static u8 menu = 0;
    u8 i = 0, j = 0;
    u8(*func[6])(u8);
    key_deep = 0;
    func[0] = Brightness;
    func[1] = Timeout;
    func[2] = contrast;
    func[3] = LCD_SET;
    func[4] = LCD_SET;
    func[5] = LCD_SET;
    LCD_write_english_string(6, 0, "--LCD_SET--");
    if (key == KEY_DOWN && menu < UI_MENU_NUM_ - 1)menu++;
    if (key == KEY_UP  && menu > 0)menu--;
    switch (menu)
    {
    case 0:
    case 1:
    case 2:
    case 3: i++; LCD_write_english_string(6, i, "1.Brightness"); j = 1; if (i > UI_MENU_NUM)break;
    case 4: i++; LCD_write_english_string(6, i, "2.Timeout");   j = 2; if (i > UI_MENU_NUM)break;
    case 5: i++; LCD_write_english_string(6, i, "3.contrast");  j = 3; if (i > UI_MENU_NUM)break;
        //    case 6: i++; LCD_write_english_string(6, i, "4.***");       j = 4; if (i > UI_MENU_NUM)break;
        //    case 7: i++; LCD_write_english_string(6, i, "5.***");       j = 5; if (i > UI_MENU_NUM)break;
        //    case 8: i++; LCD_write_english_string(6, i, "6.***");       j = 6; if (i > UI_MENU_NUM)break;
    }
    j = menu + 1;
    if (j > 4)j = 4;
    temp_func = func[menu];
    LCD_write_english_string(0, j, "*");
    return 0;
}
u8 SYS_SET(u8 key)
{
    return 0;
}
typedef struct
{
    u8 light;//亮度
    u8 Contrast;//对比度
} LCD_CONFIG;
typedef struct
{
    u8 rate;//GPS更新频率
    u8 tp;//GPS时钟脉冲输出
} GPS_CONFIG;
GPS_CONFIG gps_config;

u8 GPS_RATE(u8 key)
{
    key_deep = 1;
    if (key == KEY_DOWN && gps_config.rate < 150)gps_config.rate ++;
    else if (key == KEY_UP  && gps_config.rate > 1)gps_config.rate --;
    else if (key == KEY_SURE)
    {
        sprintf((char*)dtbuf, "RATE:%2dHz", gps_config.rate * 2);
        LCD_write_english_string(6, 0, (char*)dtbuf);
        return 0;
    }
    else return 0;

    Ublox_Cfg_Rate(gps_config.rate, 1);

    sprintf((char*)dtbuf, "RATE:%2dHz", gps_config.rate * 2);
    LCD_write_english_string(6, 0, (char*)dtbuf);
    return 1;
}
u8 GPS_TP(u8 key)
{
    key_deep = 1;
    if (key == KEY_DOWN && gps_config.tp < 9)gps_config.tp ++;
    else if (key == KEY_UP  && gps_config.rate > 1) gps_config.tp --;
    else if (key == KEY_SURE)
    {
        sprintf((char*)dtbuf, "TP:%1d/10", gps_config.rate);
        LCD_write_english_string(6, 0, (char*)dtbuf);
        return 0;
    }
    else return 0;
    Ublox_Cfg_Tp(0xf4240, gps_config.tp * 100000, 1);

    sprintf((char*)dtbuf, "TP:%1d/10", gps_config.tp);
    LCD_write_english_string(6, 0, (char*)dtbuf);
    return 1;
}

//配置UBLOX NEO-6的更新速率
//配置UBLOX NEO-6的时钟脉冲输出
u8 GPS_SET(u8 key)
{
#undef UI_MENU_NUM_
#define UI_MENU_NUM_ 6
    static u8 menu = 0;
    u8 i = 0, j = 0;
    u8(*func[6])(u8);
    key_deep = 0;
    func[0] = GPS_RATE;
    func[1] = GPS_TP;
    func[2] = GPS_SET;
    func[3] = GPS_SET;
    func[4] = GPS_SET;
    func[5] = GPS_SET;
    NOKIA_5110_LCD_Clear();
    LCD_write_english_string(6, 0, "--SET--");
    if (key == KEY_DOWN && menu < UI_MENU_NUM_ - 1)menu++;
    if (key == KEY_UP  && menu > 0)menu--;
    switch (menu)
    {
    case 0:
    case 1:
    case 2:
    case 3: i++; LCD_write_english_string(6, i, "1.GPS_RATE");    j = 1; if (i > UI_MENU_NUM)break;
    case 4: i++; LCD_write_english_string(6, i, "2.GPS_TP"); j = 2; if (i > UI_MENU_NUM)break;
    case 5: i++; LCD_write_english_string(6, i, "3.***");    j = 3; if (i > UI_MENU_NUM)break;
    case 6: i++; LCD_write_english_string(6, i, "4.***");    j = 4; if (i > UI_MENU_NUM)break;
    case 7: i++; LCD_write_english_string(6, i, "5.***");    j = 5; if (i > UI_MENU_NUM)break;
    case 8: i++; LCD_write_english_string(6, i, "6.***");    j = 6; if (i > UI_MENU_NUM)break;
    }
    j = menu + 1;
    if (j > 4)j = 4;
    temp_func = func[menu];
    LCD_write_english_string(0, j, "*");
    return 0;
}

u8 UI_menu_SET(u8 key)
{
#undef UI_MENU_NUM_
#define UI_MENU_NUM_ 6
    static u8 menu = 0;
    u8 i = 0, j = 0;
    u8(*func[6])(u8);
    key_deep = 0;
    func[0] = POI_SET;
    func[1] = START_SET;
    func[2] = USB_SET;
    func[3] = LCD_SET;
    func[4] = SYS_SET;
    func[5] = GPS_SET;
    NOKIA_5110_LCD_Clear();
    LCD_write_english_string(6, 0, "--SET--");
    if (key == KEY_DOWN && menu < UI_MENU_NUM_ - 1)menu++;
    if (key == KEY_UP  && menu > 0)menu--;
    switch (menu)
    {
    case 0:
    case 1:
    case 2:
    case 3: i++; LCD_write_english_string(6, i, "1.POI_SET");    j = 1; if (i > UI_MENU_NUM)break;
    case 4: i++; LCD_write_english_string(6, i, "2.START_SET");  j = 2; if (i > UI_MENU_NUM)break;
    case 5: i++; LCD_write_english_string(6, i, "3.USB_SET");    j = 3; if (i > UI_MENU_NUM)break;
    case 6: i++; LCD_write_english_string(6, i, "4.LCD_SET");    j = 4; if (i > UI_MENU_NUM)break;
    case 7: i++; LCD_write_english_string(6, i, "5.SYS_SET");    j = 5; if (i > UI_MENU_NUM)break;
    case 8: i++; LCD_write_english_string(6, i, "6.GPS_SET");        j = 6; if (i > UI_MENU_NUM)break;
    }
    j = menu + 1;
    if (j > 4)j = 4;
    temp_func = func[menu];
    LCD_write_english_string(0, j, "*");
    return 0;
}
extern tg_HMC5883L_TYPE hmc5883l;

u8 fan (u8 temp)
{
    u8 temp0 = 0;
    u8 i2;
    for (i2 = 0; i2 < 8; i2++)
    {
        if (temp & 1)
            temp0 |= (0x80 >> i2);
        temp >>= 1;
    }
    return temp0;
}
void fan180 (unsigned char* tem, unsigned char* Dir_Image)
{
    u8 temp = 0;
    u8 temp0 = 0;
    u8 i, i1, i2;
    for (i = 0; i < 32; i++)
    {
        for (i1 = 0; i1 < 4; i1++)
        {
            temp0 = 0;
            temp = *(Dir_Image + 96 + i - 32 * i1);
            for (i2 = 0; i2 < 8; i2++)
            {
                if (temp & 1)
                    temp0 |= (0x80 >> i2);
                temp >>= 1;
            }
            *(tem + i + 32 * i1) = temp0;
        }
    }
}
void fan270 (unsigned char* tem, unsigned char* Dir_Image)
{
    u8 i, i1, i2, i3;
    u8 temp;
    unsigned char* temp1;
    unsigned char* temp0;
    for (i = 0; i < 4; i++)
    {
        for (i1 = 0; i1 < 4; i1++)
        {
            temp1 = tem + 8 * i1 + 32 * i;
            temp0 = Dir_Image + 24 + 32 * i1 - 8 * i;
            for (i2 = 0; i2 < 8; i2++)
                *(temp1 + i2) = 0;
            for (i2 = 0; i2 < 8; i2++)
            {
                temp = *(temp0 + i2);
                for (i3 = 0; i3 < 8; i3++)
                {
                    if (temp & 0x01)
                        *(temp1 + i3) |= (0x80 >> i2);
                    temp >>= 1;
                }
            }
        }
    }
}
void fan_180 (unsigned char* tem, unsigned char* Dir_Image)//左右颠倒
{
    u8 i, i1;
    for (i = 0; i < 16; i++)
    {
        for (i1 = 0; i1 < 4; i1++)
        {
            tem[     i + 32 * i1] = Dir_Image[31 - i + 32 * i1];
            tem[31 - i + 32 * i1] = Dir_Image[     i + 32 * i1];
        }
    }
}


u8 HMC5883L(u8 key)
{
    int16_t x, y;
    u16  j;
    u8 i;
    unsigned char tem[128];
    unsigned char tem1[128];

    float angle;
    NOKIA_5110_LCD_Clear();
    key_deep = 1;
    LCD_write_english_string(6, 0, "--HMC5883L-");
    //计算方向
    x = hmc5883l.hx;
    y = hmc5883l.hy;
    //求出方向
    if (x > 0x7fff)x -= 0xffff;
    if (y > 0x7fff)y -= 0xffff;
    angle = atan2(y, x) * (180 / 3.14159265) + 180; //160us计算时间
    hmc5883l.ha = (int16_t)(angle * 10); // 得到方向精确到0.1°

    sprintf((char*)dtbuf, "%3d.%1d", hmc5883l.ha / 10, hmc5883l.ha % 10);
    LCD_write_english_string(32,4,"DIR:");
		LCD_write_english_string(38,5, (char*)dtbuf);
		
    j = hmc5883l.ha / 10;
    switch (j / 45)
    {
    case 0://0~44
        for (i = 0; i < 128; i++)
            tem[i] = Dir_Image[(j % 45) / 5][i];
        break;
    case 1://45~89
        fan270(tem1, Dir_Image[8 - (j % 45) / 5]);
        fan_180(tem, tem1);
        break;
    case 2://90~134
        fan270(tem, Dir_Image[(j % 45) / 5]);
        fan_180(tem1, tem);
        fan180(tem, tem1);
        break;
    case 3://135~179
        fan180(tem, Dir_Image[8 - (j % 45) / 5]);
        break;
    case 4://180~224
        fan_180(tem1, Dir_Image[(j % 45) / 5]);
        fan180(tem, tem1);
        break;
    case 5://225~269
        fan270(tem1, Dir_Image[8 - (j % 45) / 5]);
        fan180(tem, tem1);
        break;
    case 6://270~314
        fan270(tem, Dir_Image[(j % 45) / 5]);
        break;
    case 7://315~359
        fan_180(tem, Dir_Image[8 - (j % 45) / 5]);
        break;
    }
    LCD_draw_bmp_pixel(0,42, tem, 32, 32);
    return 0;
}
u8 MAP(u8 key)
{
	

return 0;	
}

u8 UI_menu_main(u8 key)
{
#undef UI_MENU_NUM_
#define UI_MENU_NUM_ 6
    u8 i = 0, j = 0;
    static u8 menu = 0;
    u8(*func[6])(u8);
    key_deep = 0;
    func[0] = HMC5883L;
    func[1] = UI_TIME;
    func[2] = MAP;
    func[3] = UI_COORD;
    func[4] = UI_menu_SET;
    func[5] = SD_Browse;
    NOKIA_5110_LCD_Clear();
    LCD_write_english_string(6, 0, "-MENU--");
    if (key == KEY_DOWN && menu < UI_MENU_NUM_ - 1)menu++;
    if (key == KEY_UP  && menu > 0)menu--;
    switch (menu)
    {
    case 0:
    case 1:
    case 2:
    case 3: i++; LCD_write_english_string(6, i, "1.HMC5883L");  j = 1; if (i > UI_MENU_NUM)break;
    case 4: i++; LCD_write_english_string(6, i, "2.TIME");      j = 2; if (i > UI_MENU_NUM)break;
    case 5: i++; LCD_write_english_string(6, i, "3.MAP");       j = 3; if (i > UI_MENU_NUM)break;
    case 6: i++; LCD_write_english_string(6, i, "4.COORD");     j = 4; if (i > UI_MENU_NUM)break;
    case 7: i++; LCD_write_english_string(6, i, "5.SET");       j = 5; if (i > UI_MENU_NUM)break;
    case 8: i++; LCD_write_english_string(6, i, "6.SD_Browse"); j = 6; if (i > UI_MENU_NUM)break;
    }
    j = menu + 1;
    if (j > 4)j = 4;
    temp_func = func[menu];
    LCD_write_english_string(0, j, "*");
    return 0;
}
