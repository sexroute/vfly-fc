/*
	AD.c
	ģ������ת��оƬ��������(ADS8345)
	�ṩADоƬͨѶ�����ݶ�ȡ����
	BUAA 2008-7-16
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "AD.h"

//No:0-1
//Ch:0-7

//��ȡָ���ӿڵ�ģ���ѹ���ݴ���
//��ʹ��AEXn������ΪNum���������������ȡ��Ӧ�ӿڵ�����
int16 ADReadCode(uint8 No,uint8 Ch)
{
	uint8 word,tt[6],i;
	uint16 r;
	if(Ch>=8||No>1)
	{
		return 0;
	}
	word=(Ch<<4)|0x80|0x04|0x03;//��һ��:��ʼλ���ڶ���:������ת����������:��Դ��
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

//��ȡָ��ͨ���ĵ�ѹ��ֵ(mA)
//��ʹ��AEXn������ΪNum���������������ȡ��Ӧ�ӿڵ�����
//δ�����˷ŶԵ�ѹ�ķŴ󣬲�����Χ(����ֵ��Χ)Ϊ0~2*ADVref(2048mA)��
int16 ADReadVoltage(uint8 No,uint8 Ch)
{
	int16 r=ADReadCode(No,Ch);
	r=ADVref+r*ADVref/0x8000;//TODO:�˷�ϵ��
	return r;
}
	