/*
	UART.c
	UART驱动程序
	提供初始化、查询方式发送、中断方式接收、列队方式协议解析。
	BUAA 2008-7-11
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "UART.h"
#include "Queue.h"

#include "INSFrame.h"
#include "FCFrame.h"
#include "Main.h"

QueueObj UART0Queue;
QueueObj UART1Queue;
QueueObj UART2Queue;
QueueObj UART3Queue;

QueueObj UART0SendQueue;

//测试用数据
volatile uint8 C[3],F;//TODO=DEL
//INS模块数据
volatile uint8 INSData[INSFrameLength]={0};
volatile uint8 INSFrameFlag=0;
uint32 INSFrameReceive=0,INSFrameLost=0;

//FC模块数据
volatile uint8 FCData[6+15*15]={0};
volatile uint8 FCFrameFlag=0;
uint32 FCFrameReceive=0,FCFrameLost=0;

//UART0中断响应函数
void __irq UART0Handler (void) 
{
	uint8 c;
	//发送中断处理
	c=U0LSR&0x20;	//WARN:避免条件判断错误，编译器存在Bug。
	if(c==0x20)
	{
		uint8 i=0;
		for(i=0;(i<16)&&UART0SendQueue.Count>0;i++)
		{
			U0THR=QueueFetch(&UART0SendQueue);
		}
	}
	c=U0IIR;//清除中断
  	while((U0LSR&0x01)==1)//循环读取
  	{
  		c=U0RBR;
  		//UARTSendChar(0, c);
		QueueInsert(&UART0Queue,c);
		while(UART0Queue.Count>0)
		{
			if(QueueRead(&UART0Queue,0)!=FCFrameS1)
			{
				QueueFetch(&UART0Queue);
			}
			else
			{
				break;
			}
		}
		//FC帧处理代码
		if(UART0Queue.Count>=FCFrameLength&&QueueRead(&UART0Queue,0)==FCFrameS1&&QueueRead(&UART0Queue,1)==FCFrameS2&&UART0Queue.Count>=(QueueRead(&UART0Queue,5)*FCBlockLength+FCFrameLength))
		{
			uint8 i,len=QueueRead(&UART0Queue,5)*FCBlockLength+FCFrameLength;
			FCFrameReceive++;
			if(FCFrameFlag==1)
			{
				FCFrameLost++;
			}
			for(i=0;i<len;i++)
			{
				FCData[i]=QueueFetch(&UART0Queue);
			}
			FCFrameFlag=1;
		}
		/*
		//INS帧处理代码
		if(QueueRead(&UART0Queue,0)==INSFrameS0&&QueueRead(&UART0Queue,1)==INSFrameS1&&UART0Queue.Count>=INSFrameLength)
		{
			INSFrameReceive++;
			if(INSFrameFlag==0)
			{
				uint8 i;
				//INSFrameFlag=1;
				//QueueFetch(&UART0Queue);
				//QueueFetch(&UART0Queue);
				for(i=0;i<INSFrameLength;i++)
				{
					INSData[i]=QueueFetch(&UART0Queue);
				}
				INSFrameFlag=1;
			}
			else
			{
				INSFrameLost++;
			}
		}
		*/
		//识别0xA5 0x5A 的实例代码
		/*
		if(F==0)
		{
			if(QueueRead(&UART0Queue,0)==0xA5&&QueueRead(&UART0Queue,1)==0x5A&&UART0Queue.Count==5)
			{
				UARTSendChar(0, 0xFF);
				QueueFetch(&UART0Queue);
				QueueFetch(&UART0Queue);
				C[0]=QueueFetch(&UART0Queue);
				C[1]=QueueFetch(&UART0Queue);
				C[2]=QueueFetch(&UART0Queue);
				F=1;
			}
		}
		*/
  	}
 	VICVectAddr = 0;
}

