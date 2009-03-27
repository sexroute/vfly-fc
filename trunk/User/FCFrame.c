/*
	FCFrame.c
	飞控模块与地面站通讯协议解析与执行模块
	BUAA 2008-7-26
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "Main.h"
#include "User.h"
#include "FCFrame.h"

//UART0 原始数据
extern uint8 FCData[6+15*15];
extern uint8 FCFrameFlag;

//待发送的数据
FCFrameStruct FCFrameObjSend;//FC帧头对象
uint8 FCDataSend[15*FCBlockLength]={0};//待发送的数据区数据
//待记录的数据
FCFrameStruct FCFrameObjRec;//FC帧头对象
uint8 FCDataRec[15*FCBlockLength]={0};//待发送的数据区数据

//For E
uint8 ReturnProtocolNum=0;//返回的协议个数
uint8 RecordProtocolNum=0;//记录的协议个数
extern uint32 FCFrameReceive,FCFrameLost;
extern uint32 INSFrameReceive,INSFrameLost;

//发送帧头
uint8 FCFrameSend()
{
	FCFrameObjSend.S1=FCFrameS1;
	FCFrameObjSend.S2=FCFrameS2;
	UARTBufferSend(FCUARTPORT,(uint8*)&FCFrameObjSend, sizeof(FCFrameObjSend));
	return sizeof(FCFrameObjSend);
}

//发送数据块
uint8 FCBlockSend(uint8 Num)
{
	uint8 i=0,j=0,sum=0,p=0,t=0;
	for(j=0;j<Num;j++)
	{
		sum=0;
		t=FCBlockLength-1;
		for(i=0;i<t;i++)
		{
			sum+=FCDataSend[p];
			p++;
		}
		FCDataSend[p]=sum;
		p++;
	}
	UARTBufferSend(FCUARTPORT,FCDataSend, Num*FCBlockLength);
	return Num*FCBlockLength;
}

uint8 FCSend(uint8 Reserve,uint8 BlockNum)
{
	uint8 n=0;
	//设置帧头
	FCFrameObjSend.Reserve=Reserve;
	FCFrameObjSend.BlockNum=BlockNum;
	//发送数据
	n+=FCFrameSend();
	if(BlockNum>0)
	{
		n+=FCBlockSend(BlockNum);
	}
	return n;
}

//发送文字信息,字符最多不能超多225个.
uint8 FCMessageSend(const char *str)
{
	uint8 len=strlen(str),p=0,n=0,d=0,r=0;
	while(p<len&&n<=15)
	{
		uint32 t,s;
		memset(&FCDataSend[n*FCBlockLength], 0, FCBlockLength);//清空数据
		FCDataSend[n*FCBlockLength]=0x10;//命令字

		//计算需复制字符个数
		d=len-p;
		if(d>FCBlockLength-3)
		{
			d=FCBlockLength-3;
		}
		//复制字符
		s=n*FCBlockLength+2;
		t=n*(FCBlockLength-3);
		memcpy(&FCDataSend[s],(str+t),d);
		p+=d;
		n++;
	}
	
	r=FCSend(0,n);
	return r;
}

//发送错误信息,包括1个错误ID,3个错误参数及文字信息,文字信息不能超过8个字符.
uint8 FCErrorSend(uint8 ErrorID,uint8 p1,uint8 p2,uint8 p3,const char *str)
{
	uint8 d=0,r=0;
	memset(FCDataSend, 0, FCBlockLength);//清空数据
	FCDataSend[0]=0x11;//命令字
	FCDataSend[1]=0x00;
	FCDataSend[2]=ErrorID;
	FCDataSend[3]=p1;
	FCDataSend[4]=p2;
	FCDataSend[5]=p3;

	d=strlen(str);
	if(d>8)
	{
		d=8;
	}
	memcpy(&FCDataSend[6], str, d);
	
	r=FCSend(0,1);
	return r;
}

//发送指定事件
uint8 FCEventSend(uint8 EventID)
{
	uint8 r=0;
	r=FCSend(EventID,0);
	return r;
}

//On BlockNum=0
void Process0()
{
	if(FCFrameObj.Reserve>=MODEManual&&FCFrameObj.Reserve<=MODEFactory)
	{
		//ChangeRunMode(FCFrameObj.Reserve);
		//不使用ChangeRunMode函数是为了使每次接到指令后都有返回数据
		RunState=FCFrameObj.Reserve;
		FCEventSend(RunState);
		if(FCFrameObj.Reserve>=MODEMix&&FCFrameObj.Reserve<=MODETargetX)//将自驾模式指令保存到PreAutoMode
		{
			PreAutoMode=FCFrameObj.Reserve;
		}
	}
	else if(FCFrameObj.Reserve==0)
	{
		FCEventSend(RunState);
	}
	else if(FCFrameObj.Reserve==OKFPGA)
	{
		uint8 t=FPGACheck();
		if(t==TRUE)
		{
			FCEventSend(OKFPGA);
		}
		else
		{
			FCEventSend(ErrFPGA);
		}
	}
	else if(FCFrameObj.Reserve==CollectMedianData)
	{
		CollectMedianDataState=0;
	}
	else
	{
		FCBlockError++;
	}
}

//On Word=C&D
void ProcessCD(uint8 key,uint8 p)
{
	if(key>=0xC0&&key<=0xCC)
	{
		memcpy((uint8*)(&FCAData[key-0xC0][0]),&FCData[p+2],sizeof(fp32)*3);	
	}
	else if(key>=0xD0&&key<=0xD5)
	{
		memcpy((uint8*)(&FCAData[key-0xD0+0xCD-0xC0][0]),&FCData[p+2],sizeof(fp32)*3);
	}
}

//On Word=F
void ProcessF(uint8 key,uint8 p)
{
	uint8 j=0,word=0;
	if(key==0xF0)
	{
		for(j=p+2;j<p+FCBlockLength-1;j++)
		{
			word=FCData[j];
			DataSendFlag[word]=DataSendFlag[word]|0x01;
		}
	}
	if(key==0xF1)
	{
		for(j=p+2;j<p+FCBlockLength-1;j++)				
		{
			word=FCData[j];
			DataSendFlag[word]=DataSendFlag[word]|0x02;
		}
	}
	if(key==0xF2)
	{
		for(j=p+2;j<p+FCBlockLength-1;j++)				
		{
			word=FCData[j];
			DataSendFlag[word]=DataSendFlag[word]&0xFD;
		}
	}
	if(key==0xF3)
	{
		for(j=p+2;j<p+FCBlockLength-1;j++)				
		{
			word=FCData[j];
			DataSendFlag[word]=DataSendFlag[word]|0x04;
		}
	}
	if(key==0xF4)
	{
		for(j=p+2;j<p+FCBlockLength-1;j++)				
		{
			word=FCData[j];
			DataSendFlag[word]=DataSendFlag[word]&0xFB;
		}
	}
	ReturnProtocolNum=0;
	RecordProtocolNum=0;
	for(j=1;j<0xFF;j++)
	{
		uint8 t1,t2;
		t1=DataSendFlag[j]&0x02;
		t2=DataSendFlag[j]&0x04;
		if(t1==0x02)
		{
			ReturnProtocolNum++;
		}
		if(t2==0x04)
		{
			RecordProtocolNum++;
		}

	}
}

//For 1
//设置用于读取与写入的Flash芯片ID
uint8 FlashForRead=0,FlashForWrite=1;
//设置下载速度
uint8 DataDownloadSpeed=10;
extern uint16 WriteOffset,ReadOffset,WritePageNo,ReadPageNo;

//On Word=1
void Process1(uint8 key,uint8 p)
{
	if(key==0x12)
	{
		FlashForWrite=FCData[p+2];
		FlashForRead=FCData[p+3];
		ChangeWritePageNo(U8toU16(FCData[p+4],FCData[p+5]));
		ChangeReadSet(U8toU16(FCData[p+8],FCData[p+9]),U8toU16(FCData[p+10],FCData[p+11]));
	}
	if(key==0x13)
	{
		FlashChipSelect(FlashForWrite);
		StreamWrite(&FCData[p+2],FCData[p+1]);
		StreamWriteForce();//Better

	}
	if(key==0x14)
	{
		DataDownloadSpeed=FCData[p+2];
		DataToDownload=U8toU32(FCData[p+3], FCData[p+4], FCData[p+5], FCData[p+6]);
	}
	if(key==0x15)
	{
		uint16 s,l,t;
		FlashChipSelect(FCData[p+2]);
		s=U8toU16(FCData[p+3],FCData[p+4]);
		l=U8toU16(FCData[p+5],FCData[p+6]);
		for(t=0;t<l;t++)
		{
			PageErase(s+t);
		}
	}
}

//On Word=E2
void ProcessE2(uint8 key,uint8 p)
{
	if(key==0xE2)
	{
		DataSendDF=FCData[p+2];
		DataRecDF=FCData[p+3];
	}
}

/*
0x50~0x5F ReadOnly
//On Word=5
void Process5(uint8 key,uint8 p)
{
	memcpy((uint8*)(&FCAOutData[key-0x50][0]),&FCData[p+2],sizeof(fp32)*3);	
}
*/


