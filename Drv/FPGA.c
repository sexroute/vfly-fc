/*
	FPGA.c
	FPGAģ�����������ṩ��ʼ����ģ���飬���������趨���������ڶ�ȡ��Ƶ�ʶ�ȡ���ܡ�
	���ݺ��弰��λ��ο�FPGA����ĵ���PWM.c
	BUAA 2008-7-15
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "FPGA.h"

#define __SSP

//FPGA�ڲ�ͨѶ����(��ȡ)
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

//FPGA�ڲ�ͨѶ����(����)
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

//���FPGA����״̬
uint8 FPGACheck()
{
	uint16 r=FPGARead(TESTFPGA);//���ܷ���ֵ����ȷ
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

//��ȡ����
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

//��ȡƵ��
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

//��ȡ�ߵ�ƽʱ��
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

//��������
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

//���øߵ�ƽʱ��
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

