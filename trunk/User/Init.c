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
	int i;
	/*FCAData[7][0]=0.1;
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
	*/
	//���ÿ�����ȡ��λ��
	CollectMedianDataState = 0;
	CollectMedianPosYaw = 0xFF;
	//���ÿ�������ʼ��������
	//PosXY
	FCAKData[0][0]=1; //kpx kdx kix
	FCAKData[0][1]=0;
	FCAKData[0][2]=0;
	
	FCAKData[1][0]=2; //xmax kxmax umax
	FCAKData[1][1]=1;
	FCAKData[1][2]=5;
	
	FCAKData[2][0]=1; //kpy kdy kiy
	FCAKData[2][1]=0;
	FCAKData[2][2]=0;
	
	FCAKData[3][0]=2; //ymax kymax vmax
	FCAKData[3][1]=1;
	FCAKData[3][2]=5;
		
	//VelXY
	FCAKData[4][0]=0.03; //kpu kpp kdp
	FCAKData[4][1]=0.003; 
	FCAKData[4][2]=0;
	
	FCAKData[5][0]=0; //kiu kou kop
	FCAKData[5][1]=0;
	FCAKData[5][2]=0;
	
	FCAKData[6][0]=0.03; //kpv kpr kdr
	FCAKData[6][1]=0.003;
	FCAKData[6][2]=0;
	
	FCAKData[7][0]=0; //kiv kov kor
	FCAKData[7][1]=0;
	FCAKData[7][2]=0;
	
	//Yaw ay
	FCAKData[8][0]=1; //kpay kday kiay
	FCAKData[8][1]=0;
	FCAKData[8][2]=0;
	
	FCAKData[9][0]=5; //aymax kaymax armax
	FCAKData[9][1]=1;
	FCAKData[9][2]=20;
	
	FCAKData[10][0]=0.005; //kpar kdar
	FCAKData[10][1]=0;
	FCAKData[10][2]=0;
	
	//Hei
	FCAKData[11][0]=1;
	FCAKData[11][1]=0;
	FCAKData[11][2]=0;
	
	FCAKData[12][0]=5;
	FCAKData[12][1]=1;
	FCAKData[12][2]=20;
	
	FCAKData[13][0]=0.05;
	FCAKData[13][1]=0;
	FCAKData[13][2]=0;
	
	//ʹ������
	FCATarData[2][0]=0; //λ�ñջ�ʹ�ܣ�
	FCATarData[2][1]=1; //���ʹ��
	FCATarData[2][2]=0;
	//��ر���
	FCATarData[3][0]=0;
	FCATarData[3][1]=0;
	FCATarData[3][2]=0;
	
	FCATarData[4][0]=0;
	FCATarData[4][1]=0;
	FCATarData[4][2]=0; //��Ϊת�ٿ�������־ʹ�ܺͽ��ܱ�־λ��	 
	
	//��������λ����FCAKData��
	for(i=0;i<14;i++)
	{
		DataSendFlag[0x80+i]=DataSendFlag[0x80+i]|0x01;
	}
	//������ؿ��غ�ң�ظ���״̬
	DataSendFlag[0x73]=DataSendFlag[0x73]|0x01;
	DataSendFlag[0x74]=DataSendFlag[0x74]|0x01;
}