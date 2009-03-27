/*
	FCA2.c
	FCA�ɿ��㷨ʵ��
	BUAA 2009-2-26
	������
*/

#include "Config.h"
#include "Drv.h"
#include "Main.h"
#include "User.h"
#include "FCA2.h"

#include <Math.h>

float LTLK=1.0;
//��������������������:�л�ʱ��ȡ��λ����ȡ��λʱ���ã�
float i_x = 0,i_y=0,i_h=0,i_yaw = 0;

//���㷨����Ŀ�����������-1~1֮�䣻
void FCALimitPWM()
{
	//��λ	// 0
	int i,j;
	for (i=2;i<4;i++)
	{
		for(j=0;j<3;j++)
		{
			if (FCAOutData[i][j] >1)
				FCAOutData[i][j] = 1;
			else if(FCAOutData[i][j] <-1)
				FCAOutData[i][j] = -1;
		}
	}
}

//��ȡ������Ϣ��ң������������Ϣ
void FCAGetNav()
{
	float yy;
		
	//����ϵλ�� m
	LTLK=cos((double)INSFrameObj.La*3.14/180.0/600000.0);
	FCAOutData[7][0]=INSFrameObj.La*MtMk;
	FCAOutData[7][1]=INSFrameObj.Lo*LTLK*MtMk;
	FCAOutData[7][2]=INSFrameObj.Height/1000;
	
	//20090318 ʹ����λλ����Ϊλ����㣻
	FCAOutData[7][0]-=FCAMedianData[0][0];
	FCAOutData[7][1]-=FCAMedianData[0][1];
	FCAOutData[7][2]-=FCAMedianData[0][2];
	
	//����ϵ�ٶ� m/s
	FCAOutData[8][0]=INSFrameObj.SpeedN/100.0;
	FCAOutData[8][1]=INSFrameObj.SpeedE/100.0;
	FCAOutData[8][2]=INSFrameObj.SpeedD/100.0;
	//����ϵ�Ƕ�  1��
	FCAOutData[9][0]=-INSFrameObj.Pitch/10.0;		//�������Ƿ���
	FCAOutData[9][1]=INSFrameObj.Roll/10.0;
	FCAOutData[9][2]=INSFrameObj.Yaw/10.0;
	
	//����ϵ������ 1��/s
	FCAOutData[10][0]=-INSFrameObj.AngVeloBodyY/100.0; //pitch (�����ǶȺ�x���ٶ��෴)
	FCAOutData[10][1]=INSFrameObj.AngVeloBodyX/100.0; //roll
	FCAOutData[10][2]=INSFrameObj.AngVeloBodyZ/100.0; //yaw
	
	//pp=(-FCAOutData[9][0]) * 3.1415926 / 180;
	//rr=FCAOutData[9][1] * 3.1415926 / 180;
	yy=FCAOutData[9][2] * 3.1415926 / 180;
	
	//����ϵλ�ã���������ǣ�
	FCAOutData[11][0]= cos(yy) * FCAOutData[7][0] + sin(yy) * FCAOutData[7][1];
	FCAOutData[11][1]=(-sin(yy)) * FCAOutData[7][0] + cos(yy) * FCAOutData[7][1];
	FCAOutData[11][2]= FCAOutData[7][2];		//z�����ϣ�

	//����ϵ�ٶ�1 1m/s
	FCAOutData[12][0]=INSFrameObj.SpeedBodyX/100.0;
	FCAOutData[12][1]=INSFrameObj.SpeedBodyY/100.0;
	FCAOutData[12][2]=INSFrameObj.SpeedBodyZ/100.0;//z�����ϣ�
					
	//����ϵ�ٶ�2���� 0.01m/s
	
	//FCAOutData[12][0]=cos(pp) *cos(yy) * u_tmp + cos(pp) * sin(yy) * v_tmp - sin(pp) * w_tmp;
	//FCAOutData[12][1]=(-cos(rr) * sin(yy) + sin(rr) * sin(pp) * cos(yy)) * u_tmp + (cos(rr) * cos(yy) + sin(rr) *sin(pp) * sin(yy)) * v_tmp + sin(rr) * cos(pp) * w_tmp;
	//FCAOutData[12][2]=(sin(rr) * sin(yy) + cos(rr) * sin(pp) * cos(yy)) * u_tmp + (-sin(rr) * cos(yy) + cos(rr) * sin(pp) * sin(yy)) * v_tmp +cos(rr) * cos(pp) * w_tmp;	
		
	//��ȡ��������Ϣ -1~1
	//�ܷ����������л���
	FCAOutData[4][0]=PWMIn[0];//����
	FCAOutData[4][1]=PWMIn[1];//��װ
	FCAOutData[4][2]=PWMIn[2];//ƫ��
	
	FCAOutData[5][0]=PWMIn[3];//�ܾ�
	FCAOutData[5][1]=PWMIn[4];//����
	//�л��������Ǹжȡ�ת�ٿ�����
	FCAOutData[6][0]=PWMIn[6];
	FCAOutData[6][1]=PWMIn[7];
	FCAOutData[6][2]=PWMIn[8];
}


