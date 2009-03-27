/*
	SSP.c
	SSP��������
	�ṩ��ʼ������д����(ͬ����ѯģʽ)��
	BUAA 2008-7-14
	���Գ�
*/

#include "Config.h"
#include "Drv.h"
#include "SSP.h"

//SSP��ʼ��
//SSP0����FPGA��SSP1����Flash��
//PortNum=0-1,Fdiv>=2,Mode=0|CPOL|CPHA
//ע���Ӧ��Ķ��弰Ƭѡ���Ƶ��������á�
void  SSPInit(uint8 PortNum,uint32 Fdiv,uint32 Mode)
{	
	if(PortNum==0)
	{
		//��ʼ��P1.20��P1.21��P1.23��P1.24ΪSSP�ӿ����ţ����������IO�����ӻ�ѡ���ߣ�ֱ����SSEL0���Ƭѡ595�Ĺ���
		PINSEL3 |= (0x03 << 8) | (0x00 << 10) | (0x03 << 14) | (0x03 << 16);//TODO=DEL		PINSEL3 |= (0x03 << 8) | (0x03 << 10) | (0x03 << 14) | (0x03 << 16);//TODO=DEL
		SSP0CR0=	(7<<0)|//�������ݳ���Ϊ8Bit
					(0<<4)|//SPIģʽ
					//(1<<6)|//=1ʱSCKΪ�͵�ƽ��CPOL
					//(1<<7)|//=0ʱΪ��һ��ʱ�Ӳ�����CPHA
					(0<<8)|//Ĭ��λ����
					Mode;
		SSP0CPSR = Fdiv;// ����SSP��PCLK��õķ�Ƶֵ,��ģʽ����Ч, ��СֵΪ0x02
		SSP0CR1  = 	(0<<0)|//LMB��дģʽ
					(1<<1)|//SSPʹ�ܣ���������Ĵ�����ִ��
					(0<<2);//Ĭ�ϼ�Ϊ��
	}
	else if(PortNum==1)
	{
		//��ʼ��P0.6-9ΪSSP�ӿ����ţ����������IO�����ӻ�ѡ���ߣ�ֱ����SSEL1���Ƭѡ595�Ĺ���
		PINSEL0 |= (0x02 << 12) | (0x02 << 14) | (0x02 << 16) | (0x02 << 18);//TODO=DEL
		SSP1CR0=	(7<<0)|//�������ݳ���Ϊ8Bit
					(0<<4)|//SPIģʽ
					//(1<<6)|//=1ʱSCKΪ�͵�ƽ��CPOL
					//(0<<7)|//=0ʱΪ��һ��ʱ�Ӳ�����CPHA
					(0<<8)|//Ĭ��λ����
					Mode;
		SSP1CPSR = Fdiv;// ����SSP��PCLK��õķ�Ƶֵ,��ģʽ����Ч, ��СֵΪ0x02
		SSP1CR1  = 	(0<<0)|//LMB��дģʽ
					(1<<1)|//SSPʹ�ܣ���������Ĵ�����ִ��
					(0<<2);//Ĭ�ϼ�Ϊ��

	}
}

//��������
uint8  SSPSendByte(uint8 PortNum,uint8 Data)
{
	if(PortNum==0)
	{
		while ((SSP0SR & 0x02) == 0);// �ȴ�����FIFO�����ռ�   
		SSP0DR = Data;
		while ((SSP0SR & 0x10) == 0x10);// �ȴ�����֡�������
		Data=SSP0DR;
		return Data;
	}
	else
	{
		while ((SSP1SR & 0x02) == 0);// �ȴ�����FIFO�����ռ�   
		SSP1DR = Data;
		while ((SSP1SR & 0x10) == 0x10);// �ȴ�����֡�������
		Data=0xFF;
		Data=SSP1DR;
		return Data;
	}
}

//�������ݴ�
void SSPSend(uint8 PortNum,uint8* Data,uint32 Len)
{
	int i;
	for(i=0;i<Len;i++)
	{
		Data[i]=SSPSendByte(PortNum,Data[i]);
	}
}
