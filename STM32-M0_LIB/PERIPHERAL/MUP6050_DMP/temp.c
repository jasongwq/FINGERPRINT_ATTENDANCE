/***************************
自平衡系统 for Arduino
by 黑马
 数据采集    2012-03-21
 滤波        2012-03-23
 PID控制     2012-03-27
 PID整定
 运动控制
***************************/

#include <Wire.h>
#include <Servo.h> 

/************ 传感器参数 ***********/
#define Acc 0x1D           // ADXL345地址
#define Gyr 0x69           // L3G4200D地址
#define Mag 0x1E           // HMC5883L地址
#define Gry_offset -13     // 陀螺仪偏移量
#define Gyr_Gain 0.07      // 满量程2000dps时灵敏度(dps/digital)
#define pi 3.14159

/********** 互补滤波器参数 *********/
unsigned long preTime = 0; // 采样时间
float f_angle = 0.0;       // 滤波处理后的角度值

/*********** PID控制器参数 *********/
unsigned long lastTime;           // 前次时间
float ITerm, lastInput;    // 积分项、前次输入
float Output = 0.0;        // PID输出值

/*********** 马达控制参数 **********/
Servo servoL;              // 定义左驱
Servo servoR;              // 定义右驱
# define servoL_offset 90  // 左驱偏置
# define servoR_offset 90  // 右驱偏置

/************ 程序初始化 ***********/
void setup() {
    sensor_init();        // 配置传感器
    Serial.begin(19200);  // 开启串口以便监视数据
    servoL.attach(30);    // PIN30输出到左驱
    servoR.attach(32);    // PIN32输出到右驱
    delay(1000);
  }
/************** 主程序 *************/
void loop() {
    unsigned long now = millis();                           // 当前时间(ms)
    float dt = (now - preTime) / 1000.0;                    // 微分时间(s)
    preTime = now;                                          // 记录本次时间(ms)

/********** 读取姿态传感器 *********/
    float Y_Acc = gDat(Acc, 1);                             // 获取向前的加速度(digite)
    float Z_Acc = gDat(Acc, 2);                             // 获取向下的加速度(digite)
    float angleA = atan(Y_Acc / Z_Acc) * 180 / pi;          // 根据加速度分量得到的角度(degree)
    float omega =  Gyr_Gain * (gDat(Gyr, 0) +  Gry_offset); // 当前角速度(degree/s)

/*********** 一阶互补滤波 **********/
    float K = 0.8;                                          // 取值权重
    float A = K / (K + dt);                                 // 加权系数
    f_angle = A * (f_angle + omega * dt) + (1-A) * angleA;  // 互补滤波算法
/************ PID控制器 ***********/
    now = millis();                                         // 当前时间(ms)
    float TimeCh = (now - lastTime) / 1000.0;               // 采样时间(s)
    float Kp = 10.0, Ki = 0.0, Kd = 0.0;                    // 比例系数、积分系数、微分系数
    float SampleTime = 0.1;                                 // 采样时间(s)
    float Setpoint = -3.8;                                  // 设定目标值(degree)
    float outMin = -80.0, outMax = +80.0;                   // 输出上限、输出下限
    if(TimeCh >= SampleTime) {                              // 到达预定采样时间时
        float Input = f_angle;                              // 输入赋值
        float error = Setpoint - Input;                     // 偏差值
        ITerm+= (Ki * error * TimeCh);                      // 计算积分项
        ITerm = constrain(ITerm, outMin, outMax);           // 限定值域
        float DTerm = Kd * (Input - lastInput) / TimeCh;    // 计算微分项
        Output = Kp * error + ITerm - DTerm;                // 计算输出值
        Output = constrain(Output, outMin, outMax);         // 限定值域
        servoL.write(Output + servoL_offset);               // 控制左驱
        servoR.write(Output + servoR_offset);               // 控制右驱
        lastInput = Input;                                  // 记录输入值
        lastTime = now;                                     // 记录本次时间
    }
/************ 参数上传 ***********/
    Serial.print(now);          // 计算时间
    Serial.print(",");
    Serial.print(f_angle, 6);   // 偏离角度
    Serial.print(",");
    Serial.print(Output, 6);    // PID输出值
    Serial.print(";");
// 控制微分时间
    delay(10);
}

/***************************************
九轴姿态传感器寄存器读取函数
For Arduino, by 黑马
****************************************
        调用参数表
****************************************
  type    device      axis
                   0   1   2
ADXL345     Acc    x   y   z
L3G4200D    Gyr    x   y   z
HMC5883L    Mag    x   z   y
****************************************
Example
****************************************
00 #include <Wire.h>
01 #define Acc 0x1D;
02 #define Gyr 0x69;
03 #define Mag 0x1E;
04
05  void setup() {
06    sensor_init();
07    delay(1000);
08  }
09
10  void loop() {
11    int Z-Gyroscope;
12    Z-Gyroscope = gDat(Gyr, 2);
13    delay(50);
14  }
***************************************/

int gDat(int device, int axis) {
    int v;
    byte vL, vH, address;               // 存放byte数值
    if (device == Acc) address = 0x32;  // ADXL345的读数地址
    if (device == Gyr) address = 0xA8;  // L3G4200D的读数地址
    if (device == Mag) address = 0x03;  // HMC5883L的读数地址
    address = address + axis * 2;       // 数据偏移-坐标轴
    Wire.beginTransmission(device);     // 开始传输数据
    Wire.send(address);                 // 发送指针
    Wire.requestFrom(device, 2);        // 请求2 byte数据
    while(Wire.available() < 2);        // 成功获取前等待
    vL = Wire.receive();
    vH = Wire.receive();                // 读取数据
    Wire.endTransmission();             // 结束传输
    if (device == Mag) v = (vL << 8) | vH;
    else v = (vH << 8) | vL;            // 将byte数据合并为Int
    return v;                           // 返回读书值
}

/********************************************
配置九轴姿态传感器
********************************************/
void sensor_init() {

/************ 配置 ADXL345 ***********/
    writeRegister(Acc, 0x2D, 0b00001000);    // 测量模式

/************ 配置L3G4200D ***********/
    writeRegister(Gyr, 0x20, 0b00001111);    // 设置睡眠模式、x, y, z轴使能
    writeRegister(Gyr, 0x21, 0b00000000);    // 选择高通滤波模式和高通截止频率 
    writeRegister(Gyr, 0x22, 0b00000000);    // 设置中断模式
    writeRegister(Gyr, 0x23, 0b00110000);    // 设置量程(2000dps)、自检状态、SPI模式
    writeRegister(Gyr, 0x24, 0b00000000);    // FIFO & 高通滤波

/************ 配置HMC5883L ***********/
    writeRegister(Mag, 0x02, 0x00);          // 连续测量
}

/********************************************
寄存器写入函数
********************************************/
void writeRegister(int device, byte address, byte val) {
    Wire.beginTransmission(device);          // 写入的传感器
    Wire.send(address);                      // 写入地址
    Wire.send(val);                          // 写入值
    Wire.endTransmission();                  // 结束传输
}