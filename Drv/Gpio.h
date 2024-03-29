//修改自周立功相关文件
#ifndef _GPIO_H_
#define _GPIO_H_

//   -- P0 -- 
//该端口中P0.12、13、14、31仅LPC2378具有，在LPC2364/6/8中为无效引脚

#define P0       100000
#define P0_00         0  
#define P0_01         1
#define P0_02         2
#define P0_03         3
#define P0_04         4
#define P0_05         5
#define P0_06         6
#define P0_07         7
#define P0_08         8
#define P0_09         9
#define P0_10        10
#define P0_11        11
#define P0_12        12        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P0_13        13        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P0_14        14        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P0_15        15

#define P0_16        16
#define P0_17        17
#define P0_18        18
#define P0_19        19
#define P0_20        20
#define P0_21        21
#define P0_22        22
#define P0_23        23
#define P0_24        24
#define P0_25        25
#define P0_26        26
#define P0_27        27
#define P0_28        28
#define P0_29        29
#define P0_30        30
#define P0_31        31        //（仅LPC2378具有，在LPC2364/6/8中为无效位）

//   -- P1 -- 
//该端口中P1.02、03、05、06、07、11、12、13为无效引脚

#define P1        110000
#define P1_00        100
#define P1_01        101

//#define P1_02        102     //无效位
//#define P1_03        103     //无效位

#define P1_04        104

//#define P1_05        105     //无效位
//#define P1_06        106     //无效位
//#define P1_07        107     //无效位

#define P1_08        108
#define P1_09        109
#define P1_10        110

//#define P1_11        111     //无效位
//#define P1_12        112     //无效位
//#define P1_13        113     //无效位

#define P1_14        114
#define P1_15        115

#define P1_16        116
#define P1_17        117
#define P1_18        118
#define P1_19        119
#define P1_20        120
#define P1_21        121
#define P1_22        122
#define P1_23        123
#define P1_24        124
#define P1_25        125
#define P1_26        126
#define P1_27        127
#define P1_28        128
#define P1_29        129
#define P1_30        130
#define P1_31        131

//   -- P2 -- 
//该端口中P2.14-31为无效引脚

#define P2        120000
#define P2_00        200
#define P2_01        201
#define P2_02        202
#define P2_03        203
#define P2_04        204
#define P2_05        205
#define P2_06        206
#define P2_07        207
#define P2_08        208
#define P2_09        209
#define P2_10        210
#define P2_11        211
#define P2_12        212
#define P2_13        213
//#define P2_14        214     //无效位
//#define P2_15        215     //无效位

//#define P2_16        216     //无效位
//#define P2_17        217     //无效位
//#define P2_18        218     //无效位
//#define P2_19        219     //无效位
//#define P2_20        220     //无效位
//#define P2_21        221     //无效位
//#define P2_22        222     //无效位
//#define P2_23        223     //无效位
//#define P2_24        224     //无效位
//#define P2_25        225     //无效位
//#define P2_26        226     //无效位
//#define P2_27        227     //无效位
//#define P2_28        228     //无效位
//#define P2_29        229     //无效位
//#define P2_30        230     //无效位
//#define P2_31        231     //无效位

//   -- P3 -- 
//该端口中P3.08-22，27-31为无效引脚，
//    引脚P3.00-07，23、24仅LPC2378具有，在LPC2364/6/8中为无效引脚

#define P3        130000
#define P3_00        300        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_01        301        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_02        302        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_03        303        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_04        304        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_05        305        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_06        306        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P3_07        307        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
//#define P3_08        308      //无效位
//#define P3_09        309      //无效位
//#define P3_10        310      //无效位
//#define P3_11        311      //无效位
//#define P3_12        312      //无效位
//#define P3_13        313      //无效位
//#define P3_14        314      //无效位
//#define P3_15        315      //无效位 

//#define P3_16        316      //无效位
//#define P3_17        317      //无效位
//#define P3_18        318      //无效位
//#define P3_19        319      //无效位
//#define P3_20        320      //无效位
//#define P3_21        321      //无效位
//#define P3_22        322      //无效位
#define P3_23        323
#define P3_24        324
#define P3_25        325
#define P3_26        326
//#define P3_27        327      //无效位
//#define P3_28        328      //无效位
//#define P3_29        329      //无效位
//#define P3_30        330      //无效位
//#define P3_31        331      //无效位

