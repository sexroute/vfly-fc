/*
	Init.c
	�ɿ�ģ���ʼ������
	BUAA 2008-9-4
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "Tools.h"
#include "Main.h"
#include "User.h"
#include "Init.h"

uint8 InitDevice()//TODO:�쳣����
{
	uint8 r=0,t=0;
	//GPIO
	PinInit();
	IO0DIR=1<<29|1<<30;//P0.29,P0.30����ͬΪ����������(SSP0��Ƭѡ).
	//GpioSpeedHigh();//ȫ�ָ���ģʽ,��ΪP0.29�����޷�ʹ�á�

	//ָʾϵͳ����
	LED1ON();

	//UART0-GCS
	r+=UARTInit(FCUARTPORT,FCUARTBPS);//UART0-PC
	UARTSendChar(FCUARTPORT,0x40);
	UARTSendChar(FCUARTPORT,0x40);

	//UART2-INS
	r+=UARTInit(INSUARTPORT,INSUARTBPS);
	//UARTSendChar(2,0x40);
	//UARTSendChar(2,0x40);

	//SSP0-FPGA
	r+=SSP0FPGAMode();
	//r+=SPIInit();
	//while(1)
	//{
	t=FPGACheck();
	//}
	if(t==TRUE)
	{
		FCEventSend(OKFPGA);
	}
	else
	{
		FCEventSend(ErrFPGA);
	}
	r+=t;

	//SSP1-FLASH
	r+=FlashInit(FlashQueueSize);

	//Timer0-MainLoop
	r+=TimerInit(0,1000/MainLoopHz);
	TimerDisable(0);
	//TimerEnable(0);

	//Timer1-Time+LED1
	r+=TimerInit(1,1000);
	TimerEnable(1);
	
	//Timer2-Working-LED2
	r+=TimerInit(2,100);
	TimerDisable(2);

	//�����ж�
	IRQEnable();
	
	return r;
}

void WaitGPS()
{
	while(TRUE)
	{
		uint8 t=0;
		LED2OFF();
		INS();
		t=INSFrameObj.Status&0x01;
		if(t==0x01)
		{
			break;
		}
		DelayMs(3,100);
		LED2ON();
		DelayMs(3,100);
	}
	//LED2OFF();
}

void InitPara()
{
	FCAData[7][0]=0.1;
	FCAData[7][1]=0.001;
	FCAData[7][2]=-0.02;
	FCAData[8][0]=-0.005;
	FCAData[8][1]=0.002;
	FCAData[8][2]=0.2;
	FCAData[9][0]=0.01;
	FCAData[9][1]=-0.004;
	FCAData[9][2]=0;
	FCAData[10][0]=0;
	FCAData[10][1]=0.03;
	FCAData[10][2]=0.02;
}