/*
	DA.c
	数字模拟信号转换芯片(TLV5610)驱动程序
	提供模拟电压输出功能(包含电压值的映射)。
	BUAA 2008-7-18
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "DA.h"

//以指定的电压代码输出电压
uint8 DASetCode(uint8 Ch,uint16 Code)
{
	uint8 word1,word2;
	if(Ch>=8)
	{
		return FALSE;
	}
	if(Code>0xFFF)
	{
		return FALSE;
	}
	word1=Ch<<4|Code>>8;
	word2=(uint8)Code;
	DAFSDOWN;
	word1=SSPSendByte(0, word1);
	word2=SSPSendByte(0, word2);
	DALDDOWN;
	//for(i=0;i<10;i++);
	DAUP;
	//UARTSendChar(0, word1);
	//UARTSendChar(0, word2);
	return TRUE;
}

//以指定的电压值输出电压
//已考虑运放作用，直接输入最终电压(Voltage)即可。
uint8 DASetVoltage(uint8 Ch,fp32 Voltage)
{
	fp32 div=3.37;
	uint16 code=(uint16)(Voltage/div/DAVref*0xFFF);//TODO:运放系数
	uint8 r=DASetCode(Ch,code);
	return r;
}

/*
//使能SPI接口数据同步返回
void DAOutEnable()
{
	uint8 word1,word2,i;
	word1=0x80;;
	word2=0x08;
	DAFSDOWN;
	word1=SSPSendByte(0, word1);
	word2=SSPSendByte(0, word2);
	DALDDOWN;
	for(i=0;i<10;i++);
	DAUP;
}
*/

