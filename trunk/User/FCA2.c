/*
	FCA2.c
	FCA飞控算法实现
	BUAA 2009-2-26
	吴永亮
*/

#include "Config.h"
#include "Drv.h"
#include "Main.h"
#include "User.h"
#include "FCA2.h"

#include <Math.h>

float LTLK=1.0;
//积分量：怎样进行重置:切换时获取中位，获取中位时重置；
float i_x = 0,i_y=0,i_h=0,i_yaw = 0;

//将算法输出的控制量限制在-1~1之间；
void FCALimitPWM()
{
	//限位	// 0
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

//获取导航信息和遥控器控制量信息
void FCAGetNav()
{
	float yy;
		
	//导航系位置 m
	LTLK=cos((double)INSFrameObj.La*3.14/180.0/600000.0);
	FCAOutData[7][0]=INSFrameObj.La*MtMk;
	FCAOutData[7][1]=INSFrameObj.Lo*LTLK*MtMk;
	FCAOutData[7][2]=INSFrameObj.Height/1000;
	
	//20090318 使用中位位置作为位置起点；
	FCAOutData[7][0]-=FCAMedianData[0][0];
	FCAOutData[7][1]-=FCAMedianData[0][1];
	FCAOutData[7][2]-=FCAMedianData[0][2];
	
	//导航系速度 m/s
	FCAOutData[8][0]=INSFrameObj.SpeedN/100.0;
	FCAOutData[8][1]=INSFrameObj.SpeedE/100.0;
	FCAOutData[8][2]=INSFrameObj.SpeedD/100.0;
	//导航系角度  1°
	FCAOutData[9][0]=-INSFrameObj.Pitch/10.0;		//将俯仰角反向；
	FCAOutData[9][1]=INSFrameObj.Roll/10.0;
	FCAOutData[9][2]=INSFrameObj.Yaw/10.0;
	
	//本体系角速率 1°/s
	FCAOutData[10][0]=-INSFrameObj.AngVeloBodyY/100.0; //pitch (俯仰角度和x轴速度相反)
	FCAOutData[10][1]=INSFrameObj.AngVeloBodyX/100.0; //roll
	FCAOutData[10][2]=INSFrameObj.AngVeloBodyZ/100.0; //yaw
	
	//pp=(-FCAOutData[9][0]) * 3.1415926 / 180;
	//rr=FCAOutData[9][1] * 3.1415926 / 180;
	yy=FCAOutData[9][2] * 3.1415926 / 180;
	
	//本体系位置（纠正航向角）
	FCAOutData[11][0]= cos(yy) * FCAOutData[7][0] + sin(yy) * FCAOutData[7][1];
	FCAOutData[11][1]=(-sin(yy)) * FCAOutData[7][0] + cos(yy) * FCAOutData[7][1];
	FCAOutData[11][2]= FCAOutData[7][2];		//z轴向上；

	//本体系速度1 1m/s
	FCAOutData[12][0]=INSFrameObj.SpeedBodyX/100.0;
	FCAOutData[12][1]=INSFrameObj.SpeedBodyY/100.0;
	FCAOutData[12][2]=INSFrameObj.SpeedBodyZ/100.0;//z轴向上；
					
	//本体系速度2解算 0.01m/s
	
	//FCAOutData[12][0]=cos(pp) *cos(yy) * u_tmp + cos(pp) * sin(yy) * v_tmp - sin(pp) * w_tmp;
	//FCAOutData[12][1]=(-cos(rr) * sin(yy) + sin(rr) * sin(pp) * cos(yy)) * u_tmp + (cos(rr) * cos(yy) + sin(rr) *sin(pp) * sin(yy)) * v_tmp + sin(rr) * cos(pp) * w_tmp;
	//FCAOutData[12][2]=(sin(rr) * sin(yy) + cos(rr) * sin(pp) * cos(yy)) * u_tmp + (-sin(rr) * cos(yy) + cos(rr) * sin(pp) * sin(yy)) * v_tmp +cos(rr) * cos(pp) * w_tmp;	
		
	//获取控制量信息 -1~1
	//能否配置正反切换？
	FCAOutData[4][0]=PWMIn[0];//俯仰
	FCAOutData[4][1]=PWMIn[1];//滚装
	FCAOutData[4][2]=PWMIn[2];//偏航
	
	FCAOutData[5][0]=PWMIn[3];//总距
	FCAOutData[5][1]=PWMIn[4];//油门
	//切换，陀螺仪感度、转速控制器
	FCAOutData[6][0]=PWMIn[6];
	FCAOutData[6][1]=PWMIn[7];
	FCAOutData[6][2]=PWMIn[8];
}


//获取悬停点信息
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
			FCAMedianDataTmp[0][i]+=FCAOutData[7][i];//导航系位置；
			//FCAMedianDataTmp[1][i]+=FCAOutData[12][i];//本体系速度 ，实际上一直为零；
			FCAMedianDataTmp[2][i]+=FCAOutData[9][i];//姿态；
			//FCAMedianDataTmp[3][i]+=FCAOutData[10][i];//角速度，实际上应该为零；
			//FCAMedianDataTmp[4][i]+=FCAOutData[4][i];//控制量：俯仰、滚转、偏航；
			//FCAMedianDataTmp[5][i]+=FCAOutData[5][i];//控制量：总距，油门；
		}
		CollectMedianDataState++;
	}
	else if(CollectMedianDataState==Long)
	{
		//位置
		FCAMedianData[0][0] = FCAMedianDataTmp[0][0];
		FCAMedianData[0][1] = FCAMedianDataTmp[0][1];
		FCAMedianData[0][2] = FCAMedianDataTmp[0][2];
		//航向角
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
			FCAMedianDataTmp[0][i]+=FCAOutData[7][i];//导航系位置；
			FCAMedianDataTmp[1][i]+=FCAOutData[12][i];//本体系速度 ，实际上一直为零；
			FCAMedianDataTmp[2][i]+=FCAOutData[9][i];//姿态；
			FCAMedianDataTmp[3][i]+=FCAOutData[10][i];//角速度，实际上应该为零；
			FCAMedianDataTmp[4][i]+=FCAOutData[4][i];//控制量：俯仰、滚转、偏航；
			FCAMedianDataTmp[5][i]+=FCAOutData[5][i];//控制量：总距，油门；
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

//飞行控制算法1：水平位置通道
void FCAPosXY()
{
	float yy;
	float x_tmp, y_tmp;
	
	float x_median, y_median;
	
	float x_now, y_now;
	float x_tar, y_tar;
	float d_x,d_y;

	float x_rem, y_rem;			//遥控器
	float x_cmd, y_cmd;			//上位机、
	float dt =0.025;

	// 1. 悬停中位中的位置为位置起点；由NE转化为x，y；
	//悬停点位置为导航系；所以需要准换；根据航向随时转换；
	yy=FCAOutData[9][2] * 3.1415926 / 180;
	
	//本体系位置（纠正航向角）
	x_median= cos(yy) * FCAMedianData[0][0] + sin(yy) * FCAMedianData[0][1];
	y_median=(-sin(yy)) * FCAMedianData[0][0] + cos(yy) * FCAMedianData[0][1];
	
	// 2. 获取当前位置：本体系相对距离
	x_now = FCAOutData[11][0]-x_median;
	y_now = FCAOutData[11][1]-y_median;
		
	// 3. 根据上位机指令获取目标悬停位置：导航系NE；可以升级为改变时解析
	x_tmp = FCATarData[0][0]-FCAMedianData[0][0];
	y_tmp = FCATarData[0][1]-FCAMedianData[0][1];
	
	x_cmd = cos(yy) * x_tmp + sin(yy) * y_tmp;
	y_cmd = (-sin(yy)) * x_tmp+cos(yy) * y_tmp;
	
	// 4. 遥控器接口:在位置遥控器模式下给定的是本体系位置增量；
	if(RunState==MODETargetX)
	{
		x_rem = FCAOutData[4][0]*20;	//+-20m;出死区范围了；
		y_rem = FCAOutData[4][1]*20;
	}
	else
	{
		x_rem = 0;
		y_rem = 0;
	}
	x_tar = x_cmd + x_rem;
	y_tar = y_cmd + y_rem;
	//增量
	d_x = x_tar - x_now;
	d_y = y_tar - y_now;
	
	//死区范围查询:矩形
	if( d_x < FCAKData[1][0] && d_x > (-FCAKData[1][0]) )
		d_x = d_x * FCAKData[1][1];
	
	if( d_y < FCAKData[3][0] && d_y > (-FCAKData[3][0]) )
		d_y = d_y * FCAKData[3][1];
	
	//积分；抗饱和积分,判断上次误差限
	//x积分
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
	//y积分
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
	
	//PI使用
	FCAOutData[0][0] = FCAKData[0][0]*d_x + FCAKData[0][2]*i_x;
	FCAOutData[0][1] = FCAKData[2][0]*d_y + FCAKData[2][2]*i_y;
	
	//饱和截止
	if ( FCAOutData[0][0] > FCAKData[1][2] )
		FCAOutData[0][0] = FCAKData[1][2];
	else if( FCAOutData[0][0] < -FCAKData[1][2] )
		FCAOutData[0][0] = -FCAKData[1][2];
		
	if ( FCAOutData[0][1] > FCAKData[3][2] )
		FCAOutData[0][1] = FCAKData[3][2];
	else if( FCAOutData[0][1] < -FCAKData[3][2] )
		FCAOutData[0][1] = -FCAKData[3][2];		
		
	//更新回馈量
	FCAOutData[13][0]=x_tar;
	FCAOutData[13][1]=y_tar;
}

//飞行控制算法2：水平速度通道
void FCAVelXY()
{
	//速度量
	float u_now,v_now;	//当前值
	float u_tar,v_tar;	//目标值
	float d_u,d_v;
	
	float u_pos,v_pos;	//速度闭环值
	float u_rem,v_rem;	//遥控值
	float u_cmd,v_cmd;	//上位机
	
	
	//姿态量
	float pit_now,rol_now;	//当前值
	float pit_tar,rol_tar;	//目标值
	float d_pit,d_rol;
	float p,q;
	
	float pit_rem,rol_rem;	//遥控值
	//float pit_cmd,rol_cmd;	//上位机
		
	//float i_u = 0;
	//float i_v = 0;
	//float dt = 0.025;//CHANGE
	
	//*************************计算增量信息:去除悬停点***************************************
	
	// 1. 当前速度和姿态量及角速度量；
	//计算速度
	//u_now = FCAOutData[12][0] - FCAMedianData[1][0];
	//v_now = FCAOutData[12][1] - FCAMedianData[1][1];
	u_now = FCAOutData[12][0];
	v_now = FCAOutData[12][1];
	
	//计算姿态
	pit_now = FCAOutData[9][0] - FCAMedianData[2][0];
	rol_now = FCAOutData[9][1] - FCAMedianData[2][1];
	//计算角速度
	//p = FCAOutData[14][0] - FCAMedianData[3][0];
	//q = FCAOutData[14][1] - FCAMedianData[3][1];
	p = FCAOutData[10][0];
	q = FCAOutData[10][1];
	//水平面速度积分
	//i_u +=FCAData[13][0]* dt;
	//i_v +=FCAData[13][1]* dt;
	
	// 2.上位机接口没有:给定的是本体系速度；
	u_cmd = FCATarData[1][0];
	v_cmd = FCATarData[1][1];
	
	//pit_cmd = FCATarData[2][0];
	//rol_cmd = FCATarData[2][1];
	// 3. 遥控器接口
	
	// 1速度遥控
	if(RunState==MODETargetV)
	{
		u_rem = FCAOutData[4][0]*50;	//-5~5 对应 -0.5m/s~0.5m/s
		v_rem = FCAOutData[4][1]*50; //
	}
	else
	{
		u_rem = 0;
		v_rem = 0;
	}	
	// 1姿态遥控
	if(RunState==MODETargetP)
	{
		pit_rem = FCAOutData[4][0]*50;  //-1~1 对应 -50°~50°
		rol_rem = FCAOutData[4][1]*100;	//-1~1 对应 -100°~100°
	}
	else
	{
		pit_rem = 0;
		rol_rem = 0;
	}
		
	// 4.位置闭环
	u_pos = FCAOutData[0][0];
	v_pos = FCAOutData[0][1];
	
	//****************************比较差值**********************************
	u_tar = u_pos + u_rem + u_cmd;
	d_u = u_tar - u_now;
	
	v_tar = v_pos + v_rem + v_cmd;
	d_v = v_tar - v_now;
	
	pit_tar = pit_rem;
	d_pit = pit_tar-pit_now;
	
	rol_tar = rol_rem;
	d_rol = rol_tar-rol_now;
	
	//俯仰
	FCAOutData[2][0]=FCAKData[4][0]*d_u + FCAKData[4][1]*d_pit - FCAKData[4][2]*p + FCAKData[5][1]*d_v + FCAKData[5][2]*d_rol;
	//滚转
	FCAOutData[2][1]=FCAKData[6][0]*d_v + FCAKData[6][1]*d_rol - FCAKData[6][2]*q + FCAKData[7][1]*d_u + FCAKData[7][2]*d_pit;
	
	//更新反馈量
	FCAOutData[14][0]=u_tar;
	FCAOutData[14][1]=v_tar;
	FCAOutData[15][0]=pit_tar;
	FCAOutData[15][1]=rol_tar;
}

//飞行控制算法3：垂直航向通道
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
	
	//********************************角度控制**********************************
	//上位机
	yaw_cmd = FCATarData[2][2];			
	yaw_now = FCAOutData[9][2];//-FCAMedianData[2][2]; //(0~359)-(0~359) 在-180~+180之间
	
	//位置遥控,姿态遥控，速率遥控；
	if(RunState >= MODETargetP&&RunState <= MODETargetX)
	{
		yaw_rem = FCAOutData[4][2]*30;		//当前角度的范围：航向角+-15度；
	}
	else
		yaw_rem = 0;
	
		//求解增量
	yaw_tar = yaw_rem+yaw_cmd;
	d_yaw = yaw_tar - yaw_now;
	
	while (d_yaw > 180)
     d_yaw -= 360;
	while (d_yaw <-180)
  	 d_yaw += 360;
	
	//死区范围查询:矩形
	if( d_yaw < FCAKData[9][0] && d_yaw > (-FCAKData[9][0]) )
		d_yaw = d_yaw * FCAKData[9][1];
		
	//积分；抗饱和积分,判断上次误差限
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
	//PI使用
	FCAOutData[1][0] = FCAKData[8][0]*d_yaw + FCAKData[8][2]*i_yaw;
		
	//饱和截止
	if ( FCAOutData[1][0] > FCAKData[9][2] )
		FCAOutData[1][0] = FCAKData[9][2];
	else if( FCAOutData[0][0] < -FCAKData[9][2] )
		FCAOutData[1][0] = -FCAKData[9][2];	
	
	//*******************速率控制 r =0；**************************************
	r_now = FCAOutData[10][2];//- FCAMedianData[3][2];
	
	r_tar = FCAOutData[1][0];
	d_r = r_tar - r_now;
	
	dd_r = d_r-d_r_last;
	d_r_last = d_r;
	
	//PD 输出舵量
	FCAOutData[2][2] = FCAKData[10][0] * d_r - FCAKData[10][1]*dd_r;	
	
	//给定回馈量
	FCAOutData[15][2] = yaw_tar;
}