//On Word=6
void Process6(uint8 key,uint8 p)
{
	memcpy((uint8*)(&FCAMedianData[key-0x60][0]),&FCData[p+2],sizeof(fp32)*3);	
}


//On Word=7
void Process7(uint8 key,uint8 p)
{
	memcpy((uint8*)(&FCATarData[key-0x70][0]),&FCData[p+2],sizeof(fp32)*3);	
}


//On Word=8
void Process8(uint8 key,uint8 p)
{
	memcpy((uint8*)(&FCAKData[key-0x80][0]),&FCData[p+2],sizeof(fp32)*3);	
}


//处理协议数据
void FCFrameProcess()
{
	if(FCFrameFlag==1)
	{
		uint8 i=0,j=0,p=0,sum=0,key=0;
		memcpy((uint8*)(&FCFrameObj),FCData,FCFrameLength);
		if(FCFrameObj.BlockNum==0)
		{
			Process0();
		}
		for(i=0;i<FCFrameObj.BlockNum;i++)
		{
			sum=0;
			p=FCFrameLength+i*FCBlockLength;
			for(j=p;j<p+FCBlockLength-1;j++)				
			{
				sum+=FCData[j];
			}
			if(sum==FCData[p+FCBlockLength-1])
			{
				key=FCData[p];
				if(key>=0xC0&&key<=0xD5)
				{
					ProcessCD(key, p);
				}
				else if(key>=0xF0&&key<=0xF4)
				{
					ProcessF(key,p);
				}
				else if(key>=0x12&&key<=0x15)
				{
					Process1(key,p);
				}
				else if(key==0xE2)
				{
					ProcessE2(key,p);
				}
				/*
				else if(key>=0x50&&key<=0x5F)
				{
					Process5(key,p);
				}
				*/
				else if(key>=0x60&&key<=0x65)
				{
					Process6(key,p);
				}
				else if(key>=0x70&&key<=0x74)
				{
					Process7(key,p);
				}
				else if(key>=0x80&&key<=0x8D)
				{
					Process8(key,p);
				}
				//TODO:Add Other Process
				else
				{
					FCBlockError++;
				}
			}
			else
			{
				FCBlockError++;
			}
		}
		FCFrameFlag=0;
	 }
}

