/*
	Gpio.c
	通用输入输出接口操纵驱动程序
	此驱动所有参数均指LPC2300PinCfg.h中定义的引脚宏
	BUAA 2008-7-14
	蔡云屾
*/
#include "Config.h"
#include "Drv.h"
#include "Gpio.h"

//低速GPIO引脚为置"1"
int32 GpioSetLOW(uint32 arg)
{ 
       uint8 flag = 0;
	uint8 temp;
	temp = arg % 100;
	
	if(arg/100 == 0)
	{
		IO0DIR |= (1<<temp);
		IO0SET |= (1<<temp);
		flag =1;
	}
	else if (arg/100 == 1)
	{
		IO1DIR |= (1<<temp);
		IO1SET |= (1<<temp);
		flag = 1;
	}
	if (flag == 1)
		return TRUE;
	return FALSE;

}

//高速GPIO引脚为置"1"
int32 GpioSetHI(uint32 arg)
{ 
	uint8 temp1,temp2,flag = 0;
	temp1 = arg / 100;
	temp2 = arg % 100;
	
       switch(temp1)
       	{
			case 0 :{
						FIO0DIR |= (1<<temp2);
						FIO0SET |= (1<<temp2);
						flag =1;
						break;
					}
			case 1 :{
						FIO1DIR |= (1<<temp2);
						FIO1SET |= (1<<temp2);
						flag =1;
						break;
					}
			case 2 :{
						FIO2DIR |= (1<<temp2);
 						FIO2SET |= (1<<temp2);
						flag =1;
						break;
					}
			case 3 :{
						FIO3DIR |= (1<<temp2);
						FIO3SET |= (1<<temp2);
						flag =1;
						break;
					}
			case 4 :{
						FIO4DIR |= (1<<temp2);
						FIO4SET |= (1<<temp2);
						flag =1;
						break;
					}
			default  :  break;
	   	}
	 if (flag == 1)
		return TRUE;
	return FALSE;
}

//低速GPIO引脚置为"0"
int32 GpioClrLOW(uint32 arg)
{
	uint8 temp,flag = 0;
	temp = arg%100;
	
	if(arg/100 == 0)
	{
		IO0DIR |= (1<<temp);
		IO0CLR |= (1<<temp);
		flag = 1;
	}
	else if(arg/100 == 1)
	{
		IO1DIR |= (1<<temp);
		IO1CLR |= (1<<temp);
		flag = 1;
	}
	 if (flag == 1)
		return TRUE;
	return FALSE;
}

//高速GPIO引脚置为"0"
int32 GpioClrHI(uint32 arg)
{
	uint8 temp1,temp2,flag = 0;
	temp1 = arg / 100;
	temp2 = arg % 100;

       switch(temp1)
       	{
			case 0 :{
						FIO0DIR |= (1<<temp2);
						FIO0CLR |= (1<<temp2);
						flag =1;
						break;
					}
			case 1 :{
						FIO1DIR |= (1<<temp2);
						FIO1CLR |= (1<<temp2);
						flag =1;
						break;
					}
			case 2 :{
						FIO2DIR |= (1<<temp2);
 						FIO2CLR |= (1<<temp2);
						flag =1;
						break;
					}
			case 3 :{
						FIO3DIR |= (1<<temp2);
						FIO3CLR |= (1<<temp2);
						flag =1;
						break;
					}
			case 4 :{
						FIO4DIR |= (1<<temp2);
						FIO4CLR |= (1<<temp2);
						flag =1;
						break;
					}
			default  :  break;
	   	}
	 if (flag == 1)
		return TRUE;
	return FALSE;
}

//读取低速GPIO端口电平
int32 GpioGetLOW(uint32 arg)
{
	uint32 i = 0,j = 0;
	uint8 temp,flag = 0;
	int8 r = 4;

	temp = arg%100;
	if(arg/100 == 0)
	{
		flag = 1;
		j=IO0PIN;
		i=j & (1<<temp);
		if(i != 0)
			r = 1;
		else
			r = 0;
	}
	else if(arg/100 == 1)
	{
		j=IO1PIN;
		i=j & (1<<temp);
		flag = 1;
		if(i!= 0)
			r = 1;
		else
			r = 0;
	}
	if(flag != 1)
		r = -1;
	return (r);
}

