/*
	Main.c
	vFly��ĿC�����������ļ�
	BUAA 2008-7-14
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "Tools.h"
#include "Main.h"
#include "User.h"

#include "Test.h"

uint8 FirewareVer=100;

uint8 AutoCount=0,ManualCount=0,ErrorCount=0;//�����ۼ�ң�����������״̬�Ĵ������Ӷ�����ͻ�䡣
uint8 FPGAState=TRUE;//FPGA״̬

uint8 ChangeRunMode(uint8 Mode)
{
	if(Mode>=MODEManual&&Mode<=MODEFactory)
	{
		if(RunState!=Mode)
		{
			if(Mode>=MODETarget0&&Mode<=MODETargetX)
			{//�¼ӵĻ�ȡ��λ������
				CollectMedianPosYaw=0;
			}
			
			RunState=Mode;
			FCEventSend(RunState);
		}
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

uint8 CheckFPGAState()
{
	//������
	uint8 i=0,t=0;
	for(i=0;i<5;i++)
	{
		t=FPGACheck();
		if(t==TRUE)
		{
			break;
		}
	}
		
	//FPGAģ����ʱû��һ���Գɹ����ʹ���Ϣ
	if(i>0&&t==TRUE)
	{
		FCMessageSend("FPGAWarn");
	}
	//FPGAģ�������
	if(t==FALSE)
	{
		if(FPGAState==TRUE)
		{
			FCEventSend(ErrFPGA);
		}
	}
	else
	{
		if(FPGAState==FALSE)
		{
			FCEventSend(OKFPGA);
		}
	}
	return t;
}

extern uint8 OnSwitchAuto;

void SwitchMode()
{
			
	FPGAState=CheckFPGAState();
	if(FPGAState==FALSE)
	{
		SwitchMODEManual;
		ChangeRunMode(MODEManual);
	}
	else//FPGAģ����������½������Լ��л�
	{
		if(PWMIn[5]>0.5)//�Լ�
		{
			AutoCount++;
			ManualCount=0;
		}
		else if(PWMIn[5]<-0.5)//�ּ�
		{
			ManualCount++;
			AutoCount=0;
		}
		else
		{
			ManualCount=0;
			AutoCount=0;
		}
		if(AutoCount>5)
		{
			
			SwitchMODEAuto;
			if(RunState==MODEManual)
			{
				OnSwitchAuto=0;				
			}
						
			ChangeRunMode(PreAutoMode);
			ManualCount=0;
			AutoCount=0;
		}
		if(ManualCount>5)
		{
			
			
			SwitchMODEManual;
			ChangeRunMode(MODEManual);
			ManualCount=0;
			AutoCount=0;
		}
	}
}

void Test()
{
	PinInit();
	IO0DIR=1<<29|1<<30;//P0.29,P0.30����ͬΪ����������(SSP0��Ƭѡ).
	UARTInit(0,115200);
	
	//FCMessageSend("TestMode");	
	CANTest();
	
	while(1)
	{
		
		/*
		uint8 c;
		uint16 t=FPGARead(TESTFPGA);
		UARTSendChar(0,0xff);
		c=t;
		UARTSendChar(0,c);
		c=t>>8;
		UARTSendChar(0,c);
		SSPSendByte(0,0xff);
		for(t=0;t<1024;t++)
		{;}
		*/
	}
}

int main (void)
{
	//Test
	//Test();
	//Init
	InitDevice();
	
	
	//Send Startup Message
	FCMessageSend("vFlyOK");

	//�ȴ�INS��GPS�������
	//WaitGPS();

	//�ı�����״̬������״̬��Ϣ
	RunState=DefaultRunState;
	FCEventSend(RunState);
	//��ʼ������
	InitPara();
	FCMessageSend("InitParaOK");
		
	//��������ʱ��
	TimerEnable(0);
	
	while(1)
	{
		if(Do==DoWorkLoop)
		{
			LED2ON();
			INS();
			SwitchMode();
			if(FPGAState==TRUE)
			{
				GetPWM();
			}
			//FCA();
			//FCALimitPWM(void);
			FCAGetNav();
			FCAPWMOut();
			SmoothPWM(40);
			SetPWM();
			//CollectMedianDataFun(40);
			FCACollectPosYaw(5);
			FCACollectMedianDataFun(40);
			FCFrameProcess();
			if(MainLoopRunCounter%DataSendDF==0)
			{
				DataReturn(0);
			}
			if(MainLoopRunCounter%DataRecDF==DataRecDF/2)
			{
				DataReturn(1);
			}
			MainLoopRunCounter++;
			LED2OFF();
			Do=DoNothing;
		}
		if(Do==DoFactoryLoop)
		{
			LED1ON();
			GpioClrLOW(P1_25);//һ���л����ּ�
			INS();
			FCFrameProcess();
			if(DataToDownload==0)
			{
				if(MainLoopRunCounter%DataSendDF==0)
				{
					DataReturn(0);
				}
				/*
				if(MainLoopRunCounter%DataRecDF==DataRecDF/2)
				{
					DataReturn(1);
				}
				*/
			}
			else
			{
				DataDownload();
			}
			MainLoopRunCounter++;
			LED1OFF();
			Do=DoNothing;
		}
	}
	return 0;
}

