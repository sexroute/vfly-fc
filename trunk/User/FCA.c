/*
	FCA.c
	FCA���PID�ɿ��㷨ʵ��
	BUAA 2008-9-8
	���Գ�
*/

#include "Config.h"
#include "Drv.h"
#include "Main.h"
#include "User.h"
#include "FCA.h"

#include <Math.h>

//����״̬
extern uint8 RunState;

//������
extern float PWMIn[9];
extern float PWMOut[9];

//INSͳ������
extern uint32 INSFrameError;
//INS֡����
extern INSFrameStruct INSFrameObj;

//FCA����֡
volatile fp32 FCAData[19][3]={0};//CHANGE:��FCAЭ�����ͬ��
/*
00:��̬��λ*3 
01:�ٶ���λ*3 
02:���ٶ���λ*3 
03:��������λ*3 
04:�㷨������*3 
05:ң�ؿ�����*3 
06:���������*3 
07:����ϵ��K1-3 
08:����ϵ��K4-6 
09:����ϵ��K7-9 
10:����ϵ��K10-12 
11:��ر���*3 
12:Ŀ��λ��X+Ŀ��λ��Y 

13:�ٶ�*3 (����ϵ)*
14:���ٶ�*3 int16 AngVeloBodyX;//0.01��/s int16 AngVeloBodyY;//0.01��/s int16 AngVeloBodyZ;//0.01��/s
15:�Ƕ�*3 int16 Pitch;//0.1��int16 Roll;//0.1��int16 Yaw;//0.1��
16:λ��*3 int32 Lo;// 1e-7 degree int32 La;// 1e-7 degree int32 Height;// millmeter
17:�Ƕ�����*3 *
18:���ٶ�����*3 *
*/

float u_tmp,v_tmp,w_tmp;

float pp,rr,yy;

float d_u, d_v, d_w;//�ٶ�����
//float d_roll, d_pitch, d_yaw;//�Ƕ�����
//float d_p, d_q, d_r;//���ٶ�����

float yaw0_last;
float d_yaw_last;
float d_yaw_tmp;

float i_u = 0;
float i_v = 0;
float dt = 0.025;//CHANGE

float LTLK=1.0;

void LimitValue()
{
	//��λ
	// 0
	if(FCAData[4][0]>1)
	{
		FCAData[4][0]=1;
	}
	if(FCAData[4][0]<-1)
	{
		FCAData[4][0]=-1;
	}

	// 1
	if(FCAData[4][1]>1)
	{
		FCAData[4][1]=1;
	}
	if(FCAData[4][1]<-1)
	{
		FCAData[4][1]=-1;
	}

	// 2
	if(FCAData[4][2]>1)
	{
		FCAData[4][2]=1;
	}
	if(FCAData[4][2]<-1)
	{
		FCAData[4][2]=-1;
	}
}

