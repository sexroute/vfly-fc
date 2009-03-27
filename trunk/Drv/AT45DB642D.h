/*
	AT45DB642D.h
	AT45DB642D��Դ���ݺ궨���ļ�
	BUAA 2008-7-18
	���ƌ�
*/
#ifndef __AT45_H
#define __AT45_H

//Status Register Format for AT45DB642D
//7			6		5	4	3	2	1	0
//RDY/BUSY	COMP   	1	1	1	1	X	1(1024)
#define FLASH_STAT		0xBD  //Flash��־λ
#define PAGE_SIZE		1024   //ҳ��С
#define FLASH_PAGES		8191  //Flash����ҳ��
#define FLASH_BLOCKS	1024  //BLOCKS ����
#define FLASH_BYTE		8387584  //Flash �����ֽ�

#endif

