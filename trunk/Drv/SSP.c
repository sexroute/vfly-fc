/*
	SSP.c
	SSP驱动程序
	提供初始化，读写功能(同步查询模式)。
	BUAA 2008-7-14
	张以成
*/

#include "Config.h"
#include "Drv.h"
#include "SSP.h"

//SSP初始化
//SSP0链接FPGA，SSP1链接Flash。
//PortNum=0-1,Fdiv>=2,Mode=0|CPOL|CPHA
//注意对应宏的定义及片选控制的引脚设置。
void  SSPInit(uint8 PortNum,uint32 Fdiv,uint32 Mode)
{	
	if(PortNum==0)
	{
		//初始化P1.20、P1.21、P1.23、P1.24为SSP接口引脚，不用另外的IO口作从机选择线，直接用SSEL0完成片选595的功能
		PINSEL3 |= (0x03 << 8) | (0x00 << 10) | (0x03 << 14) | (0x03 << 16);//TODO=DEL		PINSEL3 |= (0x03 << 8) | (0x03 << 10) | (0x03 << 14) | (0x03 << 16);//TODO=DEL
		SSP0CR0=	(7<<0)|//设置数据长度为8Bit
					(0<<4)|//SPI模式
					//(1<<6)|//=1时SCK为低电平即CPOL
					//(1<<7)|//=0时为第一个时钟采样即CPHA
					(0<<8)|//默认位速率
					Mode;
		SSP0CPSR = Fdiv;// 设置SSP从PCLK获得的分频值,主模式下有效, 最小值为0x02
		SSP0CR1  = 	(0<<0)|//LMB回写模式
					(1<<1)|//SSP使能，在配置完寄存器后执行
					(0<<2);//默认即为主
	}
	else if(PortNum==1)
	{
		//初始化P0.6-9为SSP接口引脚，不用另外的IO口作从机选择线，直接用SSEL1完成片选595的功能
		PINSEL0 |= (0x02 << 12) | (0x02 << 14) | (0x02 << 16) | (0x02 << 18);//TODO=DEL
		SSP1CR0=	(7<<0)|//设置数据长度为8Bit
					(0<<4)|//SPI模式
					//(1<<6)|//=1时SCK为低电平即CPOL
					//(0<<7)|//=0时为第一个时钟采样即CPHA
					(0<<8)|//默认位速率
					Mode;
		SSP1CPSR = Fdiv;// 设置SSP从PCLK获得的分频值,主模式下有效, 最小值为0x02
		SSP1CR1  = 	(0<<0)|//LMB回写模式
					(1<<1)|//SSP使能，在配置完寄存器后执行
					(0<<2);//默认即为主

	}
}

//发送数据
uint8  SSPSendByte(uint8 PortNum,uint8 Data)
{
	if(PortNum==0)
	{
		while ((SSP0SR & 0x02) == 0);// 等待发送FIFO留出空间   
		SSP0DR = Data;
		while ((SSP0SR & 0x10) == 0x10);// 等待数据帧发送完毕
		Data=SSP0DR;
		return Data;
	}
	else
	{
		while ((SSP1SR & 0x02) == 0);// 等待发送FIFO留出空间   
		SSP1DR = Data;
		while ((SSP1SR & 0x10) == 0x10);// 等待数据帧发送完毕
		Data=0xFF;
		Data=SSP1DR;
		return Data;
	}
}

//发送数据串
void SSPSend(uint8 PortNum,uint8* Data,uint32 Len)
{
	int i;
	for(i=0;i<Len;i++)
	{
		Data[i]=SSPSendByte(PortNum,Data[i]);
	}
}
