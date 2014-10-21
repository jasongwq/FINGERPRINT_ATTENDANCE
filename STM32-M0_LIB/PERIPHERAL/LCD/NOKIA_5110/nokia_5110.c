#include "nokia_5110.h"
#include "english_6x8_pixel.h"//ascll
#include "write_chinese_string_pixel.h"//����
#include "delay.h"//����


static void _GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); //ʹ��Px�˿�ʱ��
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;   //PC.7
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;        //����
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

}

/*-----------------------------------------------------------------------
LCD_write_byte    : ʹ��SPI�ӿ�д���ݵ�LCD
���������data    ��д������ݣ�
          command ��д����/����ѡ��
-----------------------------------------------------------------------*/
static void LCD_write_byte(unsigned char dat, unsigned char command)
{
    unsigned char i;
    LCD_CE = 0;

    if (command == 0)
        LCD_DC = 0;
    else
        LCD_DC = 1;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
            SDIN = 1;
        else
            SDIN = 0;
        SCLK = 0;
        dat = dat << 1;
        SCLK = 1;
    }
    LCD_CE = 1;
}
void Set_Nokia_Contrast(u8 Contrast)
{
	  if(Contrast>5)Contrast=5;	
    LCD_write_byte(0x21, 0);    // ʹ����չ��������LCDģʽ
	  LCD_write_byte(0xc0 | (1 << Contrast), 0); // ����ƫ�õ�ѹ
	  LCD_write_byte(0x20, 0);    // ʹ�û�������
}

/*-----------------------------------------------------------------------
LCD_init          : 3310LCD��ʼ��
��д����          ��2004-8-10
����޸�����      ��2004-8-10
-----------------------------------------------------------------------*/
void NOKIA_5110_LCD_Init(void)
{
    _GPIO_Init();

    // ����һ����LCD��λ�ĵ͵�ƽ����
    LCD_RST = 0;
    delay_us(1);
    LCD_RST = 1;
    // �ر�LCD
    LCD_CE = 0;
    delay_us(1);
    // ʹ��LCD
    LCD_CE = 1;
    delay_us(1);

    /*��������
    0   ˮƽ ����ָ�
    1        ��չ...
    2 ��ֱ ����ָ�
    3      ��չ...
    4 ʡ��ģʽ
    5
    6
    7
    */
    //    LCD_write_byte(0x21, 0);    // ʹ��ʡ��ģʽ
    LCD_write_byte(0x21, 0);    // ʹ����չ��������LCDģʽ
    //LCD_write_byte(0x20, 0);    // ʹ�û�����������LCDģʽ
    //0~5 5�Աȶ���� 0�Աȶ����
    LCD_write_byte(0xc0 | (1 << 4), 0); // ����ƫ�õ�ѹ
    /*
    +0x00~0x03
    0
    1
    9
    17
    24
    */
    LCD_write_byte((0x04 + 0x00), 0);  // �¶�У��
    /*
    0X10~0X17
    BS2 BS1 BS0 n �Ƽ������
    0    0   0  7   1:100
    0    0   1  6   1:80
    0    1   0  5   1:65 / 1:65
    0    1   1  4   1:48
    1    0   0  3   1:40 / 1:34
    1    0   1  2   1:24
    1    1   0  1   1:18 / 1:16
    1    1   1  0   1:10 / 1:9 / 1:8
    */
    LCD_write_byte(0x10 + 0x00, 0);  // 1:48 ����ƫ��ϵͳ
    LCD_write_byte(0x20, 0);    // ʹ�û�������
    NOKIA_5110_LCD_Clear();     // ����
    LCD_write_byte(0x0c, 0);    // �趨��ʾģʽ��������ʾ
    NOKIA_5110_LCD_Clear();
    // �ر�LCD
    LCD_CE = 0;
}

