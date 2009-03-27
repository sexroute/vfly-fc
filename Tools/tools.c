/*
	Tools.c
	���ߺ�����
	BUAA 2008-7-10
	���Գ�
*/
#include "config.h"
#include "drv.h"
#include "tools.h"

 //�ӳ�һ��ʱ��
void Delay(void)
{
	int i,j;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<2000;j++)
		{}
	}
}

//ͨ������0����ָ���ַ���
void PrintUART0(const char *str)             
{
	char TmpStr[50]="";
	sprintf(TmpStr,str);
	UARTSend(0, (uint8*)TmpStr, strlen(str));
}

//LED1��
void  LED1ON(void)
{
  IO0DIR|=0x10;
  IO0CLR|=0x10;
  return;
}

//LED1�ر�
void  LED1OFF(void)
{
  IO0DIR|=0x10;
  IO0SET|=0x10;
  return;
}

//LED2��
void  LED2ON(void)
{
  IO0DIR|=0x20;
  IO0CLR|=0x20;
  return;
}

//LED2�ر�
void  LED2OFF(void)
{
  IO0DIR|=0x20;
  IO0SET|=0x20;
  return;
}

//Beep��(SmartARM2300ʵ�������)
void BeepON(void)
{
  IO1DIR|=0x8000000;
  IO1CLR|=0x8000000;
}

//Beep�ر�(SmartARM2300ʵ�������)
void BeepOFF(void)
{
  IO1DIR|=0x8000000;
  IO1SET|=0x8000000;
}