//��ȡ��ͣ����Ϣ
fp32 FCAMedianDataTmp[6][3];


/*void FCACollectPosYaw(uint8 Long)
{
	uint8 i;
	if(CollectMedianDataState==0)
	{
		memset(FCAMedianDataTmp,0,sizeof(fp32)*3*6);
	}
	if(CollectMedianDataState<Long)
	{

		for(i=0;i<3;i++)
		{
			FCAMedianDataTmp[0][i]+=FCAOutData[7][i];//����ϵλ�ã�
			//FCAMedianDataTmp[1][i]+=FCAOutData[12][i];//����ϵ�ٶ� ��ʵ����һֱΪ�㣻
			FCAMedianDataTmp[2][i]+=FCAOutData[9][i];//��̬��
			//FCAMedianDataTmp[3][i]+=FCAOutData[10][i];//���ٶȣ�ʵ����Ӧ��Ϊ�㣻
			//FCAMedianDataTmp[4][i]+=FCAOutData[4][i];//����������������ת��ƫ����
			//FCAMedianDataTmp[5][i]+=FCAOutData[5][i];//���������ܾ࣬���ţ�
		}
		CollectMedianDataState++;
	}
	else if(CollectMedianDataState==Long)
	{
		//λ��
		FCAMedianData[0][0] = FCAMedianDataTmp[0][0];
		FCAMedianData[0][1] = FCAMedianDataTmp[0][1];
		FCAMedianData[0][2] = FCAMedianDataTmp[0][2];
		//�����
		FCAMedianData[2][2] = FCAMedianDataTmp[2][2];

		FCEventSend(CollectMedianData);
		DataSendFlag[0x60]=DataSendFlag[0x60]|0x01;
		//DataSendFlag[0x61]=DataSendFlag[0x61]|0x01;
		DataSendFlag[0x62]=DataSendFlag[0x62]|0x01;
		//DataSendFlag[0x63]=DataSendFlag[0x63]|0x01;
		//DataSendFlag[0x64]=DataSendFlag[0x64]|0x01;
		//DataSendFlag[0x65]=DataSendFlag[0x65]|0x01;
		CollectMedianDataState=0xFF;
	}
	i_x = 0;i_y=0;i_h=0;i_yaw = 0;
}
*/

