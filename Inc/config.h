/*
	Config.h
	vFly项目飞控模块MiniARM2300芯片通用头文件
	提供基本类型定义，C标准头文件及核心文件包含，时钟设定。
	BUAA 2008-7-11
	张以成
*/
#ifndef __CONFIG_H 
#define __CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

//这一段无需改动
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char uint8;// 无符号8位整型变量  
typedef signed char int8;// 有符号8位整型变量  
typedef unsigned short uint16;// 无符号16位整型变量 
typedef signed short int16;// 有符号16位整型变量 
typedef unsigned int uint32;// 无符号32位整型变量 
typedef signed int int32;// 有符号32位整型变量 
typedef float fp32;// 单精度浮点数（32位长度） 
typedef double fp64;// 双精度浮点数（64位长度） 

#define GLOBAL	 extern	

//数据类型转换函数
#define U8toU16(x, y) ((uint16)(((uint16)(y))<<8|((uint16)(x))))
#define U8toS16(x, y) ((int16)(((uint16)(y))<<8|((uint16)(x))))

#define U8toU32(x1,x2,x3,x4) ((uint32)(((uint32)(x4))<<24|((uint32)(x3))<<16|((uint32)(x2))<<8|((uint16)(x1))))
#define U8toS32(x1,x2,x3,x4) ((int32)(((uint32)(x4))<<24|((uint32)(x3))<<16|((uint32)(x2))<<8|((uint16)(x1))))

#define U16toU8(x) (uint8*)(&(x))
#define S16toU8(x) (uint8*)(&(x))


//LPC2300系列芯片寄存器定义
#include    "LPC23xx.h"
//C语言标准头文件
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <setjmp.h>
#include <rt_misc.h>
//M2300核心头文件
#include "target.h"
#include	"IRQHander.h"

//时钟配置
#define	USE_USB				1

#define Fosc                12000000					//	OSC = 12MHz

#if USE_USB	
	
	#define Fusbclk	 48000000
	
	#define Fcclk	(Fosc * 4) 							// 主频 Fcclk = 48MHz
	#define Fcco	(Fusbclk * (USBCLKDivValue+1))		// 如果用了USB,则Fcco是Fcclk与Fusbclk在275~550M之间的最小公倍数
														// Fcco是Fcclk在275~550M之间的最小倍数
	#define Fpclk	(Fcclk / 2)
	
	#define PLL_NValue			1
	#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
	#define CCLKDivValue		(Fcco/Fcclk-1)
	#define USBCLKDivValue		5	
#else
	#define Fcclk	(Fosc * 4)							// 主频 Fcclk = 48MHz
	#define Fcco    (Fcclk* 6)
	
	#define Fpclk	(Fcclk / 4)
	
	#define PLL_NValue			1	
	#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
	#define CCLKDivValue		(Fcco/Fcclk-1)
	#define USBCLKDivValue		254		
#endif	


#define     BUZZER                  1 << 27             // P1.27控制蜂鸣器，低电平蜂鸣

#define     BUZZER_Set()            PINSEL3&=0xFFCFFFFF,IO1DIR|=(1<<27),IO1CLR|=(1<<27)  // 开蜂鸣器
#define     BUZZER_Clr()            IO1SET|=(1<<27) 

#ifdef __cplusplus
	}
#endif
#endif

