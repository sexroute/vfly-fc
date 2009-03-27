/*
	Tools.c
	工具函数集
	BUAA 2008-7-10
	张以成
*/
#include "config.h"
#include "drv.h"
#include "tools.h"

 //延迟一定时间
void Delay(void)
{
	int i,j;
	for(i=0;i<1000;i++)
	{
		for(j=0;j<2000;j++)
		{}
	}
}

//通过串口0发送指定字符串
void PrintUART0(const char *str)             
{
	char TmpStr[50]="";
	sprintf(TmpStr,str);
	UARTSend(0, (uint8*)TmpStr, strlen(str));
}

//LED1打开
void  LED1ON(void)
{
  IO0DIR|=0x10;
  IO0CLR|=0x10;
  return;
}

//LED1关闭
void  LED1OFF(void)
{
  IO0DIR|=0x10;
  IO0SET|=0x10;
  return;
}

//LED2打开
void  LED2ON(void)
{
  IO0DIR|=0x20;
  IO0CLR|=0x20;
  return;
}

//LED2关闭
void  LED2OFF(void)
{
  IO0DIR|=0x20;
  IO0SET|=0x20;
  return;
}

//Beep打开(SmartARM2300实验板配套)
void BeepON(void)
{
  IO1DIR|=0x8000000;
  IO1CLR|=0x8000000;
}

//Beep关闭(SmartARM2300实验板配套)
void BeepOFF(void)
{
  IO1DIR|=0x8000000;
  IO1SET|=0x8000000;
}

