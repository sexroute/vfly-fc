/*
	Flash.c
	Flash(AT45DB642D)��������
	�ṩ�ͼ�ҳģʽ�͸߼���ģʽ���ֶ�д��ʽ��������Ѱַ�ȹ��ܡ�
	����ʹ����ģʽ��������ṩ���õ����ܵĶ�д���塣
	��ȡʹ��Buffer1��д��ʹ��Buffer2��оƬ���״�ʹ��ǰ�����Ϊ1024byte/pageģʽ��
	оƬ�ĸ��ֲ�����������ο�Flash.h��оƬ�ֲ�
	BUAA 2008-7-17
	���ƌ����Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "Queue.h"
#include "AT45DB642D.h"
#include "Flash.h"

uint8 FlashSelect;//���浱ǰ���ݵ�FlashоƬID,���鲻Ҫֱ�Ӹ��Ķ���ʹ��FlashChipSelect()

//��д״̬����
uint16 WriteOffset,ReadOffset,WritePageNo,ReadPageNo;
uint8 IsReaded;
//���ݻ����ж�
uint16 StreamQueueSize;
QueueObj  StreamQueue;

//ѡ��Ŀ��FlashоƬ(��Ҫ�ڶ�дδ���ʱ����оƬѡ�񣬲��ڶ�д����ʱ������ġ�)
uint8 FlashChipSelect(uint8 No)
{
	if(No<=1)
	{
		FlashSelect=No;
		return TRUE;
	}
	return FALSE;
}

//ѡ��Ŀ��оƬ
void FlashDown()
{
	if(FlashSelect==0)
	{
		FLASH0DOWN;
	}
	else
	{
		FLASH1DOWN;
	}
}

//��ȡFlash״̬
uint8 ReadStatus()
{
	uint8 t;
	
	FlashDown();
	t=SSPSendByte(1,0xD7);
	t=SSPSendByte(1,0x00);
	FLASHUP;
	
	return t;
}

//�ȴ�оƬReady(��ʱʱ����ͷ�ļ����趨)
uint8 WaitReady()
{
	uint32 i;
	uint8 t=ReadStatus();
	
	for(i = 0; (i<OVERTIME)&&((t & FLASH_STAT) != FLASH_STAT); i++)
	{
       	DelayMs(3,1);//TODO?
		t = ReadStatus();
	}
	
    	if (i==OVERTIME)
    	{
       	 return FALSE;
    	}
		
	return TRUE;
}

//Flash��ʼ����QueueSizeΪStreamQueue�ĳ��ȡ�
//StreamQueue��д����߼�д�������仺�����ÿ�����FlashоƬBusy������Ƚ��л��塣
//QueueSize������ڵ���д������鳤�ȣ�QueueSize������ʱ�رո߼�д���������벻Ҫ���á�
//Ĭ��ѡ��оƬ0��
uint8 FlashInit(uint16 QueueSize)
{
	SSPInit(1,2,0|CPHA|CPOL);
	
	FlashSelect=0;
	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	FlashSelect=1;
	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	FlashSelect=0;
	WriteOffset=0;
	WritePageNo=0;
	ReadPageNo=0;
	ReadOffset=0;
	IsReaded=FALSE;

	StreamQueueSize=QueueSize;
	if(StreamQueueSize>0)
	{
		QueueInit(&StreamQueue,StreamQueueSize);
	}
	return TRUE;
}

//������д��Buffer
uint8 BufferWrite(uint16 Offset,uint8 *Data, uint16 Len)
{
	uint16 t;
	uint8 temp1,temp2;

	if(Offset + Len > PAGE_SIZE)
	{
		return FALSE;
	}

	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	temp1 = (uint8)(Offset>>8);
	temp2 = (uint8)(Offset);
	
	FlashDown();
	t = SSPSendByte(1,0x87);
	t = SSPSendByte(1,0x00);
    	t = SSPSendByte(1,temp1);
    	t = SSPSendByte(1,temp2);
	//t = SSPSendByte(1,0x00);
       for (t = 0; t < Len; t++)
       {
          	SSPSendByte(1,Data[t]);	
       }
	FLASHUP;
	
	return TRUE;
}

//��ȡBuffer�е�����
uint8 BufferRead(uint16 Offset,uint8 *Data, uint16 Len)
{
	uint16 i;
	uint8 t;
	uint8 temp1,temp2;

	if(Offset + Len > PAGE_SIZE)
	{
		return FALSE;
	}
	
  	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	temp1 = (uint8)(Offset>>8);
	temp2 = (uint8)(Offset);
	
	FlashDown();
  	SSPSendByte(1,0xD4);
	SSPSendByte(1,0x00);
  	SSPSendByte(1,temp1);
  	SSPSendByte(1,temp2);
	SSPSendByte(1,0x00);
	for (i = 0; i < Len; i++)
  	{
    		t = SSPSendByte(1,0x00);
    		Data[i] = t;
  	}
	FLASHUP;
	
  	return TRUE; 
}


//��Buffer�е�����д��Flash
uint8 PageWrite(uint16 PageNo)
{
	uint16 t;
	uint8 temp1,temp2;
	
	if (PageNo >= FLASH_PAGES)
	{
		return FALSE; 
	}
	
	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	temp1 = (uint8)(PageNo>>6);
	temp2 = (uint8)(PageNo<<2);
	
	FlashDown();
	t = SSPSendByte(1,0x86);
	t = SSPSendByte(1,temp1);
	t = SSPSendByte(1,temp2);
	t = SSPSendByte(1,0x00);
	FLASHUP;
	
	return TRUE;
}

//��ȡFlash�е����ݵ�Buffer
uint8 PageRead(uint16 PageNo)
{
	uint8 temp1,temp2;
	//uint8 t;
	
  	if (PageNo >= FLASH_PAGES)
	{
		return FALSE; 
	}
	
	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	temp1 = (uint8)(PageNo>>6);
	temp2 = (uint8)(PageNo<<2);
	
	FlashDown();
  	SSPSendByte(1,0x53);
  	SSPSendByte(1,temp1);
  	SSPSendByte(1,temp2);
	SSPSendByte(1,0x00);
	FLASHUP;
	
  	return TRUE; 
}

//����Block
uint8 BlockErase(uint16 BlockNo)
{
	//uint16 i;
	uint8 temp1,temp2;
	
	if (BlockNo>= FLASH_BLOCKS)
	{
		return FALSE;
	}
	
	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	temp1 = (uint8)((BlockNo>>3)&0xff);
	temp2 = (uint8)((BlockNo<<5)&0xff);
	
	FlashDown();
	SSPSendByte(1,0x50);
	SSPSendByte(1,temp1);
	SSPSendByte(1,temp2);
	SSPSendByte(1,0x00);	
	FLASHUP;
	
	return TRUE;
}

//����Page
uint8 PageErase(uint16 PageNo)
{
	//uint16 i;
	uint8 temp1,temp2;

	if (PageNo >= FLASH_PAGES)
	{
		return FALSE;
	}
	
	if(WaitReady()==FALSE)
	{
		return FALSE;
	}
	
	temp1 = (uint8)((PageNo>>6)&0xff);
	temp2 = (uint8)((PageNo<<2)&0xff);
	
	FlashDown();
	SSPSendByte(1,0x81);
	SSPSendByte(1,temp1);
	SSPSendByte(1,temp2);
	SSPSendByte(1,0x00);
	FLASHUP;
	
	return TRUE;
}

//ת��FlashоƬ��PageSizeģʽΪ1024(�˺���ֻ���ڵ�һ��Ӳ����ʼ��ʱ����)��
uint8 PageModeSwitch()
{
	uint8 t;
	
	SSPInit(1,2,0|CPHA|CPOL);
	FlashDown();
	SSPSendByte(1,0x3D);
	//SSPSendByte(1,0x00);
	SSPSendByte(1,0x2A);
	//SSPSendByte(1,0x00);
	SSPSendByte(1,0x80);
	//SSPSendByte(1,0x00);
	SSPSendByte(1,0xA6);
	//SSPSendByte(1,0x00);
	FLASHUP;
	
	t = ReadStatus();
	UARTSendChar(0,t);
	return(t);
}

//�ı�д����ʼPageλ�á�
//�ı�ʱ�Ὣδд��Flash������һ��д�롣
//Ĭ��PageNoΪ0��
uint8 ChangeWritePageNo(uint16 PageNo)
{
	if(PageNo==WritePageNo)
	{
		return TRUE;
	}
	if(PageNo<FLASH_PAGES)
	{
		while(StreamQueue.Count>0)
		{
			WaitReady();
			StreamWrite(NULL,0);
		}
		if(WriteOffset>0)
		{
			PageWrite(WritePageNo);
			WriteOffset=0;
		}
		WritePageNo=PageNo;
		return TRUE;
	}
	return FALSE;
}

//��ȡ��ǰд���PageNo
uint16 GetWritePageNo()
{
	return WritePageNo;
}

//��������ӵ��ж���
uint8 AddQueue(uint8* Data,uint8 Len)
{
	uint8 r=TRUE;
	int i;
	//��ǰ�ж��Ƿ�ᶪʧ����
	if(Len+StreamQueue.Count>StreamQueueSize)
	{
		r=FALSE;
	}
	for(i=0;i<Len;i++)
	{
		QueueInsert(&StreamQueue, Data[i]);
	}
	return r;
}

//���ж��е�����д��Buffer��
uint8 BufferWriteFromQueue()
{
	uint16 t,n;
	uint8 temp1,temp2;

	temp1 = (uint8)(WriteOffset>>8);
	temp2 = (uint8)(WriteOffset);
	
	FlashDown();
	t = SSPSendByte(1,0x87);
	t = SSPSendByte(1,0x00);
	t = SSPSendByte(1,temp1);
	t = SSPSendByte(1,temp2);
	//t = SSPSendByte(1,0x00);
	n=PAGE_SIZE;
	for (t = 0;((WriteOffset<n)&&(StreamQueue.Count>0)); t++)
	{
		SSPSendByte(1,QueueFetch(&StreamQueue));
		WriteOffset++;
	}
	FLASHUP;
	
	return t;
}

//��ģʽ����д�뺯��
//�˺�������FlashоƬBusyʱ�Զ��������ݣ����´ε��û�ǿ��д��ʱ��д��Flash�С�
//���Ҵ˺������Զ���ԽPageSize�������Զ����죬����д��PageBufferʱ�Զ�д��Flash��
//����д�����ݳ��Ȳ�Ӧ����StreamQueueSize
//����ֵ
//д��ʧ��FALSE:0
//�������ݶ�ʧ-1
//�ɹ�д��TRUE:1
int8 StreamWrite(uint8* Data,uint8 Len)
{
	int8 lost,t,s;
	//���������Դ��´�д����ʱ���Ƚ��ж��е�����д��Flash��
	if(Len<=StreamQueue.Length&&Len+StreamQueue.Count>StreamQueue.Length)
	{
		WaitReady();
		StreamWrite(NULL,0);
	}
	//������д���ж�
	lost=AddQueue(Data,Len);
	s=ReadStatus();
	
	if((s& FLASH_STAT) == FLASH_STAT)//�ж�Flash״̬
	{
		BufferWriteFromQueue();
		//Flash������ʱд��Flash
		if(WriteOffset>=PAGE_SIZE)
		{
			t=PageWrite(WritePageNo);
			WritePageNo++;
			WriteOffset=0;
		}
	}
	if(t==FALSE)
	{
		return FALSE;
	}
	else
	{
		if(lost==FALSE)
		{
			return -1;
		}
		return TRUE;
	}
}

//ǿ��д�뺯��
//�˺������ж���Buffer�е�ʣ������д��Flash��
uint8 StreamWriteForce()
{
	//����ж�
	while(StreamQueue.Count>0)
	{
		WaitReady();
		StreamWrite(NULL,0);
	}
	//��ջ���
	if(WriteOffset>0)
	{
		PageWrite(WritePageNo);
	}
	return TRUE;
}

//�ı��ȡ��ʼ��ȡλ��
uint8 ChangeReadSet(uint16 PageNo,uint16 Offset)
{
	if(PageNo==ReadPageNo)
	{
		ReadOffset=Offset;
		return TRUE;
	}
	if(PageNo<FLASH_PAGES&&Offset<PAGE_SIZE)
	{
		ReadPageNo=PageNo;
		ReadOffset=Offset;
		IsReaded=FALSE;
		return TRUE;
	}
	return FALSE;
}

//�ı��ȡ��ʼ��ȡλ��
//�˺���ΪͳһѰַģʽ
uint8 ChangeReadAddress(uint32 Address)
{
	return ChangeReadSet(Address/PAGE_SIZE,Address%PAGE_SIZE);
}

//��ȡ��ǰ��ȡPageNo
uint16 GetReadPageNo()
{
	return ReadPageNo;
}

//��ȡ��ǰ��ȡOffset
uint16 GetReadOffset()
{
	return ReadOffset;
}

//��ȡ��ǰ��ȡλ��(ͳһѰַģʽ)
uint32 GetReadAddress()
{
	return ReadPageNo*PAGE_SIZE+ReadOffset;
}

//��ģʽ���ݶ�ȡ����
//�˺������ڵ�һ�ζ�ȡĿ��Pageʱ�Զ��������ݣ����ڽ������Ķ�ȡ��ʹ�û������ݡ�
//���Ҵ˺������Զ���ԽPageSize�������Զ����죬���ڶ���PageBufferʱ�Զ���ȡ��һPage��
//����ֵ:�ɹ���ȡ�����ݸ�����
uint8 StreamRead(uint8* Data,uint8 Len)
{
	uint8 Readed=0;
	while(Readed<Len)
	{
		if(IsReaded==FALSE)//�ж��Ƿ��Ѿ���ȡ��Page
		{
			PageRead(ReadPageNo);
			IsReaded=TRUE;
		}
		if(ReadOffset+Len-Readed<=PAGE_SIZE)//�жϴ������ݳ��ȣ�С��PageSzie��ֱ�Ӷ�ȡ�������ȡ��Page��β��
		{
			BufferRead(ReadOffset, Data+Readed, Len-Readed);
			ReadOffset+=Len-Readed;
			Readed+=Len-Readed;
		}
		else
		{
			BufferRead(ReadOffset, Data+Readed, PAGE_SIZE-ReadOffset);
			Readed+=PAGE_SIZE-ReadOffset;
			ReadOffset+=PAGE_SIZE-ReadOffset;
			if(ChangeReadSet(ReadPageNo+1,0)==FALSE)
			{
				break;
			}
		}
	}
	return Readed;
}

