/*
	Flash.c
	Flash(AT45DB642D)驱动程序。
	提供低级页模式和高级流模式两种读写方式，及擦出寻址等功能。
	建议使用流模式，其可以提供更好的性能的读写缓冲。
	读取使用Buffer1，写入使用Buffer2，芯片在首次使用前需更改为1024byte/page模式。
	芯片的各种参数及长度请参考Flash.h或芯片手册
	BUAA 2008-7-17
	蔡云屾张以成
*/
#include "Config.h"
#include "Drv.h"
#include "Queue.h"
#include "AT45DB642D.h"
#include "Flash.h"

uint8 FlashSelect;//保存当前操纵的Flash芯片ID,建议不要直接更改而是使用FlashChipSelect()

//读写状态参数
uint16 WriteOffset,ReadOffset,WritePageNo,ReadPageNo;
uint8 IsReaded;
//数据缓冲列队
uint16 StreamQueueSize;
QueueObj  StreamQueue;

//选择目标Flash芯片(不要在读写未完成时更改芯片选择，并在读写交替时按需更改。)
uint8 FlashChipSelect(uint8 No)
{
	if(No<=1)
	{
		FlashSelect=No;
		return TRUE;
	}
	return FALSE;
}

//选中目标芯片
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

//读取Flash状态
uint8 ReadStatus()
{
	uint8 t;
	
	FlashDown();
	t=SSPSendByte(1,0xD7);
	t=SSPSendByte(1,0x00);
	FLASHUP;
	
	return t;
}

//等待芯片Ready(超时时间在头文件中设定)
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

//Flash初始化，QueueSize为StreamQueue的长度。
//StreamQueue对写入起高级写入驱动其缓冲作用可以在Flash芯片Busy情况下先进行缓冲。
//QueueSize必须大于单次写入的数组长度，QueueSize等于零时关闭高级写入驱动，请不要调用。
//默认选中芯片0。
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

//将数据写入Buffer
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

//读取Buffer中的数据
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


//将Buffer中的数据写入Flash
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

//读取Flash中的数据到Buffer
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

//擦除Block
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

//擦除Page
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

//转换Flash芯片的PageSize模式为1024(此函数只需在第一次硬件初始化时调用)。
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

//改变写入起始Page位置。
//改变时会将未写入Flash的数据一并写入。
//默认PageNo为0。
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

//获取当前写入的PageNo
uint16 GetWritePageNo()
{
	return WritePageNo;
}

//将数据添加到列队中
uint8 AddQueue(uint8* Data,uint8 Len)
{
	uint8 r=TRUE;
	int i;
	//提前判断是否会丢失数据
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

//将列队中的数据写入Buffer中
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

//流模式数据写入函数
//此函数会在Flash芯片Busy时自动缓冲数据，在下次调用或强制写入时在写入Flash中。
//并且此函数会自动跨越PageSize的限制自动延伸，并在写满PageBuffer时自动写入Flash。
//单次写入数据长度不应大于StreamQueueSize
//返回值
//写入失败FALSE:0
//部分数据丢失-1
//成功写入TRUE:1
int8 StreamWrite(uint8* Data,uint8 Len)
{
	int8 lost,t,s;
	//当缓存无以存下待写数据时，先将列队中的数据写入Flash。
	if(Len<=StreamQueue.Length&&Len+StreamQueue.Count>StreamQueue.Length)
	{
		WaitReady();
		StreamWrite(NULL,0);
	}
	//将数据写入列队
	lost=AddQueue(Data,Len);
	s=ReadStatus();
	
	if((s& FLASH_STAT) == FLASH_STAT)//判断Flash状态
	{
		BufferWriteFromQueue();
		//Flash缓存满时写入Flash
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

//强制写入函数
//此函数将列队与Buffer中的剩余数据写入Flash。
uint8 StreamWriteForce()
{
	//清空列队
	while(StreamQueue.Count>0)
	{
		WaitReady();
		StreamWrite(NULL,0);
	}
	//清空缓存
	if(WriteOffset>0)
	{
		PageWrite(WritePageNo);
	}
	return TRUE;
}

//改变读取起始读取位置
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

//改变读取起始读取位置
//此函数为统一寻址模式
uint8 ChangeReadAddress(uint32 Address)
{
	return ChangeReadSet(Address/PAGE_SIZE,Address%PAGE_SIZE);
}

//获取当前读取PageNo
uint16 GetReadPageNo()
{
	return ReadPageNo;
}

//获取当前读取Offset
uint16 GetReadOffset()
{
	return ReadOffset;
}

//获取当前读取位置(统一寻址模式)
uint32 GetReadAddress()
{
	return ReadPageNo*PAGE_SIZE+ReadOffset;
}

//流模式数据读取函数
//此函数会在第一次读取目标Page时自动缓冲数据，并在接下来的读取中使用缓冲数据。
//并且此函数会自动跨越PageSize的限制自动延伸，并在读满PageBuffer时自动读取下一Page。
//返回值:成功读取的数据个数。
uint8 StreamRead(uint8* Data,uint8 Len)
{
	uint8 Readed=0;
	while(Readed<Len)
	{
		if(IsReaded==FALSE)//判断是否已经读取该Page
		{
			PageRead(ReadPageNo);
			IsReaded=TRUE;
		}
		if(ReadOffset+Len-Readed<=PAGE_SIZE)//判断待读数据长度，小于PageSzie则直接读取，否则读取到Page结尾。
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

