#ifndef __FLASH_H
#define __FLASH_H

#define FLASHUP			GpioSetHI(P2_04);GpioSetHI(P2_03);GpioSetHI(P2_02) //ȡ��FlashоƬѡ��
#define FLASH0DOWN		GpioClrHI(P2_04);GpioClrHI(P2_03);GpioClrHI(P2_02) //ѡͨFlash0
#define FLASH1DOWN		GpioClrHI(P2_04);GpioClrHI(P2_03);GpioSetHI(P2_02) //ѡͨFlash1

#define OVERTIME		50000//������ʱʱ��(��λms)

//�ͼ�Flash��������
uint8 FlashChipSelect(uint8 No);
uint8 ReadStatus(void);
uint8 FlashInit(uint16 QueueSize);
uint8 BufferWrite(uint16 Offset,uint8 *Data, uint16 Len);
uint8 PageWrite(uint16 PageNo);
uint8 PageRead(uint16 PageNo);
uint8 BufferRead(uint16 Offset,uint8 *Data, uint16 Len);
uint8 BlockErase(uint16 BlockNo);
uint8 PageErase(uint16 PageNo);
uint8 PageModeSwitch(void);

//�߼�Flash��������
uint8 ChangeWritePageNo(uint16 PageNo);
uint16 GetWritePageNo(void);
int8 StreamWrite(uint8* Data,uint8 Len);
uint8 StreamWriteForce(void);
uint8 ChangeReadSet(uint16 PageNo,uint16 Offset);
uint8 ChangeReadAddress(uint32 Address);
uint16 GetReadPageNo(void);
uint16 GetReadOffset(void);
uint8 StreamRead(uint8* Data,uint8 Len);

#endif