//飞行控制算法4：垂直高度通道
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
	h_now = FCAOutData[7][2];//导航系
	
	if(RunState>=MODETargetP&&RunState<=MODETargetX)
	{
		h_rem = FCAOutData[5][0]*20;		//+-20m
	}
	else
		h_rem = 0;
	
	//求解增量
	h_tar = h_rem+h_cmd;
	
	d_h = h_tar - h_now;
	
	//死区范围查询:矩形
	if( d_h < FCAKData[12][0] && d_h > (-FCAKData[12][0]) )
		d_h = d_h * FCAKData[12][1];
		
	//积分；抗饱和积分,判断上次误差限
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
		
	//PI使用
	FCAOutData[0][2] = FCAKData[11][0]*d_h + FCAKData[11][2]*i_h;
		
	//饱和截止
	if ( FCAOutData[0][2] > FCAKData[12][2] )
		FCAOutData[0][2] = FCAKData[12][2];
	else if( FCAOutData[0][0] < -FCAKData[12][2] )
		FCAOutData[0][2] = -FCAKData[12][2];	
		
		
	//升降速率控制
	//*******************速率控制 w =0；**************************************
	w_now = FCAOutData[12][2];
	
	//速度遥控模式；姿态遥控模式
	//姿态遥控 && 速度遥控
	//if(RunMode == MODETargetP || RunMode == MODETargetV)
	//	w_rem = = FCAOutData[5][0]*50;
	//else
	w_rem = 0;
	
	w_tar = w_rem + FCAOutData[0][2];
	d_w = w_tar - w_now;
	
	dd_w = d_w-d_w_last;
	d_w_last = d_w;
	
	//PD 输出舵量 总距
	FCAOutData[3][0] = FCAKData[13][0] * d_w - FCAKData[13][1]*dd_w;
}

//当转换为自动驾驶模式时刻标记为0
uint8 OnSwitchAuto=0xFF;


//对OnSwitchAuto进行累加，并实现平滑。
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

//将输出量由FCAOutData传递给PWMOut；传递之前限位;根据模式进行
//调试时，通过混控模式完成局部自主+局部手动；
void FCAPWMOut()
{
	//手动控制模式Manual
	//混合控制模式Mix
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
	//中位数据
	PWMOut[0]+=FCAMedianData[4][0];
	PWMOut[1]+=FCAMedianData[4][1];
	PWMOut[2]+=FCAMedianData[4][2];
	PWMOut[3]+=FCAMedianData[5][0];
	PWMOut[4]+=FCAMedianData[5][1];
	//再次进行限位；
	for (i=0;i<5;i++)
	{
		if(PWMOut[i]>1)
			PWMOut[i]=1;
		else if(PWMOut[i]<-1)
			PWMOut[i]=-1;
	}	
}
