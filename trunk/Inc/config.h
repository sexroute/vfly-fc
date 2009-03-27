/*
	Config.h
	vFly��Ŀ�ɿ�ģ��MiniARM2300оƬͨ��ͷ�ļ�
	�ṩ�������Ͷ��壬C��׼ͷ�ļ��������ļ�������ʱ���趨��
	BUAA 2008-7-11
	���Գ�
*/
#ifndef __CONFIG_H 
#define __CONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

//��һ������Ķ�
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif

typedef unsigned char uint8;// �޷���8λ���ͱ���  
typedef signed char int8;// �з���8λ���ͱ���  
typedef unsigned short uint16;// �޷���16λ���ͱ��� 
typedef signed short int16;// �з���16λ���ͱ��� 
typedef unsigned int uint32;// �޷���32λ���ͱ��� 
typedef signed int int32;// �з���32λ���ͱ��� 
typedef float fp32;// �����ȸ�������32λ���ȣ� 
typedef double fp64;// ˫���ȸ�������64λ���ȣ� 

#define GLOBAL	 extern	

//��������ת������
#define U8toU16(x, y) ((uint16)(((uint16)(y))<<8|((uint16)(x))))
#define U8toS16(x, y) ((int16)(((uint16)(y))<<8|((uint16)(x))))

#define U8toU32(x1,x2,x3,x4) ((uint32)(((uint32)(x4))<<24|((uint32)(x3))<<16|((uint32)(x2))<<8|((uint16)(x1))))
#define U8toS32(x1,x2,x3,x4) ((int32)(((uint32)(x4))<<24|((uint32)(x3))<<16|((uint32)(x2))<<8|((uint16)(x1))))

#define U16toU8(x) (uint8*)(&(x))
#define S16toU8(x) (uint8*)(&(x))


//LPC2300ϵ��оƬ�Ĵ�������
#include    "LPC23xx.h"
//C���Ա�׼ͷ�ļ�
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <setjmp.h>
#include <rt_misc.h>
//M2300����ͷ�ļ�
#include "target.h"
#include	"IRQHander.h"

//ʱ������
#define	USE_USB				1

#define Fosc                12000000					//	OSC = 12MHz

#if USE_USB	
	
	#define Fusbclk	 48000000
	
	#define Fcclk	(Fosc * 4) 							// ��Ƶ Fcclk = 48MHz
	#define Fcco	(Fusbclk * (USBCLKDivValue+1))		// �������USB,��Fcco��Fcclk��Fusbclk��275~550M֮�����С������
														// Fcco��Fcclk��275~550M֮�����С����
	#define Fpclk	(Fcclk / 2)
	
	#define PLL_NValue			1
	#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
	#define CCLKDivValue		(Fcco/Fcclk-1)
	#define USBCLKDivValue		5	
#else
	#define Fcclk	(Fosc * 4)							// ��Ƶ Fcclk = 48MHz
	#define Fcco    (Fcclk* 6)
	
	#define Fpclk	(Fcclk / 4)
	
	#define PLL_NValue			1	
	#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
	#define CCLKDivValue		(Fcco/Fcclk-1)
	#define USBCLKDivValue		254		
#endif	


#define     BUZZER                  1 << 27             // P1.27���Ʒ��������͵�ƽ����

#define     BUZZER_Set()            PINSEL3&=0xFFCFFFFF,IO1DIR|=(1<<27),IO1CLR|=(1<<27)  // ��������
#define     BUZZER_Clr()            IO1SET|=(1<<27) 

#ifdef __cplusplus
	}
#endif
#endif