/*数据记录发送部分*/
//待发送数据块个数,待记录数据块个数,发送数据的阀值,记录数据的阀值.
uint8 DataInSend=0,DataInRec=0,NumToSend=10,NumToRec=10;
//数据缓存
uint8 BlockBuffer[FCBlockLength];

//将待发送的数据放入BlockBuffer
//可查询的协议必须在此有对应的实现
uint8 MakeBlock(uint8 word)
{
	uint8 r=FALSE;
	memset(BlockBuffer, 0, sizeof(BlockBuffer));
	BlockBuffer[0]=word;
	if(word==0xB0)
	{
		BlockBuffer[1]=INSFrameObj.Status;
		memcpy(&BlockBuffer[2], (((uint8*)&INSFrameObj)+5), sizeof(int16)*6);
		r=TRUE;
	}
	else if(word==0xB1)
	{
		memcpy(&BlockBuffer[2], (((uint8*)&INSFrameObj)+5+sizeof(int16)*6), sizeof(int16)*3);
		r=TRUE;
	}
	else if(word==0xB2)
	{
		memcpy(&BlockBuffer[2], (((uint8*)&INSFrameObj)+5+sizeof(int16)*9), sizeof(int32)*3);
		r=TRUE;
	}
	else if(word==0xB3)
	{
		memcpy(&BlockBuffer[2], (((uint8*)&INSFrameObj)+5+sizeof(int16)*9+sizeof(int32)*3), sizeof(uint8)*6);
		r=TRUE;
	}
	else if(word==0xB4)
	{
		memcpy(&BlockBuffer[2], (((uint8*)&INSFrameObj)+5+sizeof(int16)*9+sizeof(int32)*3+sizeof(uint8)*6),  sizeof(int16)*6);
		r=TRUE;
	}
	else if(word>=0xC0&&word<=0xCC)
	{
		memcpy(&BlockBuffer[2], &FCAData[word-0xC0],  sizeof(int32)*3);
		r=TRUE;
	}
	else if(word>=0xD0&&word<=0xD5)
	{
		memcpy(&BlockBuffer[2], &FCAData[word-0xD0+0xCD-0xC0],  sizeof(int32)*3);
		r=TRUE;
	}
	else if(word==0x12)
	{
		BlockBuffer[2]=FlashForWrite;
		BlockBuffer[3]=FlashForRead;
		
		BlockBuffer[4]=WritePageNo;
		BlockBuffer[5]=(WritePageNo>>8);
		
		BlockBuffer[6]=WriteOffset;
		BlockBuffer[7]=(WriteOffset>>8);
				
		BlockBuffer[8]=ReadPageNo;
		BlockBuffer[9]=(ReadPageNo>>8);
		
		BlockBuffer[10]=ReadOffset;
		BlockBuffer[11]=(ReadOffset>>8);
		r=TRUE;
	}
	else if(word==0x14)
	{
		BlockBuffer[2]=DataDownloadSpeed;
		BlockBuffer[3]=(uint8)(DataToDownload);
		BlockBuffer[4]=(uint8)(DataToDownload>>8);
		BlockBuffer[5]=(uint8)(DataToDownload>>16);
		BlockBuffer[6]=(uint8)(DataToDownload>>24);
		r=TRUE;
	}
	else if(word==0xE0)
	{
		BlockBuffer[2]=(uint8)(INSFrameReceive);
		BlockBuffer[3]=(uint8)(INSFrameReceive>>8);
		BlockBuffer[4]=(uint8)(INSFrameReceive>>16);
		BlockBuffer[5]=(uint8)(INSFrameReceive>>24);

		BlockBuffer[6]=(uint8)(INSFrameLost);
		BlockBuffer[7]=(uint8)(INSFrameLost>>8);
		BlockBuffer[8]=(uint8)(INSFrameLost>>16);
		BlockBuffer[9]=(uint8)(INSFrameLost>>24);

		BlockBuffer[10]=(uint8)(INSFrameError);
		BlockBuffer[11]=(uint8)(INSFrameError>>8);
		BlockBuffer[12]=(uint8)(INSFrameError>>16);
		BlockBuffer[13]=(uint8)(INSFrameError>>24);
		r=TRUE;
	}
	else if(word==0xE1)
	{
		BlockBuffer[2]=(uint8)(FCFrameReceive);
		BlockBuffer[3]=(uint8)(FCFrameReceive>>8);
		BlockBuffer[4]=(uint8)(FCFrameReceive>>16);
		BlockBuffer[5]=(uint8)(FCFrameReceive>>24);

		BlockBuffer[6]=(uint8)(FCFrameLost);
		BlockBuffer[7]=(uint8)(FCFrameLost>>8);
		BlockBuffer[8]=(uint8)(FCFrameLost>>16);
		BlockBuffer[9]=(uint8)(FCFrameLost>>24);

		BlockBuffer[10]=(uint8)(FCBlockError);
		BlockBuffer[11]=(uint8)(FCBlockError>>8);
		BlockBuffer[12]=(uint8)(FCBlockError>>16);
		BlockBuffer[13]=(uint8)(FCBlockError>>24);
		r=TRUE;
	}
	else if(word==0xE2)
	{
		BlockBuffer[2]=DataSendDF;
		BlockBuffer[3]=DataRecDF;
		r=TRUE;
	}
	else if(word==0xE3)
	{
		BlockBuffer[2]=(uint8)(MainLoopRunCounter);
		BlockBuffer[3]=(uint8)(MainLoopRunCounter>>8);
		BlockBuffer[4]=(uint8)(MainLoopRunCounter>>16);
		BlockBuffer[5]=(uint8)(MainLoopRunCounter>>24);

		BlockBuffer[6]=RunState;
		BlockBuffer[7]=ReturnProtocolNum;
		BlockBuffer[8]=RecordProtocolNum;
		BlockBuffer[9]=ErrorNum;

		BlockBuffer[10]=LastErrorNo;
		BlockBuffer[11]=LastErrorPara[0];
		BlockBuffer[12]=LastErrorPara[1];
		BlockBuffer[13]=LastErrorPara[2];
		r=TRUE;
	}
	else if(word>=0x50&&word<=0x5F)
	{
		memcpy(&BlockBuffer[2], &FCAOutData[word-0x50],  sizeof(fp32)*3);
		r=TRUE;
	}
	else if(word>=0x60&&word<=0x65)
	{
		memcpy(&BlockBuffer[2], &FCAMedianData[word-0x60],  sizeof(fp32)*3);
		r=TRUE;
	}
	else if(word>=0x70&&word<=0x74)
	{
		memcpy(&BlockBuffer[2], &FCATarData[word-0x70],  sizeof(fp32)*3);
		r=TRUE;
	}
	else if(word>=0x80&&word<=0x8D)
	{
		memcpy(&BlockBuffer[2], &FCAKData[word-0x80],  sizeof(fp32)*3);
		r=TRUE;
	}
	//TODO:Add Other Block
	return r;
}