//   -- P4 -- 
//该端口中P4.16-23，26、27为无效引脚，
//    引脚P3.00-15，24、25、30、31仅LPC2378具有，在LPC2364/6/8中为无效引脚

#define P4        140000
#define P4_00        400        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_01        401        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_02        402        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_03        403        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_04        404        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_05        405        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_06        406        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_07        407        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_08        408        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_09        409        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_10        410        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_11        411        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_12        412        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_13        413        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_14        414        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_15        415        //（仅LPC2378具有，在LPC2364/6/8中为无效位）

//#define P4_16        416      //无效位
//#define P4_17        417      //无效位
//#define P4_18        418      //无效位
//#define P4_19        419      //无效位
//#define P4_20        420      //无效位
//#define P4_21        421      //无效位
//#define P4_22        422      //无效位
//#define P4_23        423      //无效位
#define P4_24        424        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_25        425        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
//#define P4_26        426      //无效位
//#define P4_27        427      //无效位
#define P4_28        428
#define P4_29        429
#define P4_30        430        //（仅LPC2378具有，在LPC2364/6/8中为无效位）
#define P4_31        431        //（仅LPC2378具有，在LPC2364/6/8中为无效位）


/*******************************************************************************************
** 函数名称: int32 GpioSetLOW(uint32 arg)
** 函数功能: 低速GPIO引脚为置"1"
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: OPERATE_SUCCESS —〉操作成功
**           负值            —〉操作失败
*******************************************************************************************/
int32 GpioSetLOW(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioSetHI(uint32 arg)
** 函数功能: 高速GPIO引脚为置"1"
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: OPERATE_SUCCESS —〉操作成功
**           负值            —〉操作失败
*******************************************************************************************/
int32 GpioSetHI(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioClrLOW(uint32 arg)
** 函数功能: 低速GPIO引脚置为"0"
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: OPERATE_SUCCESS —〉操作成功
**           负值            —〉操作失败
*******************************************************************************************/
int32 GpioClrLOW(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioClrHI(uint32 arg)
** 函数功能: 高速GPIO引脚置为"0"
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: OPERATE_SUCCESS —〉操作成功
**           负值            —〉操作失败
*******************************************************************************************/
int32 GpioClrHI(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioGetLOW(uint32 arg)
** 函数功能: 读取低速GPIO端口电平
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: "0" —〉端口为低电平
**           "1" —〉端口为高电平
**           负值—〉操作失败
*******************************************************************************************/
int32 GpioGetLOW(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioGetHI(uint32 arg)
** 函数功能: 读取高速GPIO端口电平
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: "0" —〉端口为低电平
**           "1" —〉端口为高电平
**           负值—〉操作失败
*******************************************************************************************/
int32 GpioGetHI(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioCplLOW(uint32 arg)
** 函数功能: 取反低速GPIO端口电平
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: "0" —〉端口被置为低电平
**           "1" —〉端口被置为高电平
**           负值—〉操作失败
*******************************************************************************************/
int32 GpioCplLOW(uint32 arg);

/*******************************************************************************************
** 函数名称: int32 GpioCplHI(uint32 arg)
** 函数功能: 取反高速GPIO端口电平
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: "0" —〉端口被置为低电平
**           "1" —〉端口被置为高电平
**           负值—〉操作失败
*******************************************************************************************/
int32 GpioCplHI(uint32 arg);

/*******************************************************************************************
** 函数名称: void GpioSpeedHigh(void)
** 函数功能: 设置P0或P1口为高速访问模式
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: OPERATE_SUCCESS —〉操作成功
**           负值            —〉操作失败
** 说    明: P2、P3、P4硬件只支持高速GPIO，
**           故本函数仅用于设置P0、P1口
*******************************************************************************************/
#define GpioSpeedHigh()    (SCS |= 0x01)          //GPIOM位设为1，使能P0、P1为高速访问模式
    
/*******************************************************************************************
** 函数名称: void GpioSpeedLow(void)
** 函数功能: 设置P0或P1口为低速访问模式
** 入口参数: arg —〉GPIO引脚宏定义
** 返 回 值: OPERATE_SUCCESS —〉操作成功
**           负值            —〉操作失败
** 说    明: P2、P3、P4硬件只支持高速GPIO，
**           故本函数仅用于设置P0、P1口
*******************************************************************************************/
#define GpioSpeedLow()    (SCS &= ~0x01)          //GPIOM位设为0，设置P0、P1为低速访问模式
 
#endif