void FCA()
{
	//����ϵ�Ƕ�
	FCAData[15][0]=INSFrameObj.Pitch/10.0;
	FCAData[15][1]=INSFrameObj.Roll/10.0;
	FCAData[15][2]=INSFrameObj.Yaw/10.0;
	//����ϵ������
	FCAData[14][0]=INSFrameObj.AngVeloBodyX/100.0;
	FCAData[14][1]=INSFrameObj.AngVeloBodyY/100.0;
	FCAData[14][2]=INSFrameObj.AngVeloBodyZ/100.0;
	//����ϵ�ٶ�
	u_tmp=INSFrameObj.SpeedN/100.0;
	v_tmp=INSFrameObj.SpeedE/100.0;
	w_tmp=INSFrameObj.SpeedD/100.0;
	//����ϵλ��
	LTLK=cos((double)INSFrameObj.La*3.14/180.0/600000.0);
	FCAData[16][0]=INSFrameObj.La*MtMk;
	FCAData[16][1]=INSFrameObj.Lo*LTLK*MtMk;
	FCAData[16][2]=INSFrameObj.Height;
	//����ϵ�ٶ�
	pp=FCAData[15][0] * 3.1415926 / 180;
	rr=FCAData[15][1] * 3.1415926 / 180;
	yy=FCAData[15][2] * 3.1415926 / 180;


	FCAData[13][0]=cos(pp) *cos(yy) * u_tmp + cos(pp) * sin(yy) * v_tmp - sin(pp) * w_tmp;
	FCAData[13][1]=(-cos(rr) * sin(yy) + sin(rr) * sin(pp) * cos(yy)) * u_tmp + (cos(rr) * cos(yy) + sin(rr) *sin(pp) * sin(yy)) * v_tmp + sin(rr) * cos(pp) * w_tmp;
	FCAData[13][2]=(sin(rr) * sin(yy) + cos(rr) * sin(pp) * cos(yy)) * u_tmp + (-sin(rr) * cos(yy) + cos(rr) * sin(pp) * sin(yy)) * v_tmp +cos(rr) * cos(pp) * w_tmp;	

	//������ٶ�����
	FCAData[18][0]= FCAData[14][0] - FCAData[2][0];
	FCAData[18][1]= FCAData[14][1] - FCAData[2][1];
	FCAData[18][2]= FCAData[14][2] - FCAData[2][2];

	//�����ٶ�����
	d_u = FCAData[13][0] - FCAData[1][0];
	d_v = FCAData[13][1] - FCAData[1][1];
	d_w = FCAData[13][2]- FCAData[1][2];

	//����Ƕ�����
	FCAData[17][0] = FCAData[15][0] - FCAData[0][0];
	FCAData[17][1] = FCAData[15][1] - FCAData[0][1];
	FCAData[17][2] = FCAData[15][2] - FCAData[0][2];

	//�����������0�ȵ���
	d_yaw_tmp = FCAData[18][2] - d_yaw_last;
	if (d_yaw_tmp > 300)
       	FCAData[18][2] = FCAData[18][2] - 360;
	else if (d_yaw_tmp < -300)
		FCAData[18][2] = FCAData[18][2] + 360;
	d_yaw_last = FCAData[18][2];

	//ˮƽ���ٶȻ���
	i_u +=FCAData[13][0]* dt;
	i_v +=FCAData[13][1]* dt;

	//��ȡ�ֿ���
	FCAData[5][0]=PWMIn[0];
	FCAData[5][1]=PWMIn[1];
	FCAData[5][2]=PWMIn[2];

	if(RunState==MODETargetP)
	{
		//pPID=λ��ң��PID
		//ң����������FCAData[17]��
		FCAData[4][0] = FCAData[7][0] * d_u + FCAData[7][1] * d_v + FCAData[7][2] * (FCAData[17][0]+ 30*FCAData[5][0])+ FCAData[8][0] * FCAData[18][1] + FCAData[10][1] * i_u+FCAData[3][0];//?d_q
		FCAData[4][1] = FCAData[8][1] * d_u + FCAData[8][2] * d_v + FCAData[9][0] * (FCAData[17][1] + 50*FCAData[5][1])+ FCAData[9][1] * FCAData[18][0] + FCAData[10][2] * i_v+FCAData[3][1];//?d_p
		FCAData[4][2] = FCAData[9][2] * FCAData[17][2] + FCAData[10][0] * FCAData[18][2]+FCAData[3][2];
	}
	else
	{
		//mPID=��ͨȫ�Կض�·PID
		FCAData[4][0] = FCAData[7][0] * d_u + FCAData[7][1] * d_v + FCAData[7][2] * FCAData[17][0] + FCAData[8][0] * FCAData[18][1] + FCAData[10][1] * i_u+FCAData[3][0];//?d_q
		FCAData[4][1] = FCAData[8][1] * d_u + FCAData[8][2] * d_v + FCAData[9][0] * FCAData[17][1] + FCAData[9][1] * FCAData[18][0] + FCAData[10][2] * i_v+FCAData[3][1];//?d_p
		FCAData[4][2] = FCAData[9][2] * FCAData[17][2] + FCAData[10][0] * FCAData[18][2]+FCAData[3][2];
	}

	//�Կ�ģʽ(���)=�㷨����ֱ�����
	if(RunState>=MODETarget0&&RunState<=MODETargetX)
	{
		FCAData[6][0]=FCAData[4][0];
		FCAData[6][1]=FCAData[4][1];
		FCAData[6][2]=FCAData[4][2];
	}
	//���ģʽ=������&��λ
	else if(RunState==MODEMix)
	{
		FCAData[6][0]=FCAData[11][0]*FCAData[5][0]+(1-FCAData[11][0])*FCAData[4][0];
		FCAData[6][1]=FCAData[11][1]*FCAData[5][1]+(1-FCAData[11][1])*FCAData[4][1];
		FCAData[6][2]=FCAData[11][2]*FCAData[5][2]+(1-FCAData[11][2])*FCAData[4][2];
		
		//��λ
		// 0
		if(FCAData[6][0]>1)
		{
			FCAData[6][0]=1;
		}
		if(FCAData[6][0]<-1)
		{
			FCAData[6][0]=-1;
		}

		// 1
		if(FCAData[6][1]>1)
		{
			FCAData[6][1]=1;
		}
		if(FCAData[6][1]<-1)
		{
			FCAData[6][1]=-1;
		}

		// 2
		if(FCAData[6][2]>1)
		{
			FCAData[6][2]=1;
		}
		if(FCAData[6][2]<-1)
		{
			FCAData[6][2]=-1;
		}
	}
	//�ֿ�ģʽ=�ֿ�����ֱ�����
	else if(RunState==MODEManual)
	{
		FCAData[6][0]=FCAData[5][0];
		FCAData[6][1]=FCAData[5][1];
		FCAData[6][2]=FCAData[5][2];
	}

	//�������
	PWMOut[0]=FCAData[6][0];
	PWMOut[1]=FCAData[6][1];
	PWMOut[2]=FCAData[6][2];	
}


