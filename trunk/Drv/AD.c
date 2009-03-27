/*
	AD.c
	模拟数字转换芯片驱动程序(ADS8345)
	提供AD芯片通讯及数据读取功能
	BUAA 2008-7-16
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "AD.h"

//No:0-1
//Ch:0-7

//读取指定接口的模拟电压数据代码
//需使用AEXn宏来作为Num参数的替代量来读取对应接口的数据
int16 ADReadCode(uint8 No,uint8 Ch)
{
	uint8 word,tt[6],i;
	uint16 r;
	if(Ch>=8||No>1)
	{
		return 0;
	}
	word=(Ch<<4)|0x80|0x04|0x03;//第一组:起始位；第二组:单周期转换；第三组:电源打开
	//GpioClrLOW(P1_21);
	if(No==0)
	{
		AD0_DOWN;
	}
	else
	{
		AD1_DOWN;
	}
	tt[0]=SSPSendByte(0,word);
	
	for(i=0;i<5;i++);
	//tt[1]=SSPSendByte(0,0);
	//tt[2]=SSPSendByte(0,0);
	//tt[3]=SSPSendByte(0,0);
	
	tt[4]=SSPSendByte(0,0);
	tt[5]=SSPSendByte(0,0);
	AD_UP;
	//GpioSetLOW(P1_21);
	//UARTSendChar(0,0xAA);
	//UARTSendChar(0,tt[4]);
	//UARTSendChar(0,tt[5]);
	//UARTSend(0,tt+4,2);
	r=((tt[4]<<9)|(tt[5]<<1));
	return r;
}

//读取指定通道的电压数值(mA)
//需使用AEXn宏来作为Num参数的替代量来读取对应接口的数据
//未考虑运放对电压的放大，测量范围(返回值范围)为0~2*ADVref(2048mA)。
int16 ADReadVoltage(uint8 No,uint8 Ch)
{
	int16 r=ADReadCode(No,Ch);
	r=ADVref+r*ADVref/0x8000;//TODO:运放系数
	return r;
}
	