void FCACollectMedianDataFun(uint8 Long)
{
	uint8 i,j;
	if(CollectMedianDataState==0)
	{
		memset(FCAMedianDataTmp,0,sizeof(fp32)*3*6);
	}
	if(CollectMedianDataState<Long)
	{

		for(i=0;i<3;i++)
		{
			FCAMedianDataTmp[0][i]+=FCAOutData[7][i];//����ϵλ�ã�
			FCAMedianDataTmp[1][i]+=FCAOutData[12][i];//����ϵ�ٶ� ��ʵ����һֱΪ�㣻
			FCAMedianDataTmp[2][i]+=FCAOutData[9][i];//��̬��
			FCAMedianDataTmp[3][i]+=FCAOutData[10][i];//���ٶȣ�ʵ����Ӧ��Ϊ�㣻
			FCAMedianDataTmp[4][i]+=FCAOutData[4][i];//����������������ת��ƫ����
			FCAMedianDataTmp[5][i]+=FCAOutData[5][i];//���������ܾ࣬���ţ�
		}
		CollectMedianDataState++;
	}
	else if(CollectMedianDataState==Long)
	{
		for(j=0;j<6;j++)
		{
			for(i=0;i<3;i++)
			{
				FCAMedianData[j][i]=FCAMedianDataTmp[j][i]/(Long*1.0);
			}
		}
		FCEventSend(CollectMedianData);
		DataSendFlag[0x60]=DataSendFlag[0x60]|0x01;
		DataSendFlag[0x61]=DataSendFlag[0x61]|0x01;
		DataSendFlag[0x62]=DataSendFlag[0x62]|0x01;
		DataSendFlag[0x63]=DataSendFlag[0x63]|0x01;
		DataSendFlag[0x64]=DataSendFlag[0x64]|0x01;
		DataSendFlag[0x65]=DataSendFlag[0x65]|0x01;
		CollectMedianDataState=0xFF;
	}
	i_x = 0;i_y=0;i_h=0;i_yaw = 0;
}

//���п����㷨1��ˮƽλ��ͨ��
void FCAPosXY()
{
	float yy;
	float x_tmp, y_tmp;
	
	float x_median, y_median;
	
	float x_now, y_now;
	float x_tar, y_tar;
	float d_x,d_y;

	float x_rem, y_rem;			//ң����
	float x_cmd, y_cmd;			//��λ����
	float dt =0.025;

	// 1. ��ͣ��λ�е�λ��Ϊλ����㣻��NEת��Ϊx��y��
	//��ͣ��λ��Ϊ����ϵ��������Ҫ׼�������ݺ�����ʱת����
	yy=FCAOutData[9][2] * 3.1415926 / 180;
	
	//����ϵλ�ã���������ǣ�
	x_median= cos(yy) * FCAMedianData[0][0] + sin(yy) * FCAMedianData[0][1];
	y_median=(-sin(yy)) * FCAMedianData[0][0] + cos(yy) * FCAMedianData[0][1];
	
	// 2. ��ȡ��ǰλ�ã�����ϵ��Ծ���
	x_now = FCAOutData[11][0]-x_median;
	y_now = FCAOutData[11][1]-y_median;
		
	// 3. ������λ��ָ���ȡĿ����ͣλ�ã�����ϵNE����������Ϊ�ı�ʱ����
	x_tmp = FCATarData[0][0]-FCAMedianData[0][0];
	y_tmp = FCATarData[0][1]-FCAMedianData[0][1];
	
	x_cmd = cos(yy) * x_tmp + sin(yy) * y_tmp;
	y_cmd = (-sin(yy)) * x_tmp+cos(yy) * y_tmp;
	
	// 4. ң�����ӿ�:��λ��ң����ģʽ�¸������Ǳ���ϵλ��������
	if(RunState==MODETargetX)
	{
		x_rem = FCAOutData[4][0]*20;	//+-20m;��������Χ�ˣ�
		y_rem = FCAOutData[4][1]*20;
	}
	else
	{
		x_rem = 0;
		y_rem = 0;
	}
	x_tar = x_cmd + x_rem;
	y_tar = y_cmd + y_rem;
	//����
	d_x = x_tar - x_now;
	d_y = y_tar - y_now;
	
	//������Χ��ѯ:����
	if( d_x < FCAKData[1][0] && d_x > (-FCAKData[1][0]) )
		d_x = d_x * FCAKData[1][1];
	
	if( d_y < FCAKData[3][0] && d_y > (-FCAKData[3][0]) )
		d_y = d_y * FCAKData[3][1];
	
	//���֣������ͻ���,�ж��ϴ������
	//x����
	if(FCAOutData[0][0]>=FCAKData[1][2])
	{
		if(d_x <0)
			i_x += d_x*dt;
	}
	else if(FCAOutData[0][0]<=-FCAKData[1][2])
	{
		if(d_x >0)
			i_x += d_x*dt;
	}
	else
		i_x+=d_x*dt;
	//y����
	if(FCAOutData[0][1]>=FCAKData[3][2])
	{
		if(d_y <0)
			i_y += d_y*dt;
	}
	else if(FCAOutData[0][1]<=-FCAKData[3][2])
	{
		if(d_x >0)
			i_y += d_y*dt;
	}
	else
		i_y+=d_y*dt;
	
	//PIʹ��
	FCAOutData[0][0] = FCAKData[0][0]*d_x + FCAKData[0][2]*i_x;
	FCAOutData[0][1] = FCAKData[2][0]*d_y + FCAKData[2][2]*i_y;
	
	//���ͽ�ֹ
	if ( FCAOutData[0][0] > FCAKData[1][2] )
		FCAOutData[0][0] = FCAKData[1][2];
	else if( FCAOutData[0][0] < -FCAKData[1][2] )
		FCAOutData[0][0] = -FCAKData[1][2];
		
	if ( FCAOutData[0][1] > FCAKData[3][2] )
		FCAOutData[0][1] = FCAKData[3][2];
	else if( FCAOutData[0][1] < -FCAKData[3][2] )
		FCAOutData[0][1] = -FCAKData[3][2];		
		
	//���»�����
	FCAOutData[13][0]=x_tar;
	FCAOutData[13][1]=y_tar;
}