//将数据写入Flash
uint8 FCRec(uint8 Reserve,uint8 BlockNum)
{
	uint8 i=0,j=0,sum=0,p=0,t=0;
	//设置帧头
	FCFrameObjRec.S1=FCRecFrameS1;
	FCFrameObjRec.S2=FCRecFrameS2;
	FCFrameObjRec.Source=0;
	FCFrameObjRec.Target=0;
	FCFrameObjRec.Reserve=Reserve;
	FCFrameObjRec.BlockNum=BlockNum;
	//发送数据
	FlashChipSelect(FlashForWrite);
	StreamWrite((uint8*)&FCFrameObjRec,sizeof( FCFrameObjRec));
	for(j=0;j<BlockNum;j++)
	{
		sum=0;
		t=FCBlockLength-1;
		for(i=0;i<t;i++)
		{
			sum+=FCDataRec[p];
			p++;
		}
		FCDataRec[p]=sum;
		p++;
	}
	if(BlockNum>0)
	{
		StreamWrite(FCDataRec, FCBlockLength*BlockNum);
	}
	StreamWriteForce();
	return BlockNum*FCBlockLength;
}

uint8 DataReturnStart=0x01,DataReturnEnd=0xFF;

//发送并记录指定数据
void DataReturn(uint8 Part)
{
	uint8 i=0;
	for(i=DataReturnStart;i<DataReturnEnd;i++)
	{
		if(Part==0)
		{
			uint8 t1=0,t2=0;
			//WARN:切勿轻易更改条件计算，编译器存在bug!
			t1=DataSendFlag[i]&0x01;
			t2=DataSendFlag[i]&0x02;
			if(t1==0x01||t2==0x02)
			{
				uint8 r=MakeBlock(i);
				if(r==TRUE)
				{
					memcpy(&FCDataSend[DataInSend*FCBlockLength], BlockBuffer, sizeof(BlockBuffer));
					DataInSend++;
					if(DataInSend>=NumToSend)
					{
						FCSend(0, DataInSend);
						DataInSend-=NumToSend;
					}
				}
			}
			DataSendFlag[i]&=0xFE;
		}
		if(Part==1)
		{
			uint8 t1=0;
			//WARN:切勿轻易更改条件计算，编译器存在bug!
			t1=DataSendFlag[i]&0x04;
			if(t1==0x04)
			{
				uint8 r=MakeBlock(i);
				if(r==TRUE)
				{
					memcpy(&FCDataRec[DataInRec*FCBlockLength], BlockBuffer, sizeof(BlockBuffer));
					DataInRec++;
					if(DataInRec>=NumToRec)
					{
						FCRec(0, DataInRec);
						DataInRec-=NumToRec;
					}
				}
			}
		}
	}
	if(DataInSend>0)
	{
		FCSend(0, DataInSend);
		DataInSend=0;
	}
	if(DataInRec>0)
	{
		FCRec(0, DataInRec);
		DataInRec=0;
	}
}

uint8 DownloadBuffer[50];

//当有数据需下载时返回指定数据
void DataDownload()
{
	uint32 d=DataDownloadSpeed;
	if(DataToDownload>0)
	{
		if(DataToDownload<d)
		{
			d=DataToDownload;
		}
		if(d>50)
		{
			d=50;
		}
		FlashChipSelect(FlashForRead);
		d=StreamRead(DownloadBuffer, d);
		UARTBufferSend(FCUARTPORT, DownloadBuffer,d);
		DataToDownload-=d;
	}
}

