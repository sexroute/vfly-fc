/*
	SPI.c
	SPI驱动程序
	提供初始化，读写功能(同步查询模式)。
	BUAA 2008-7-14
	张以成
*/

#include "Config.h"
#include "Drv.h"
#include "SPI.h"

//SPI控制器初始化函数
uint32 SPIInit()
{
	PINSEL0	|= 	0xc0000000;	// 设置P0.15脚为SCK脚
   	PINSEL1	|=	(0x03 << 2) | (0x03 << 4);	// 设置P0.17、P0.18引脚为SPI引脚		
	//初始化SPI控制寄存器//TODO=CHANGE
   	S0SPCCR  =	192;		// 设置SPI时钟分频	
 	S0SPCR   = 	(0 << 2) |	// SPI控制器每次传输发送和接收8位数据。
 				(1 << 3) |	// CPHA = 0, 数据在SCK 的第一个时钟沿采样
 				(1 << 4) |	// CPOL = 0, SCK 为高有效
 				(1 << 5) |	// MSTR = 1, SPI 处于主模式
 				(1 << 6) |	// LSBF = 0, SPI 数据传输MSB (位7)在先
 				(0 << 7);		// SPIE = 0, SPI 中断被禁止
 	return TRUE;
}

//发送数据
uint8 SPISendByte(uint8 data)
{
	uint32 temp = 0;
	S0SPDR = data;
	while (0 == (S0SPSR & 0x80));	// 等待SPIF置位，即等待数据发送完毕
	temp=S0SPSR;				// 通过读S0SPSR，清除SPIF标志
	temp=S0SPDR;
	return temp;
}

/*
uint8 SPIGetByte()
{
	uint8 temp;
	while ( 0 == (S0SPSR & 0x80));// 等待SPIF置位，即等待数据发送完毕
	temp=S0SPSR;
	temp=S0SPDR;
	return temp;// 返回接收到的数据
}
*/