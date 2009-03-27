/*
	INS.c
	完成INS协议解析函数功能
	BUAA 2008-8-26
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "Main.h"
#include "User.h"
#include "INSFrame.h"

//INS原始数据from UART
extern uint8 INSData[INSFrameLength],INSFrameFlag;

/*
//INS解析后的数据
uint8 INSFrameLength,INSFrameType,INSFrameMask;
int16 INSNAV[15];
int32 INSGPS[3];
uint8 INSGPSNum;
int16 INSGPSAccuracy;
uint8 INSGPSTime[3];
*/

void U8toU16s(uint8* Source,int16* Target,uint8 Num)
{
	uint8 i;
	uint16 t;
	for(i=0;i<Num;i++)
	{
		t=U8toS16(Source[i*2],Source[i*2+1]);
		Target[i]=t;
	}
}

//检测串口数据当数据可用时进行解析
void INS()
{
	//uint8 data[INSFrameLength]={0};
	if(INSFrameFlag==1)
	 {
	 	if(INSData[2]==INSFrameLength-2&&INSData[3]==INSFrameS3)
		{
			uint8 i=0,sum=0;
			for(i=2;i<INSFrameLength-1;i++)
			{
				sum+=INSData[i];
			}
			if(sum==INSData[INSFrameLength-1])
			{
				//memcpy((uint8*)(&INSFrameObj),INSData,INSFrameLength);
				//UARTSendChar(0,0xFF);
			}
			else if(sum-1==INSData[INSFrameLength-1])
			{
				memcpy((uint8*)(&INSFrameObj),INSData,INSFrameLength);
				//UARTSendChar(0,0xFD);
			}
			else
			{
				INSFrameError++;
				//UARTSendChar(0,0xFE);					
			}
		}
		else
		{
			INSFrameError++;
			//UARTSendChar(0,0xFE);
		}
		INSFrameFlag=0;//CHANGE:还原
	 	/*
		memcpy(data,INSData,INSFrameLength);
		INSFrameFlag=0;
		if(data[0]==INSFrameLength&&data[1]==INSFrameS3)
		{
			uint8 i=0,sum=0;
			for(i=0;i<INSFrameLength-1;i++)
			{
				sum+=data[i];
			}
			if(sum==data[INSFrameLength-1])
			{
				//帧信息
				INSFrameLength=data[0];
				INSFrameType=data[1];
				INSFrameMask=data[2];
				
				//GPS信息
				INSGPSNum=data[33];
				INSGPSAccuracy=U8toS16(data[34],data[35]);
				INSGPSTime[0]=data[36];
				INSGPSTime[1]=data[37];
				INSGPSTime[2]=data[38];

				//INSNAV
				//俯仰、滚转、偏航角
				if(INSFrameMask&0x01==0x01)//Bit0
				{
					U8toU16s(&data[3],&INSNAV[0], 3);
				}
				//北东地速度
				if(INSFrameMask&0x02==0x02)//Bit1
				{
					U8toU16s(&data[9],&INSNAV[3], 3);
				}
				//载体系速度
				if(INSFrameMask&0x04==0x04)//BIt2
				{
					U8toU16s(&data[15],&INSNAV[6], 3);
				}
				//载体系加速度
				if(INSFrameMask&0x20==0x20)//Bit5
				{
					U8toU16s(&data[39],&INSNAV[9], 3);
				}
				//载体系角速率
				if(INSFrameMask&0x40==0x40)//Bit6
				{
					U8toU16s(&data[45],&INSNAV[12], 3);
				}

				//INSGPS
				//经纬度
				if(INSFrameMask&0x08==0x08)//Bit3
				{
					INSGPS[0]=U8toS32(data[21],data[22],data[23],data[24]);
					INSGPS[1]=U8toS32(data[25],data[26],data[27],data[28]);
				}
				//海平面高度
				if(INSFrameMask&0x10==0x10)//Bit4
				{
					INSGPS[2]=U8toS32(data[29],data[30],data[31],data[32]);
				}
			}
			else
			{
				INSFrameError++;					
			}
		}
		else
		{
			INSFrameError++;
		}
		*/
		
		//UARTSendChar(0,INSFrameS0);
		//UARTSendChar(0,INSFrameS1);
		//UARTSend(0,data, INSFrameLength);
	 }
}

