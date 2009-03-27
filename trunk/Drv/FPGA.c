/*
	FPGA.c
	FPGA模块驱动程序，提供初始化、模块检查，脉宽周期设定，脉宽周期读取，频率读取功能。
	数据含义及单位请参考FPGA设计文档或PWM.c
	BUAA 2008-7-15
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "FPGA.h"

#define __SSP

//FPGA内部通讯函数(读取)
uint16 FPGARead(uint8 Word)
{
	uint8 c[2];
	uint16 r=0;
	FPGA_DOWN;
	
	#ifdef __SSP
		SSPSendByte(0,Word);
		c[0]=SSPSendByte(0,0);
		c[1]=SSPSendByte(0,0);
	#endif
	
	#ifndef __SSP
		SPISendByte(Word);
		c[0]=SPISendByte(0);
		c[1]=SPISendByte(0);
	#endif
	
	FPGA_UP;
	r=(c[0]+(c[1]<<8));
	return r;
}

//FPGA内部通讯函数(设置)
uint16 FPGASet(uint8 Word,uint16 Data)
{
	uint8 c[2];
	uint16 r=0;
	c[0]=Data;
	c[1]=Data>>8;
	FPGA_DOWN;

	#ifdef __SSP
		SSPSendByte(0,Word);
		c[0]=SSPSendByte(0,c[0]);
		c[1]=SSPSendByte(0,c[1]);
	#endif

	#ifndef __SSP
		SPISendByte(Word);
		c[0]=SPISendByte(c[0]);
		c[1]=SPISendByte(c[1]);
	#endif
	
	FPGA_UP;
	r=(c[0]+(c[1]<<8));
	return r;
}

//检测FPGA工作状态
uint8 FPGACheck()
{
	uint16 r=FPGARead(TESTFPGA);//可能返回值不正确
	if(r==0xDDEE)
	{
		r=TRUE;
	}
	else
	{
		r=FALSE;
	}
	return r;
}

//读取周期
uint16 FPGAReadTime(uint8 Num)
{
	uint16 r;
	if(Num>=12)
	{
		r=0;
	}
	else
	{
		r=FPGARead(READTIME+Num);
	}
	return r;
}

//读取频率
uint16 FPGAReadFre(uint8 Num)
{
	uint16 r;
	if(Num>=4)
	{
		r=0;
	}
	else
	{
		r=FPGARead(READFRE+Num);
	}
	return r;
}

//读取高电平时间
uint16 FPGAReadStep(uint8 Num)
{
	uint16 r;
	if(Num>=12)
	{
		r=0;
	}
	else
	{
		r=FPGARead(READSTEP+Num);
	}
	return r;
}

//设置周期
uint16 FPGASetTime(uint8 Num,uint16 Data)
{
	uint16 r;
	if(Num>=12)
	{
		r=0;
	}
	else
	{
		r=FPGASet(SETTIME+Num,Data);
	}
	return r;
}

//设置高电平时间
uint16 FPGASetStep(uint8 Num,uint16 Data)
{
	uint16 r;
	if(Num>=12)
	{
		r=0;
	}
	else
	{
		r=FPGASet(SETSTEP+Num,Data);
	}
	return r;
}

