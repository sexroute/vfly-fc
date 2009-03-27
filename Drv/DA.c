/*
	DA.c
	����ģ���ź�ת��оƬ(TLV5610)��������
	�ṩģ���ѹ�������(������ѹֵ��ӳ��)��
	BUAA 2008-7-18
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "DA.h"

//��ָ���ĵ�ѹ���������ѹ
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

//��ָ���ĵ�ѹֵ�����ѹ
//�ѿ����˷����ã�ֱ���������յ�ѹ(Voltage)���ɡ�
uint8 DASetVoltage(uint8 Ch,fp32 Voltage)
{
	fp32 div=3.37;
	uint16 code=(uint16)(Voltage/div/DAVref*0xFFF);//TODO:�˷�ϵ��
	uint8 r=DASetCode(Ch,code);
	return r;
}

/*
//ʹ��SPI�ӿ�����ͬ������
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

