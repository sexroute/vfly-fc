/*
	CAN.c
	CAN总线收发驱动程序
	提供初始化、数据帧发送(群发)、中断方式接收、基于ID的数据解析。
	修改自周立功相关示例程序。
	BUAA 2008-7-23
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "CAN.h"

MessageDetail TempFrame;//发送帧
MessageDetail CAN0Frame;//CAN0接收帧
MessageDetail CAN1Frame;//CAN1接收帧

uint8 EnterSWReset(uint8 Ch);
uint8 QuitSWReset(uint8 Ch);
void __irq CANIRQ(void);

//初始化CAN总线控制器
//Ch为0-1
//Baud(速率)使用CAN.h中的已定义值
uint8 CANInit(uint8 Ch, uint32 Baud)
{
	uint32 regaddr;
	
	switch(Ch)//配置CAN控制器引脚
	{
		case 0:
			PCONP |= 0x01L<<13;//打开CAN1控制器电源
			PINSEL0 &= ~(0x03L<<0);//RD1
			PINSEL0 |= (0x01L<<0);
			PINSEL0 &= ~(0x03L<<2);//TD1
			PINSEL0 |= (0x01L<<2);
			break;
		case 1:
			PCONP |= 0x01L<<14;//打开CAN1控制器电源
			PINSEL0 &= ~(0x03L<<8);//RD2
			PINSEL0 |= (0x02L<<8);
			PINSEL0 &= ~(0x03L<<10);//TD2
			PINSEL0 |= (0x02L<<10);
			break;
		default:
			break;
	}
	
	EnterSWReset(Ch);
	
	regaddr = (uint32)(&CAN1BTR)+Ch*CANOFFSET;
	RGE(regaddr) = Baud;
	
	CAN_AFMR &= ~0x07;//验收过滤设置为旁路模式
	CAN_AFMR|=(1<<1);

	if(Ch == 0)
	{
		CAN1CMR |= (0x01<<2);//释放接收缓冲区
		CAN1ICR &= ~(0x01);
		CAN1IER |= ((1<<0)|(1<<2));//使能CAN0接收与错误报警中断
	}
	else
	{
		CAN2CMR |= (0x01<<2);
		CAN2ICR &= ~(0x01);
		CAN2IER |= ((1<<0)|(1<<2));//使能CAN1接收与错误报警中断
	}

	if(InstallIRQ(CAN_INT,(void *)CANIRQ,HIGHEST_PRIORITY)==FALSE)
	{
		return FALSE;
	}

	QuitSWReset(Ch);
	
	return TRUE;
}

//CAN控制器进入软件复位模式
uint8 EnterSWReset(uint8 Ch)
{
	uint32 regaddr;
	REG_CANMOD regmod;
	regaddr = (uint32)(&CAN1MOD)+Ch*CANOFFSET;			
	regmod.DWord = RGE(regaddr);//读取CAN1MOD寄存器
	regmod.Bits.RM = 1;//RM位置“1”
	RGE(regaddr) = regmod.DWord;//回写CAN1MOD寄存器
	regmod.DWord = RGE(regaddr);//验证写入值	
	return (0 != regmod.Bits.RM)? TRUE:FALSE; 
}

//CAN控制器退出软件复位模式
uint8 QuitSWReset(uint8 Ch)
{
	uint32 regaddr;
	REG_CANMOD regmod;
	regaddr = (uint32)(&CAN1MOD)+Ch*CANOFFSET;			
	regmod.DWord = RGE(regaddr);//读取CAN1MOD寄存器
	regmod.Bits.RM = 0;//RM位置“0”
	RGE(regaddr) = regmod.DWord;//回写CAN1MOD寄存器
	regmod.DWord = RGE(regaddr);//验证写入值	
	return (0 != regmod.Bits.RM)? FALSE:TRUE; 
}

//接收CAN帧并存放在指定的结构体中 
uint8 CANReceiveFrame(uint8 Ch, MessageDetail *Frame)
{
	uint32 mes;
	uint32 regaddr;

	regaddr = (uint32)(&CAN1RFS)+Ch*CANOFFSET;//处理DLC、RTR、FF
	mes = RGE(regaddr);
	Frame->LEN =  (mes >>16) & 0x0F;//获取帧长度
	Frame->FF  =  (mes >>31);//获取FF

	regaddr = (uint32)(&CAN1RID)+Ch*CANOFFSET;//处理ID
	mes = RGE(regaddr);
	
	if(Frame->FF)//FF为1，ID为29位
	{
		Frame->CANID = mes & 0x1fffffff;
	}
	else//FF为0 ，ID为11位
	{	
		Frame->CANID = mes & 0x000007ff;
	}
	
	regaddr = (uint32)(&CAN1RDA)+Ch*CANOFFSET;//处理 数据A
	mes = RGE(regaddr);
	Frame->DATAA = mes; 
   
	regaddr = (uint32)(&CAN1RDB)+Ch*CANOFFSET;//处理 数据B
	mes = RGE(regaddr);
	Frame->DATAB = mes;

	return TRUE;
}

//向指定帧结构体中写入数据
//LEN数据长度(必须<=8，剩余补零)
//FF扩展帧模式，在此例中需为TRUE:1。
//ID29位数据，约定为5位循环ID+8位源地址+8位目标地址+8位命令字依次从高到低。
//data帧数据(8byte)
//Frame目标数据帧
uint8 CANWriteFrame(uint8 LEN,uint8 FF, uint32 ID,uint8 *data,MessageDetail *Frame)
{
	uint8 i;
	
	if(LEN>8)
	{
		return FALSE;
	}
	else
	{
		Frame->LEN=LEN;
		Frame->FF=FF;
		Frame->CANID=ID;
		Frame->DATAA=0;//先清零
		Frame->DATAB=0;

		//此处决定数据顺序
		for(i=0;i<LEN;i++)
		{
			if(i<4)
			{
				Frame->DATAA |= (*(data+i)<<(i*8)); 
			}
			else
			{
				Frame->DATAB |= (*(data+i)<<((i-4)*8));
			}
		}
	//Frame->DATAB = ID;	
	return TRUE;
	}
}

//发送指定帧
//BufNum发送所使用的缓存号，一般性建议等于通道号。
//Frame待发送帧
uint8 CANSendFrame(uint8 Ch, uint8 BufNum,MessageDetail *Frame)
{
	uint32 CAN32reg;
	uint32 regaddr;
	uint8 FFflag,t;
	
	//BufNum-=1;		// 计算地址方便。//TODO:DEL
	
	regaddr = (uint32)(&CAN1TFI1)+Ch*CANOFFSET+BufNum*0X10;// 3缓冲区间地址差 0x10
	CAN32reg = RGE(regaddr);
	CAN32reg &= ~((0x0fL<<16) | (0x01L<<30) | (0x80000000));//清 DLC,RTR.FF位
	CAN32reg |= ((uint32)(*Frame).LEN<<16) | ((uint32)(*Frame).FF<<31);
	RGE(regaddr)=CAN32reg;
	
	FFflag = (*Frame).FF;
	
	regaddr = (uint32)(&CAN1TID1)+Ch*CANOFFSET+BufNum*0X10;//写帧ID
	CAN32reg = RGE(regaddr);
	if(FFflag)//FF为1，ID为29位
	{
		CAN32reg &=0x70000000;
		CAN32reg |= ((*Frame).CANID & 0x1fffffff);
	}
	else
	{
		CAN32reg &= 0xfffff800;//FF为0 ，ID为11位
		CAN32reg |= ((*Frame).CANID & 0x000007ff);
	}
	RGE(regaddr)=CAN32reg;
	
	regaddr = (uint32)(&CAN1TDA1)+Ch*CANOFFSET+BufNum*0X10;// 写帧数据A
	RGE(regaddr) = (*Frame).DATAA;
	
	regaddr = (uint32)(&CAN1TDB1)+Ch*CANOFFSET+BufNum*0X10;// 写帧数据B
	RGE(regaddr) = (*Frame).DATAB;
	
	regaddr = (uint32)(&CAN1CMR)+Ch*CANOFFSET;//写控制寄存器，发送
	CAN32reg = RGE(regaddr);
	
	CAN32reg &= ~0x03;
	CAN32reg |= 0x03;
	CAN32reg &= ~((1<<5)|(1<<6)|(1<<7));
	switch(BufNum)
	{
		case 0: CAN32reg |= 01<<5;break;
		case 1: CAN32reg |= 01<<6;break;
		case 2: CAN32reg |= 01<<7;break;
		default: break;
	}
	RGE(regaddr)=CAN32reg;
	
	regaddr = (uint32)(&CAN1GSR)+Ch*CANOFFSET;//查询发送状态
	CAN32reg = RGE(regaddr);
	
	//IRQDisable();
	//r=FALSE;
	for(t=0;t<100;t++)
	{
		CAN32reg=RGE(regaddr);
		if(CAN32reg&(1<<3))
		{
			//r=TRUE;
			break;
		}
	}
	//IRQEnable();
	return TRUE;
}

//以指定的ID格式发送消息(推荐使用此函数来发送数据，以减少对驱动函数的直接调用。)
//帧ID约定为5位循环ID+8位源地址+8位目标地址+8位命令字依次从高到低。
//ID循环ID
//Source源设备地址
//Target目标设备地址
//Word命令字
//Len数据长度(必须<=8)
//data待发送的数据
uint8 CANSendMessage(uint8 Ch,uint8 ID,uint8 Source, uint8 Target,uint8 Word,uint8 Len,uint8 *data)
{
	uint32 t;
	MessageDetail* SendFrame;

	if(Ch>1||ID>0x1F||Len>8||data==NULL)
	{
		t=FALSE;
	}
	else
	{
		ID=ID&0x1F;
		t=(ID<<24)|(Source<<16)|(Target<<8)|(Word<<0);

		SendFrame=&TempFrame;
		//if(Ch==1)
		//{
		//	SendFrame=&CAN1Frame;
		//}
		if(CANWriteFrame(Len, 1, t, data, SendFrame)==TRUE)
		{
			t=FALSE;
			t=CANSendFrame(Ch,Ch,SendFrame);
		}
	}
	return t;
}

//接收到CAN总线上数据帧时被调用的Message处理函数(不要在此函数中执行周期过长的操作)
//ID循环ID
//Source源设备地址
//Target目标设备地址
//Word命令字
//Len数据长度(<=8)
//DATAA低8位数据
//DATAB高8位数据(具体参见发送过程)
//但数据帧为非扩展帧(即FF=0)时此函数不会被调用。
void CANProcessMessage(uint8 Ch,uint8 ID,uint8 Source, uint8 Target,uint8 Word,uint8 Len,uint32 DataA,uint32 DataB)
{
}

//CAN控制器中断处理函数
void __irq CANIRQ()
{
 	uint8 ch;
	uint32 CAN32reg,mes;
	uint32 regaddr;
	//IRQDisable();

	for(ch=0;ch<2;ch++)//最大CAN通道数为2，分别检测两个通道的中断 
	{
		regaddr = (uint32)(&CAN1ICR)+ch*CANOFFSET;	
		CAN32reg=RGE(regaddr);	
		
		if((CAN32reg&(1<<0))!= 0)	//RI 接收中断
		{
			MessageDetail* ReceiveFrame;
			
			CAN32reg &= ~(0x01);
			RGE(regaddr) = CAN32reg;

			ReceiveFrame=&CAN0Frame;
			if(ch==1)
			{
				ReceiveFrame=&CAN1Frame;
			}

			CANReceiveFrame(ch, ReceiveFrame);//收到CAN0中断,接收帧
			
			
			//Test Code
			if(ch==0)
			{
				UARTSendChar(0,0xCC);
				//CANSendFrame(0,0,ReceiveFrame);
				
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>24));
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>16));
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>8));
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>0));
				
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>24));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>16));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>8));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>0));
				
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>24));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>16));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>8));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>0));
				
			}
			else
			{
				CANSendFrame(1,1,ReceiveFrame);
			}
			

			if(ReceiveFrame->FF==1)
			{
				uint8 ID,Source,Target,Word;
				uint32 DATAA,DATAB;
				
				ID=ReceiveFrame->CANID>>24;
				ID=ID&0x1F;
				Source=ReceiveFrame->CANID>>16;
				Target=ReceiveFrame->CANID>>8;
				Word=ReceiveFrame->CANID>>0;
				DATAA=ReceiveFrame->DATAA;
				DATAB=ReceiveFrame->DATAB;
				
				CANProcessMessage(ch, ID,Source,Target,Word, ReceiveFrame->LEN, DATAA,DATAB);
			}
			
			if(ch==0)
			{
				regaddr = (uint32)(&CAN1CMR)+ch*CANOFFSET;				
				mes=RGE(regaddr);
				mes |= (1<<2);//释放接收缓冲区
				RGE(regaddr)=mes;
			}
			if(ch==1)
			{
				regaddr = (uint32)(&CAN1CMR)+ch*CANOFFSET;				
				mes=RGE(regaddr);
				mes |= (1<<2);//释放接收缓冲区
				RGE(regaddr)=mes;
			}
		}
		//根据需要添加
		/*
		if((CAN32reg&(1<<1))!= 0)// TI1 第一发送缓冲区发送完成中断

		{
			;
		}
		if((CAN32reg&(1<<9))!= 0)// TI2 第二发送缓冲区发送完成中断

		{
			;
		}
		if((CAN32reg&(1<<10))!= 0)// TI3 第三发送缓冲区发送完成中断
		{
			;
		}
		*/
		if((CAN32reg&(1<<7))!= 0)	// BEI 总线错误中断
		{
			EnterSWReset(ch);
			regaddr = (uint32)(&CAN1GSR)+ch*CANOFFSET;				
			mes=RGE(regaddr);
			mes &=0x00ff;												
			RGE(regaddr)=mes;//总线错误清除处理
			QuitSWReset(ch);
		}
		if((CAN32reg&(1<<6))!= 0)	// ALI 仲裁丢失中断
		{
			//添加用户代码
		}
		if((CAN32reg&(1<<3))!= 0)
		{
			//添加用户代码
		}
	}
	//IRQEnable();
	VICVectAddr = 0;	
}

