/*
	SSP0.c
	SSP0��������������
	�ṩSSP0�����������ӵ������豸��ͨѶģʽЭ�����ܡ�
	BUAA 2008-7-26
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "SSP0.h"

//����SSP������SPIͨѶ��FPGAģʽ
uint8 SSP0FPGAMode()
{
	SSPInit(0,192,0|CPHA|CPOL);//TODO:Up Clock
   	SelectY0;
	return TRUE;
}

//����SSP������SPIͨѶ��ADģʽ
uint8 SSP0ADMode()
{
	SSPInit(0,192,0);
   	SelectY0;
	return TRUE;
}

//����SSP������SPIͨѶ��DAģʽ
uint8 SSP0DAMode()
{
	SSPInit(0,192,0|CPHA);
   	SelectY0;
	return TRUE;
}