//UART1中断响应函数
void __irq UART1Handler (void) 
{
	uint8 c;
  	while((U1LSR&0x01)==1)//循环读取
  	{
  		c=U1RBR;
		//UARTSendChar(1, c);
		QueueInsert(&UART1Queue,c);
		//TODO:添加协议头检测代码及处理
		/*
		//INS帧处理代码
		if(QueueRead(&UART1Queue,0)==INSFrameS0&&QueueRead(&UART1Queue,1)==INSFrameS1&&UART1Queue.Count>=INSFrameLength)
		{
			INSFrameReceive++;
			if(INSFrameFlag==0)
			{
				uint8 i;
				//INSFrameFlag=1;
				//QueueFetch(&UART1Queue);
				//QueueFetch(&UART1Queue);
				for(i=0;i<INSFrameLength;i++)
				{
					INSData[i]=QueueFetch(&UART1Queue);
				}
				INSFrameFlag=1;
			}
			else
			{
				INSFrameLost++;
			}
		}
		*/
  	}
 	VICVectAddr = 0;
}

//UART2中断响应函数
void __irq UART2Handler (void) 
{
	uint8 c;
  	while((U2LSR&0x01)==1)//循环读取
  	{
  		c=U2RBR;
  		//UARTSendChar(0, c);//TODO:CHANGE
		QueueInsert(&UART2Queue,c);
		while(UART2Queue.Count>0)
		{
			if(QueueRead(&UART2Queue,0)!=INSFrameS0)
			{
				QueueFetch(&UART2Queue);
			}
			else
			{
				break;
			}
		}
		//TODO:添加协议头检测代码及处理
		//INS帧处理代码
		if(UART2Queue.Count>=INSFrameLength&&QueueRead(&UART2Queue,0)==INSFrameS0&&QueueRead(&UART2Queue,1)==INSFrameS1)
		{
			uint8 i;
			INSFrameReceive++;
			if(INSFrameFlag==1)
			{
				INSFrameLost++;	
			}
			for(i=0;i<INSFrameLength;i++)
			{
				INSData[i]=QueueFetch(&UART2Queue);
			}
			INSFrameFlag=1;
		}
  	}
 	VICVectAddr = 0;
}

//UART3中断响应函数
void __irq UART3Handler (void) 
{
	uint8 c;
  	while((U3LSR&0x01)==1)//循环读取
  	{
  		c=U3RBR;
  		UARTSendChar(3, c);
		QueueInsert(&UART3Queue,c);
		//TODO:添加协议头检测代码及处理
  	}
 	VICVectAddr = 0;
}