uint8 OnSwitchAuto=0xFF;

void SmoothPWM(uint8 Long)
{
	if(OnSwitchAuto<Long)
	{
		PWMOut[0]=(PWMIn[0]*(Long-OnSwitchAuto)+PWMOut[0]*OnSwitchAuto)/Long;
		PWMOut[1]=(PWMIn[1]*(Long-OnSwitchAuto)+PWMOut[1]*OnSwitchAuto)/Long;
		PWMOut[2]=(PWMIn[2]*(Long-OnSwitchAuto)+PWMOut[2]*OnSwitchAuto)/Long;
		OnSwitchAuto++;
	}
}

uint8 CollectMedianDataState=0xFF;
fp32 MedianData[4][3];
extern uint8 DataSendFlag[256];

void CollectMedianDataFun(uint8 Long)
{
	uint8 i,j;
	if(CollectMedianDataState==0)
	{
		memset(MedianData,0,sizeof(fp32)*3*4);
	}
	if(CollectMedianDataState<Long)
	{

		for(i=0;i<3;i++)
		{
			MedianData[0][i]+=FCAData[15][i];
			MedianData[1][i]+=FCAData[13][i];
			MedianData[2][i]+=FCAData[14][i];
			MedianData[3][i]+=FCAData[5][i];
		}
		CollectMedianDataState++;
	}
	else if(CollectMedianDataState==Long)
	{
		for(j=0;j<4;j++)
		{
			for(i=0;i<3;i++)
			{
				FCAData[j][i]=MedianData[j][i]/(Long*1.0);
			}
		}
		FCEventSend(CollectMedianData);
		DataSendFlag[0xC0]=DataSendFlag[0xC0]|0x01;
		DataSendFlag[0xC1]=DataSendFlag[0xC1]|0x01;
		DataSendFlag[0xC2]=DataSendFlag[0xC2]|0x01;
		DataSendFlag[0xC3]=DataSendFlag[0xC3]|0x01;
		CollectMedianDataState=0xFF;
	}
}