//���п����㷨2��ˮƽ�ٶ�ͨ��
void FCAVelXY()
{
	//�ٶ���
	float u_now,v_now;	//��ǰֵ
	float u_tar,v_tar;	//Ŀ��ֵ
	float d_u,d_v;
	
	float u_pos,v_pos;	//�ٶȱջ�ֵ
	float u_rem,v_rem;	//ң��ֵ
	float u_cmd,v_cmd;	//��λ��
	
	
	//��̬��
	float pit_now,rol_now;	//��ǰֵ
	float pit_tar,rol_tar;	//Ŀ��ֵ
	float d_pit,d_rol;
	float p,q;
	
	float pit_rem,rol_rem;	//ң��ֵ
	//float pit_cmd,rol_cmd;	//��λ��
		
	//float i_u = 0;
	//float i_v = 0;
	//float dt = 0.025;//CHANGE
	
	//*************************����������Ϣ:ȥ����ͣ��***************************************
	
	// 1. ��ǰ�ٶȺ���̬�������ٶ�����
	//�����ٶ�
	//u_now = FCAOutData[12][0] - FCAMedianData[1][0];
	//v_now = FCAOutData[12][1] - FCAMedianData[1][1];
	u_now = FCAOutData[12][0];
	v_now = FCAOutData[12][1];
	
	//������̬
	pit_now = FCAOutData[9][0] - FCAMedianData[2][0];
	rol_now = FCAOutData[9][1] - FCAMedianData[2][1];
	//������ٶ�
	//p = FCAOutData[14][0] - FCAMedianData[3][0];
	//q = FCAOutData[14][1] - FCAMedianData[3][1];
	p = FCAOutData[10][0];
	q = FCAOutData[10][1];
	//ˮƽ���ٶȻ���
	//i_u +=FCAData[13][0]* dt;
	//i_v +=FCAData[13][1]* dt;
	
	// 2.��λ���ӿ�û��:�������Ǳ���ϵ�ٶȣ�
	u_cmd = FCATarData[1][0];
	v_cmd = FCATarData[1][1];
	
	//pit_cmd = FCATarData[2][0];
	//rol_cmd = FCATarData[2][1];
	// 3. ң�����ӿ�
	
	// 1�ٶ�ң��
	if(RunState==MODETargetV)
	{
		u_rem = FCAOutData[4][0]*50;	//-5~5 ��Ӧ -0.5m/s~0.5m/s
		v_rem = FCAOutData[4][1]*50; //
	}
	else
	{
		u_rem = 0;
		v_rem = 0;
	}	
	// 1��̬ң��
	if(RunState==MODETargetP)
	{
		pit_rem = FCAOutData[4][0]*50;  //-1~1 ��Ӧ -50��~50��
		rol_rem = FCAOutData[4][1]*100;	//-1~1 ��Ӧ -100��~100��
	}
	else
	{
		pit_rem = 0;
		rol_rem = 0;
	}
		
	// 4.λ�ñջ�
	u_pos = FCAOutData[0][0];
	v_pos = FCAOutData[0][1];
	
	//****************************�Ƚϲ�ֵ**********************************
	u_tar = u_pos + u_rem + u_cmd;
	d_u = u_tar - u_now;
	
	v_tar = v_pos + v_rem + v_cmd;
	d_v = v_tar - v_now;
	
	pit_tar = pit_rem;
	d_pit = pit_tar-pit_now;
	
	rol_tar = rol_rem;
	d_rol = rol_tar-rol_now;
	
	//����
	FCAOutData[2][0]=FCAKData[4][0]*d_u + FCAKData[4][1]*d_pit - FCAKData[4][2]*p + FCAKData[5][1]*d_v + FCAKData[5][2]*d_rol;
	//��ת
	FCAOutData[2][1]=FCAKData[6][0]*d_v + FCAKData[6][1]*d_rol - FCAKData[6][2]*q + FCAKData[7][1]*d_u + FCAKData[7][2]*d_pit;
	
	//���·�����
	FCAOutData[14][0]=u_tar;
	FCAOutData[14][1]=v_tar;
	FCAOutData[15][0]=pit_tar;
	FCAOutData[15][1]=rol_tar;
}

