/*
	Main.c
	vFly项目C语言主函数文件
	BUAA 2008-7-14
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "Tools.h"
#include "Main.h"
#include "User.h"

#include "Test.h"

uint8 FirewareVer=100;

uint8 AutoCount=0,ManualCount=0,ErrorCount=0;//用于累计遥控器处于这个状态的次数，从而避免突变。
uint8 FPGAState=TRUE;//FPGA状态

uint8 ChangeRunMode(uint8 Mode)
{
	if(Mode>=MODEManual&&Mode<=MODEFactory)
	{
		if(RunState!=Mode)
		{
			if(Mode>=MODETarget0&&Mode<=MODETargetX)
			{//新加的获取中位函数；
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
	//错误检测
	uint8 i=0,t=0;
	for(i=0;i<5;i++)
	{
		t=FPGACheck();
		if(t==TRUE)
		{
			break;
		}
	}
		
	//FPGA模块检测时没有一次性成功则发送此消息
	if(i>0&&t==TRUE)
	{
		FCMessageSend("FPGAWarn");
	}
	//FPGA模块错误处理
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
	else//FPGA模块正常情况下进行手自驾切换
	{
		if(PWMIn[5]>0.5)//自驾
		{
			AutoCount++;
			ManualCount=0;
		}
		else if(PWMIn[5]<-0.5)//手驾
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
	IO0DIR=1<<29|1<<30;//P0.29,P0.30必须同为输出才能输出(SSP0的片选).
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

	//等待INS的GPS完成启动
	//WaitGPS();

	//改变运行状态并发送状态信息
	RunState=DefaultRunState;
	FCEventSend(RunState);
	//初始化参数
	InitPara();
	FCMessageSend("InitParaOK");
		
	//启动主计时器
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
			GpioClrLOW(P1_25);//一律切换到手驾
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

