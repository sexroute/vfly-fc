/*
	Timer.c
	ʱ�ӿ�������������
	�ṩ��ʼ�����жϴ�����ʱ���ӳ١�ʱ�Ӹ�λ��ʹ�ܡ��رչ��ܡ�
	BUAA 2008-7-13
	���ƌ�
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

//�ӳ�MS����ʱ��
void DelayMs(uint8 TimerNum, uint32 DelayInMs)
{
	if ( TimerNum == 0 )
	{
		//����ʱ��0����Delay
		T0TCR = 0x02;//��λʱ��
		T0PR  = 0x00;	//��Ƶ��Ϊ0
		T0MR0 = DelayInMs * (Fpclk / 1000-1);
		T0IR  = 0xff;	//��λ�����ж�
		T0MCR = 0x04;//ֹͣʱ��
		T0TCR = 0x01;//��ʱ��
		//�ȴ�ʱ�����
		while (T0TCR & 0x01);
  	}
	else if ( TimerNum == 1 )
	{
		//����ʱ��1����Delay
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
		//����ʱ��2����Delay
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
		//����ʱ��3����Delay
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

//timer0���ж�
void __irq Timer0Handler ()  
{  
	T0IR = 1;//����жϱ�־
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

//timer1���ж�
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

//timer2���ж�
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

//timer3���ж�
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

//ʱ�ӵ�ʹ��
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

//ʱ�ӵĹر�
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

//ʱ�ӵĸ�λ
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

//ʱ�ӵĳ�ʼ��
//TimerInterval��λΪms
uint32 TimerInit ( uint8 TimerNum, uint32 TimerInterval ) 
{
	if ( TimerNum == 0 )
	{
		uint32 p=T0PR;
		Timer0Counter = 0;
		T0MR0 = TimerInterval * (Fpclk / ((p+1)*1000)); 
		T0MCR = 3;//����ƥ���ж�
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