/*-----------------------------------------------------------------------
LCD_clear         : LCD��������
-----------------------------------------------------------------------*/
void NOKIA_5110_LCD_Clear(void)
{
    unsigned int i;
    LCD_write_byte(0x0c, 0);
    LCD_write_byte(0x80, 0);

    for (i = 0; i < 504; i++)
        LCD_write_byte(0, 1);
}

/*-----------------------------------------------------------------------
LCD_set_XY        : ����LCD���꺯��
���������X       ��0��83
          Y       ��0��5
-----------------------------------------------------------------------*/
static void LCD_set_XY(unsigned char X, unsigned char Y)
{
    LCD_write_byte(0x40 | Y, 0);        // column
    LCD_write_byte(0x80 | X, 0);            // row
}

/*-----------------------------------------------------------------------
LCD_write_char    : ��ʾӢ���ַ�
���������c       ����ʾ���ַ���
-----------------------------------------------------------------------*/
void LCD_write_char(unsigned char c)
{
    unsigned char line;
    c -= 32;
    for (line = 0; line < 6; line++)
        LCD_write_byte(font6x8[c][line], 1);
}

/*-----------------------------------------------------------------------
LCD_write_english_String  : Ӣ���ַ�����ʾ����
���������*s      ��Ӣ���ַ���ָ��;
          X��Y    : ��ʾ�ַ�����λ��,x 0-83 ,y 0-5
-----------------------------------------------------------------------*/
void LCD_write_english_string(unsigned char X, unsigned char Y, char* s)
{
    LCD_set_XY(X, Y);
    while (*s)
    {
        LCD_write_char(*s);
        s++;
    }
}
/*-----------------------------------------------------------------------
LCD_write_chinese_string: ��LCD����ʾ����
���������X��Y    ����ʾ���ֵ���ʼX��Y���ꣻ
          ch_with �����ֵ���Ŀ��
          num     ����ʾ���ֵĸ�����
          line    �����ֵ��������е���ʼ����
          row     ��������ʾ���м��
���ԣ�
    LCD_write_chi(0,0,12,7,0,0);
    LCD_write_chi(0,2,12,7,0,0);
    LCD_write_chi(0,4,12,7,0,0);
-----------------------------------------------------------------------*/
void LCD_write_chinese_string(unsigned char X, unsigned char Y,
                              unsigned char ch_with, unsigned char num,
                              unsigned char line, unsigned char row)
{
    unsigned char i, n;

    LCD_set_XY(X, Y);                            //���ó�ʼλ��

    for (i = 0; i < num;)
    {
        for (n = 0; n < ch_with * 2; n++)        //дһ������
        {
            if (n == ch_with)                    //д���ֵ��°벿��
            {
                if (i == 0) LCD_set_XY(X, Y + 1);
                else
                    LCD_set_XY((X + (ch_with + row)*i), Y + 1);
            }
            LCD_write_byte(write_chinese[line + i][n], 1);
        }
        i++;
        LCD_set_XY((X + (ch_with + row)*i), Y);
    }
}



/*-----------------------------------------------------------------------
LCD_draw_map      : λͼ���ƺ���
���������X��Y    ��λͼ���Ƶ���ʼX��Y���ꣻ
          *map    ��λͼ�������ݣ�
          Pix_x   ��λͼ���أ�����
          Pix_y   ��λͼ���أ���
-----------------------------------------------------------------------*/
void LCD_draw_bmp_pixel(unsigned char X, unsigned char Y, unsigned char* map,
                        unsigned char Pix_x, unsigned char Pix_y)
{
    unsigned int i, n;
    unsigned char row;

    if (Pix_y % 8 == 0) row = Pix_y / 8; //����λͼ��ռ����
    else row = Pix_y / 8 + 1;

    for (n = 0; n < row; n++)
    {
        LCD_set_XY(X, Y);
        for (i = 0; i < Pix_x; i++)
        {
            LCD_write_byte(map[i + n * Pix_x], 1);
        }
        Y++;                         //����
    }
}




