/*
	Timer.c
	时钟控制器驱动程序
	提供初始化、中断触发、时间延迟、时钟复位、使能、关闭功能。
	BUAA 2008-7-13
	蔡云屾
*/
#include "Config.h"
#include "Drv.h"
#include "Timer.h"
#include "Tools.h"

#include "Main.h"
#include "FCFrame.h"

uint32 Timer0Counter = 0;
uint32 Timer1Counter = 0;
uint32 Timer2Counter = 0;
uint32 Timer3Counter = 0;

//延迟MS级的时间
void DelayMs(uint8 TimerNum, uint32 DelayInMs)
{
	if ( TimerNum == 0 )
	{
		//设置时钟0来做Delay
		T0TCR = 0x02;//复位时钟
		T0PR  = 0x00;	//分频数为0
		T0MR0 = DelayInMs * (Fpclk / 1000-1);
		T0IR  = 0xff;	//复位所有中断
		T0MCR = 0x04;//停止时钟
		T0TCR = 0x01;//打开时钟
		//等待时间结束
		while (T0TCR & 0x01);
  	}
	else if ( TimerNum == 1 )
	{
		//设置时钟1来做Delay
		T1TCR = 0x02;
		T1PR  = 0x00;	
		T1MR0 = DelayInMs * (Fpclk / 1000-1);
		T1IR  = 0xff;	
		T1MCR = 0x04;
		T1TCR = 0x01;
		while (T1TCR & 0x01);
  	}
	else if ( TimerNum == 2)
	{
		//设置时钟2来做Delay
  		PCONP|=(1<<22);
		T2TCR = 0x02;
		T2PR  = 0x00;
		T2MR0 = DelayInMs * (Fpclk / 1000-1);
		T2IR  = 0xff;	
		T2MCR = 0x04;
		T2TCR = 0x01;  
		while (T2TCR & 0x01);	
  	}
  	else if ( TimerNum == 3)
	{
		//设置时钟3来做Delay
  		PCONP|=(1<<23);
		T3TCR = 0x02;
		T3PR  = 0x00;	
		T3MR0 = DelayInMs * (Fpclk / 1000-1);
		T3IR  = 0xff;	
		T3MCR = 0x04;
		T3TCR = 0x01;
		while (T3TCR & 0x01);	
	}
}

extern uint8 Do;
extern uint8 RunState;
uint8 piece=0;

//timer0的中断
void __irq Timer0Handler ()  
{  
	T0IR = 1;//清除中断标志
	if(Do==DoWorkLoop||Do==DoFactoryLoop)
	{
		//FCErrorSend(0,Timer0Counter, Do,0,"TimeOver");//CHANGE:ErrorID
	}
	if(RunState>=MODEManual&&RunState<=MODETargetX)
	{	
		Do=DoWorkLoop;
	}
	else if(RunState==MODEFactory)
	{
		Do=DoFactoryLoop;
	}
	else
	{
		Do=DoNothing;
	}
	VICVectAddr = 0;
}

//timer1的中断
void __irq Timer1Handler ()  
{  
	T1IR = 1;
	Timer1Counter++;
	if(Timer1Counter % 2 == 1)
  		LED1ON();
	else if(Timer1Counter % 2 == 0)
  		LED1OFF();
	VICVectAddr = 0;
}

//timer2的中断
void __irq Timer2Handler ()  
{  
	T2IR = 1;
	Timer2Counter++;
	if(Timer2Counter % 2 == 1)
  		LED2ON();
	else if(Timer2Counter % 2 == 0)
  		LED2OFF();
	VICVectAddr = 0;
}

//timer3的中断
void __irq Timer3Handler ()  
{  
	T3IR = 1;
	Timer3Counter++;
	if(Timer3Counter % 2 == 1)
  		LED2ON();
	else if(Timer3Counter % 2 == 0)
		LED2OFF();
	VICVectAddr = 0;
}

//时钟的使能
void TimerEnable( uint8 TimerNum )
{
	if ( TimerNum == 0 )
	{
		T0TCR = 1;
	}
	else if(TimerNum == 1)
	{
		T1TCR = 1;
	}
	else if(TimerNum == 2)
	{
		T2TCR = 1;
	}
	else if(TimerNum == 3)
	{
		T3TCR = 1;
	}
}

//时钟的关闭
void TimerDisable( uint8 TimerNum )
{
	if ( TimerNum == 0 )
	{
		T0TCR = 0;
	}
	else if(TimerNum == 1)
	{
		T1TCR = 0;
	}
	else if(TimerNum == 2)
	{
		T2TCR = 0;
	}
	else if(TimerNum == 3)
	{
		T3TCR = 0;
	}
}

//时钟的复位
void TimerReset( uint8 TimerNum )
{
	uint32 regVal;

	if ( TimerNum == 0 )
	{
		regVal = T0TCR;
		regVal |= 0x02;
		T0TCR = regVal;
	}
	else if (TimerNum == 1)
	{
		regVal = T1TCR;
		regVal |= 0x02;
		T1TCR = regVal;
	}
	else if (TimerNum == 2)
	{
		regVal = T2TCR;
		regVal |= 0x02;
		T2TCR = regVal;
	}
	else if (TimerNum == 3)
	{
		regVal = T3TCR;
		regVal |= 0x02;
		T3TCR = regVal;
	}
}

//时钟的初始化
//TimerInterval单位为ms
uint32 TimerInit ( uint8 TimerNum, uint32 TimerInterval ) 
{
	if ( TimerNum == 0 )
	{
		uint32 p=T0PR;
		Timer0Counter = 0;
		T0MR0 = TimerInterval * (Fpclk / ((p+1)*1000)); 
		T0MCR = 3;//重置匹配中断
		if (InstallIRQ(TIMER0_INT,(void*)Timer0Handler,HIGHEST_PRIORITY) == FALSE)
		{
	  		return (FALSE);
		}  
		else
		{
			return (TRUE);
		}
	}
	else if ( TimerNum == 1 )
	{
		Timer1Counter = 0;
		T1MR0 = TimerInterval * (Fpclk / 1000-1);
		T1MCR = 3;
		if ( InstallIRQ( TIMER1_INT,  (void*)Timer1Handler,HIGHEST_PRIORITY) == FALSE )
		{
			return (FALSE);
		}  
		else
		{
	  		return (TRUE);
		}
	}
	else if ( TimerNum == 2 )
	{
		PCONP|=(1<<22);
		Timer2Counter = 0;
		T2MR0 = TimerInterval * (Fpclk / 1000-1);
		T2MCR = 3;
		if ( InstallIRQ( TIMER2_INT,  (void*)Timer2Handler,HIGHEST_PRIORITY ) == FALSE )
		{
	  		return (FALSE);
		}  
		else
		{
			return (TRUE);
		}
	}
	else if ( TimerNum == 3 )
	{
		PCONP|=(1<<23);
		Timer3Counter = 0;
		T3MR0 = TimerInterval * (Fpclk / 1000-1);
		T3MCR = 3;
		if ( InstallIRQ( TIMER3_INT,  (void*)Timer3Handler,HIGHEST_PRIORITY ) == FALSE )
		{
			return (FALSE);
		}  
		else
		{
	  		return (TRUE);
		}
	}
	return (FALSE);
}