//���п����㷨3����ֱ����ͨ��
void FCAYaw()
{
	float yaw_now;
	float yaw_tar;
	float d_yaw;
	
	float yaw_rem;
	float yaw_cmd;
	
	float r_now;
	float r_tar;
	float d_r=0,d_r_last=0;
	
	float dd_r = 0;
	
	//********************************�Ƕȿ���**********************************
	//��λ��
	yaw_cmd = FCATarData[2][2];			
	yaw_now = FCAOutData[9][2];//-FCAMedianData[2][2]; //(0~359)-(0~359) ��-180~+180֮��
	
	//λ��ң��,��̬ң�أ�����ң�أ�
	if(RunState >= MODETargetP&&RunState <= MODETargetX)
	{
		yaw_rem = FCAOutData[4][2]*30;		//��ǰ�Ƕȵķ�Χ�������+-15�ȣ�
	}
	else
		yaw_rem = 0;
	
		//�������
	yaw_tar = yaw_rem+yaw_cmd;
	d_yaw = yaw_tar - yaw_now;
	
	while (d_yaw > 180)
     d_yaw -= 360;
	while (d_yaw <-180)
  	 d_yaw += 360;
	
	//������Χ��ѯ:����
	if( d_yaw < FCAKData[9][0] && d_yaw > (-FCAKData[9][0]) )
		d_yaw = d_yaw * FCAKData[9][1];
		
	//���֣������ͻ���,�ж��ϴ������
	if(FCAOutData[1][0]>=FCAKData[9][2])
	{
		if(d_yaw <0)
			i_yaw += d_yaw;
	}
	else if(FCAOutData[1][0]<=-FCAKData[9][2])
	{
		if(d_yaw >0)
			i_yaw += d_yaw;
	}
	else
		i_yaw+=d_yaw;
	//PIʹ��
	FCAOutData[1][0] = FCAKData[8][0]*d_yaw + FCAKData[8][2]*i_yaw;
		
	//���ͽ�ֹ
	if ( FCAOutData[1][0] > FCAKData[9][2] )
		FCAOutData[1][0] = FCAKData[9][2];
	else if( FCAOutData[0][0] < -FCAKData[9][2] )
		FCAOutData[1][0] = -FCAKData[9][2];	
	
	//*******************���ʿ��� r =0��**************************************
	r_now = FCAOutData[10][2];//- FCAMedianData[3][2];
	
	r_tar = FCAOutData[1][0];
	d_r = r_tar - r_now;
	
	dd_r = d_r-d_r_last;
	d_r_last = d_r;
	
	//PD �������
	FCAOutData[2][2] = FCAKData[10][0] * d_r - FCAKData[10][1]*dd_r;	
	
	//����������
	FCAOutData[15][2] = yaw_tar;
}

