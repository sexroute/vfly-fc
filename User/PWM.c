/*
	PWM.c
	ͨ��FPGAģ��ʵ��PWM����������ɼ�
	BUAA 2008-9-4
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "Main.h"
#include "User.h"
#include "PWM.h"

//���0.5~2.5ms
uint16 PWMCodeIn[9]={0};//0x28F-0x51E
uint16 PWMCodeOut[9]={0};//0x28F-0x51E
float PWMInBuffer[9]={0};//-1~1
float PWMCurrOut[9]={0};//-1~1

void GetPWM()
{
	uint8 i=0;
	float t=0;
	PWMCodeIn[0]=FPGAReadStep(PitchChannel);
	PWMCodeIn[1]=FPGAReadStep(RollChannel);
	PWMCodeIn[2]=FPGAReadStep(DirectChannel);
	PWMCodeIn[3]=FPGAReadStep(DistanceChannel);
	PWMCodeIn[4]=FPGAReadStep(ThrottleChannel);
	PWMCodeIn[5]=FPGAReadStep(SwitchChannel);
	PWMCodeIn[6]=FPGAReadStep(GyroChannel);
	PWMCodeIn[7]=FPGAReadStep(RPMChannel);
	PWMCodeIn[8]=FPGAReadStep(ReserveChannel);
	for(i=0;i<9;i++)
	{
		//���˵�<0.5ms��>2.5ms���ź�
		if(PWMCodeIn[i]<PWMZero||PWMCodeIn[i]>PWMZero*5)
		{
			continue;
		}
		//����ߵ�ƽʱ�䲢�ŵ���ʱ������
		t=(PWMCodeIn[i]-PWMZero*3);
		t=t/(PWMZero*2);
		//���˵�ͻ�����0.5���ź�
		if(PWMInBuffer[i]-t<0.5&&t-PWMInBuffer[i]<0.5)
		{
			PWMIn[i]=t;
		}
		PWMInBuffer[i]=t;
	}
}

void SetPWM()
{
	
	//δƽ��ֱ�����
	uint8 i=0;
	for(i=0;i<9;i++)
	{
		//TODO:�Ļ�
		PWMCodeOut[i]=PWMOut[i]*PWMZero*2+PWMZero*3;
		//PWMCodeOut[i]=(PWMOut[i]/2)*PWMZero+PWMZero*3;//��С������������Գ��
	}

	//���㷨����ͨ��ǿ��ֱ�����
	//PWMCodeOut[2]=PWMCodeIn[2];
	//PWMCodeOut[3]=PWMCodeIn[3];
	//PWMCodeOut[4]=PWMCodeIn[4];
	PWMCodeOut[5]=PWMCodeIn[5];
	PWMCodeOut[6]=PWMCodeIn[6];
	PWMCodeOut[7]=PWMCodeIn[7];
	PWMCodeOut[8]=PWMCodeIn[8];
	
	/*
	//����1/3ƽ���˲�
	uint8 i=0;
	for(i=0;i<9;i++)
	{
		//TODO:�Ļ�
		PWMCodeOut[i]=((PWMOut[i]*PWMZero*2+PWMZero*3)+PWMCodeOut[i]*2)/3;
		//PWMCodeOut[i]=(PWMOut[i]/2)*PWMZero+PWMZero*3;//��С������������Գ��
	}

	//���㷨����ͨ��ǿ��ֱ�����
	PWMCodeOut[2]=(PWMCodeIn[2]+PWMCodeOut[2]*2)/3;
	PWMCodeOut[3]=(PWMCodeIn[3]+PWMCodeOut[3]*2)/3;
	PWMCodeOut[4]=(PWMCodeIn[4]+PWMCodeOut[3]*2)/3;
	PWMCodeOut[5]=(PWMCodeIn[5]+PWMCodeOut[3]*2)/3;
	PWMCodeOut[6]=(PWMCodeIn[6]+PWMCodeOut[3]*2)/3;
	PWMCodeOut[7]=(PWMCodeIn[7]+PWMCodeOut[3]*2)/3;
	PWMCodeOut[8]=(PWMCodeIn[8]+PWMCodeOut[3]*2)/3;
	*/

	FPGASetStep(PitchChannel,PWMCodeOut[0]);
	FPGASetStep(RollChannel,PWMCodeOut[1]);
	FPGASetStep(DirectChannel,PWMCodeOut[2]);
	FPGASetStep(DistanceChannel,PWMCodeOut[3]);
	FPGASetStep(ThrottleChannel,PWMCodeOut[4]);
	FPGASetStep(SwitchChannel,PWMCodeOut[5]);
	FPGASetStep(GyroChannel,PWMCodeOut[6]);
	FPGASetStep(RPMChannel,PWMCodeOut[7]);
	FPGASetStep(ReserveChannel,PWMCodeOut[8]);
}