uint32 UARTInit(uint32 PortNum, uint32 Baudrate)
{
	uint32 Fdiv;
	if(PortNum==0)
	{
		PINSEL0 |= 0x00000050;//RxD0 and TxD0//TODO=DEL
		U0LCR = 0x83;//8 bits, no Parity, 1 Stop bit
		Fdiv = (Fpclk / 16)/Baudrate;//baud rate
		U0DLM = Fdiv / 256;							
		U0DLL = Fdiv % 256;
		U0LCR = 0x03;//DLAB = 0
		U0FCR = 0x06;//FIFO清零
		U0FCR = 0xC1;//设置触发点并使能

		QueueInit(&UART0Queue,UART0QueueSize);
		QueueInit(&UART0SendQueue,UART0QueueSize);
		
		if (InstallIRQ(UART0_INT,(void *)UART0Handler,HIGHEST_PRIORITY) == FALSE)//CHANGE=HIGHEST_PRIORITY
		{
			return (FALSE);
		}
		
		U0IER=IER_RBR|IER_THRE;
		return TRUE;
	}
	if(PortNum==1)
	{
		//PINSEL0 |= 0x40000000;//Enable TxD1 P0.15//TODO=DEL
		//PINSEL1 |= 0x00000001;//Enable RxD1 P0.16//TODO=DEL
		PINSEL4 |= 0x0000000A;
		U1LCR = 0x83;//8 bits, no Parity, 1 Stop bit
		Fdiv = ( Fpclk / 16 ) / Baudrate;//baud rate
		U1DLM = Fdiv / 256;							
		U1DLL = Fdiv % 256;
		U1LCR = 0x03;//DLAB = 0
		U1FCR = 0x06;//FIFO清零
		U1FCR = 0xC1;//设置触发点并使能
		
		QueueInit(&UART1Queue,UART1QueueSize);
		 
		if (InstallIRQ(UART1_INT,(void *)UART1Handler,HIGHEST_PRIORITY) == FALSE)//CHANGE=HIGHEST_PRIORITY
		{
			return (FALSE);
		}
		
		U1IER=IER_RBR;
		return TRUE;
	}
	if(PortNum==2)
	{
		PCONP|=(1<<24);
		PINSEL0 |= 0x500000;//RxD2 and TxD2//TODO=DEL
		U2LCR = 0x83;//8 bits, no Parity, 1 Stop bit
		Fdiv = (Fpclk / 16)/Baudrate;//baud rate
		U2DLM = Fdiv / 256;							
		U2DLL = Fdiv % 256;
		U2LCR = 0x03;//DLAB = 0
		U2FCR = 0x06;//FIFO清零
		U2FCR = 0xC1;//设置触发点并使能

		QueueInit(&UART2Queue,UART2QueueSize);
		
		if (InstallIRQ(UART2_INT,(void *)UART2Handler,HIGHEST_PRIORITY) == FALSE)//CHANGE=HIGHEST_PRIORITY
		{
			return (FALSE);
		}
		
		U2IER=IER_RBR;
		return TRUE;
	}
	if(PortNum==3)
	{
		PCONP|=(1<<25);
		PINSEL0 |= 0x140000;//RxD3 and TxD3//TODO=DEL
		U3LCR = 0x83;//8 bits, no Parity, 1 Stop bit
		Fdiv = (Fpclk / 16)/Baudrate;//baud rate
		U3DLM = Fdiv / 256;							
		U3DLL = Fdiv % 256;
		U3LCR = 0x03;//DLAB = 0
		U3FCR = 0x06;//FIFO清零
		U3FCR = 0xC1;//设置触发点并使能

		QueueInit(&UART3Queue,UART3QueueSize);
		
		if (InstallIRQ(UART3_INT,(void *)UART3Handler,HIGHEST_PRIORITY) == FALSE)//CHANGE=HIGHEST_PRIORITY
		{
			return (FALSE);
		}
		
		U3IER=IER_RBR;
		return TRUE;
	}
	return FALSE;
}

//循环查询方式发送多个字符
void UARTSend(uint32 PortNum,uint8 *BufferPtr,uint32 Length)
{
	if (PortNum == 0)
	{
		while (Length != 0)//循环发送
		{
			U0THR = *BufferPtr;
			while((U0LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	else if(PortNum == 1)
	{
		while (Length != 0)//循环发送
		{
			U1THR = *BufferPtr;
			while((U1LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	else if(PortNum == 2)
	{
		while (Length != 0)//循环发送
		{
			U2THR = *BufferPtr;
			while((U2LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	else if(PortNum == 3)
	{
		while (Length != 0)//循环发送
		{
			U3THR = *BufferPtr;
			while((U3LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	return;
}

//查询方式发送单个字符
void UARTSendChar(uint32 PortNum,uint8 C)
{
	if (PortNum == 0)
	{
		U0THR = C;
		while((U0LSR&0x40)==0);
	}
	else if (PortNum == 1)
	{
		U1THR = C;
		while((U1LSR&0x40)==0);
	}
	else if (PortNum == 2)
	{
		U2THR = C;
		while((U2LSR&0x40)==0);
	}
	else if (PortNum == 3)
	{
		U3THR = C;
		while((U3LSR&0x40)==0);
	}
}

//以发送中断方式发送数据,过快发送数据可能造成数据错误!(计算理论值为12KB/S,115200bps=14KB/S)
//现只对UART0提供Buffer
void UARTBufferSend(uint32 PortNum,uint8 *BufferPtr,uint32 Length)
{
	if(PortNum==0)
	{
		uint8 i=0;
		for(i=0;i<Length;i++)
		{
			QueueInsert(&UART0SendQueue, *(BufferPtr+i));
		}
		if(UART0SendQueue.Length>0)
		{
			U0THR = QueueFetch(&UART0SendQueue);
		}
	}
	else
	{
		UARTSend(PortNum,BufferPtr,Length);
	}
}