//���п����㷨4����ֱ�߶�ͨ��
void FCAHei()
{
	float h_now;
	float h_tar;
	float d_h;
	
	float h_rem;
	float h_cmd;
	
	//float h_yaw;

	float w_now,w_tar,d_w;
	float w_rem;
	float d_w_last = 0,dd_w=0;

	h_cmd = FCATarData[0][2];
	h_now = FCAOutData[7][2];//����ϵ
	
	if(RunState>=MODETargetP&&RunState<=MODETargetX)
	{
		h_rem = FCAOutData[5][0]*20;		//+-20m
	}
	else
		h_rem = 0;
	
	//�������
	h_tar = h_rem+h_cmd;
	
	d_h = h_tar - h_now;
	
	//������Χ��ѯ:����
	if( d_h < FCAKData[12][0] && d_h > (-FCAKData[12][0]) )
		d_h = d_h * FCAKData[12][1];
		
	//���֣������ͻ���,�ж��ϴ������
	if(FCAOutData[0][2]>=FCAKData[12][2])
	{
		if(d_h <0)
			i_h += d_h;
	}
	else if(FCAOutData[0][2]<=-FCAKData[12][2])
	{
		if(d_h >0)
			i_h += d_h;
	}
	else
		i_h+=d_h;
		
	//PIʹ��
	FCAOutData[0][2] = FCAKData[11][0]*d_h + FCAKData[11][2]*i_h;
		
	//���ͽ�ֹ
	if ( FCAOutData[0][2] > FCAKData[12][2] )
		FCAOutData[0][2] = FCAKData[12][2];
	else if( FCAOutData[0][0] < -FCAKData[12][2] )
		FCAOutData[0][2] = -FCAKData[12][2];	
		
		
	//�������ʿ���
	//*******************���ʿ��� w =0��**************************************
	w_now = FCAOutData[12][2];
	
	//�ٶ�ң��ģʽ����̬ң��ģʽ
	//��̬ң�� && �ٶ�ң��
	//if(RunMode == MODETargetP || RunMode == MODETargetV)
	//	w_rem = = FCAOutData[5][0]*50;
	//else
	w_rem = 0;
	
	w_tar = w_rem + FCAOutData[0][2];
	d_w = w_tar - w_now;
	
	dd_w = d_w-d_w_last;
	d_w_last = d_w;
	
	//PD ������� �ܾ�
	FCAOutData[3][0] = FCAKData[13][0] * d_w - FCAKData[13][1]*dd_w;
}

//��ת��Ϊ�Զ���ʻģʽʱ�̱��Ϊ0
uint8 OnSwitchAuto=0xFF;


//��OnSwitchAuto�����ۼӣ���ʵ��ƽ����
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

//���������FCAOutData���ݸ�PWMOut������֮ǰ��λ;����ģʽ����
//����ʱ��ͨ�����ģʽ��ɾֲ�����+�ֲ��ֶ���
void FCAPWMOut()
{
	//�ֶ�����ģʽManual
	//��Ͽ���ģʽMix
	int i;
	FCALimitPWM();
	if(RunState>=MODETarget0&&RunState<=MODETargetX)
	{
		PWMOut[0]=FCAOutData[2][0]*FCATarData[3][0]+FCAOutData[4][0]*(1-FCATarData[3][0]);
		PWMOut[1]=FCAOutData[2][1]*FCATarData[3][1]+FCAOutData[4][1]*(1-FCATarData[3][1]);
		PWMOut[2]=FCAOutData[2][2]*FCATarData[3][2]+FCAOutData[4][2]*(1-FCATarData[3][2]);
		PWMOut[3]=FCAOutData[3][0]*FCATarData[4][0]+FCAOutData[5][0]*(1-FCATarData[4][0]);
		PWMOut[4]=FCAOutData[3][1]*FCATarData[4][1]+FCAOutData[5][1]*(1-FCATarData[4][1]);
	}
	//��λ����
	PWMOut[0]+=FCAMedianData[4][0];
	PWMOut[1]+=FCAMedianData[4][1];
	PWMOut[2]+=FCAMedianData[4][2];
	PWMOut[3]+=FCAMedianData[5][0];
	PWMOut[4]+=FCAMedianData[5][1];
	//�ٴν�����λ��
	for (i=0;i<5;i++)
	{
		if(PWMOut[i]>1)
			PWMOut[i]=1;
		else if(PWMOut[i]<-1)
			PWMOut[i]=-1;
	}	
}
