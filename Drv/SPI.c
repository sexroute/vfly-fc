/*
	SPI.c
	SPI��������
	�ṩ��ʼ������д����(ͬ����ѯģʽ)��
	BUAA 2008-7-14
	���Գ�
*/

#include "Config.h"
#include "Drv.h"
#include "SPI.h"

//SPI��������ʼ������
uint32 SPIInit()
{
	PINSEL0	|= 	0xc0000000;	// ����P0.15��ΪSCK��
   	PINSEL1	|=	(0x03 << 2) | (0x03 << 4);	// ����P0.17��P0.18����ΪSPI����		
	//��ʼ��SPI���ƼĴ���//TODO=CHANGE
   	S0SPCCR  =	192;		// ����SPIʱ�ӷ�Ƶ	
 	S0SPCR   = 	(0 << 2) |	// SPI������ÿ�δ��䷢�ͺͽ���8λ���ݡ�
 				(1 << 3) |	// CPHA = 0, ������SCK �ĵ�һ��ʱ���ز���
 				(1 << 4) |	// CPOL = 0, SCK Ϊ����Ч
 				(1 << 5) |	// MSTR = 1, SPI ������ģʽ
 				(1 << 6) |	// LSBF = 0, SPI ���ݴ���MSB (λ7)����
 				(0 << 7);		// SPIE = 0, SPI �жϱ���ֹ
 	return TRUE;
}

//��������
uint8 SPISendByte(uint8 data)
{
	uint32 temp = 0;
	S0SPDR = data;
	while (0 == (S0SPSR & 0x80));	// �ȴ�SPIF��λ�����ȴ����ݷ������
	temp=S0SPSR;				// ͨ����S0SPSR�����SPIF��־
	temp=S0SPDR;
	return temp;
}

/*
uint8 SPIGetByte()
{
	uint8 temp;
	while ( 0 == (S0SPSR & 0x80));// �ȴ�SPIF��λ�����ȴ����ݷ������
	temp=S0SPSR;
	temp=S0SPDR;
	return temp;// ���ؽ��յ�������
}
*/