int32 GpioGetHI(uint32 arg)
{
	uint32 i = 0,j = 0;
	uint8 temp1,temp2,flag = 0;
	int32 r=4;

	temp1 = arg /100;
	temp2 = arg %100;
	switch(temp1)
       	{
			case 0 :{
						j=FIO0PIN;
						i =j & (1<<temp2);
						flag =1;
						if(i!=0)
							r=1;
						else 
							r=0;
						break;
					}
			case 1 :{
						j=FIO1PIN;
						i=j & (1<<temp2);
						flag =1;
						if(i !=0)
							r=1;
						else 
							r=0;
							break;
					}
			case 2 :{
						j=FIO2PIN;
						i=j & (1<<temp2);
						flag =1;
						if(i  !=0)
							r = 1;
						else 
						 	r = 0;
						break;
					}
			case 3 :{
				  		j =FIO3PIN;
						i=j & (1<<temp2);
						flag =1;
						if(i!=0)
							r = 1;
						else 
							r = 0;
						break;
					}
			case 4 :{
						j=FIO4PIN;
						i=j & (1<<temp2);
						flag = 1;
						if(i !=0)
							r = 1;
						else 
							r = 0;
						break;
					}
			default  :  break;
	   	}
	if(flag!=1)
		r = -1;
	return(r);
}

//取反低速GPIO端口电平
int32 GpioCplLOW(uint32 arg)
{
	uint32 i = 0,j = 0;
	uint8 temp,flag = 0;
	int8 r = 4;

	temp = arg%100;
	if(arg/100 == 0)
	{
		j=IO0PIN;
		i=j & (1<<temp);
		flag = 1;
		if(i !=0)
		{
			GpioClrLOW(arg);
			r = 1;
		}
		else
		{
			GpioSetLOW(arg);
			r = 0;
		}

	}
	else if(arg/100 == 1)
	{
		j=IO1PIN;
		i=j & (1<<temp);
		flag = 1;
		if(i !=0)
		{
			GpioClrLOW(arg);
			r = 1;
		}
		else
		{
			GpioSetLOW(arg);
			r = 0;
		}
	}
	if(flag != 1)
		r = -1;
	return(r);
}

//取反高速GPIO端口电平
int32 GpioCplHI(uint32 arg)
{
	uint32 i = 0,k = 0;
	uint8 temp1,temp2,flag = 0;
	int8 r = 4;
	
	temp1 = arg /100;
	temp2 = arg %100;
	switch(temp1)
       	{
			case 0 :{
						k = FIO0PIN;
						i=k & (1<<temp2);
						flag =1;
						if(i !=0)
						{
							GpioClrHI(arg);
							r = 1;
						}
						else
						{
							GpioSetHI(arg);
							r=0;
						}
						break;
					}
			case 1 :{
						k = FIO1PIN;
						i=k & (1<<temp2);
						flag =1;
						if(i !=0)
						{
							GpioClrHI(arg);
							r=1;
						}
						else
						{
							GpioSetHI(arg);
							r=0;
						}
						break;
					}
			case 2 :{
						k = FIO2PIN;
						i=k & (1<<temp2);
						flag =1;
						if(i !=0)
						{
							GpioClrHI(arg);
							r=1;
						}
						else
						{
							GpioSetHI(arg);
							r=0;
						}
						break;
					}
			case 3 :{
						k = FIO3PIN;
						i=k & (1<<temp2);
						flag =1;
						if(i !=0)
						{
							GpioClrHI(arg);
							r=1;
						}
						else
						{
							GpioSetHI(arg);
							r=0;
						}
						break;
					}
			case 4 :{
						k = FIO4PIN;
						i=k & (1<<temp2);
						flag =1;
						if(i !=0)
						{
							GpioClrHI(arg);
							r=1;
						}
						else
						{
							GpioSetHI(arg);
							r=0;
						}
						break;
					}
			default  :  break;
	   	}
	if(flag!=1)
		r=-1;
	return(r);
}

