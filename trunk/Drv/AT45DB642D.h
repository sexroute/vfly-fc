/*
	AT45DB642D.h
	AT45DB642D资源数据宏定义文件
	BUAA 2008-7-18
	蔡云屾
*/
#ifndef __AT45_H
#define __AT45_H

//Status Register Format for AT45DB642D
//7			6		5	4	3	2	1	0
//RDY/BUSY	COMP   	1	1	1	1	X	1(1024)
#define FLASH_STAT		0xBD  //Flash标志位
#define PAGE_SIZE		1024   //页大小
#define FLASH_PAGES		8191  //Flash的总页数
#define FLASH_BLOCKS	1024  //BLOCKS 总数
#define FLASH_BYTE		8387584  //Flash 的总字节

#endif

