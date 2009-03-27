/*
	UART.c
	UART��������
	�ṩ��ʼ������ѯ��ʽ���͡��жϷ�ʽ���ա��жӷ�ʽЭ�������
	BUAA 2008-7-11
	���Գ�
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

//����������
volatile uint8 C[3],F;//TODO=DEL
//INSģ������
volatile uint8 INSData[INSFrameLength]={0};
volatile uint8 INSFrameFlag=0;
uint32 INSFrameReceive=0,INSFrameLost=0;

//FCģ������
volatile uint8 FCData[6+15*15]={0};
volatile uint8 FCFrameFlag=0;
uint32 FCFrameReceive=0,FCFrameLost=0;

//UART0�ж���Ӧ����
void __irq UART0Handler (void) 
{
	uint8 c;
	//�����жϴ���
	c=U0LSR&0x20;	//WARN:���������жϴ��󣬱���������Bug��
	if(c==0x20)
	{
		uint8 i=0;
		for(i=0;(i<16)&&UART0SendQueue.Count>0;i++)
		{
			U0THR=QueueFetch(&UART0SendQueue);
		}
	}
	c=U0IIR;//����ж�
  	while((U0LSR&0x01)==1)//ѭ����ȡ
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
		//FC֡�������
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
		//INS֡�������
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
		//ʶ��0xA5 0x5A ��ʵ������
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

//UART1�ж���Ӧ����
void __irq UART1Handler (void) 
{
	uint8 c;
  	while((U1LSR&0x01)==1)//ѭ����ȡ
  	{
  		c=U1RBR;
		//UARTSendChar(1, c);
		QueueInsert(&UART1Queue,c);
		//TODO:���Э��ͷ�����뼰����
		/*
		//INS֡�������
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

//UART2�ж���Ӧ����
void __irq UART2Handler (void) 
{
	uint8 c;
  	while((U2LSR&0x01)==1)//ѭ����ȡ
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
		//TODO:���Э��ͷ�����뼰����
		//INS֡�������
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

//UART3�ж���Ӧ����
void __irq UART3Handler (void) 
{
	uint8 c;
  	while((U3LSR&0x01)==1)//ѭ����ȡ
  	{
  		c=U3RBR;
  		UARTSendChar(3, c);
		QueueInsert(&UART3Queue,c);
		//TODO:���Э��ͷ�����뼰����
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
		U0FCR = 0x06;//FIFO����
		U0FCR = 0xC1;//���ô����㲢ʹ��

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
		U1FCR = 0x06;//FIFO����
		U1FCR = 0xC1;//���ô����㲢ʹ��
		
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
		U2FCR = 0x06;//FIFO����
		U2FCR = 0xC1;//���ô����㲢ʹ��

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
		U3FCR = 0x06;//FIFO����
		U3FCR = 0xC1;//���ô����㲢ʹ��

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

//ѭ����ѯ��ʽ���Ͷ���ַ�
void UARTSend(uint32 PortNum,uint8 *BufferPtr,uint32 Length)
{
	if (PortNum == 0)
	{
		while (Length != 0)//ѭ������
		{
			U0THR = *BufferPtr;
			while((U0LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	else if(PortNum == 1)
	{
		while (Length != 0)//ѭ������
		{
			U1THR = *BufferPtr;
			while((U1LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	else if(PortNum == 2)
	{
		while (Length != 0)//ѭ������
		{
			U2THR = *BufferPtr;
			while((U2LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	else if(PortNum == 3)
	{
		while (Length != 0)//ѭ������
		{
			U3THR = *BufferPtr;
			while((U3LSR&0x40)==0);
			BufferPtr++;
			Length--;
		}
	}
	return;
}

//��ѯ��ʽ���͵����ַ�
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

//�Է����жϷ�ʽ��������,���췢�����ݿ���������ݴ���!(��������ֵΪ12KB/S,115200bps=14KB/S)
//��ֻ��UART0�ṩBuffer
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

