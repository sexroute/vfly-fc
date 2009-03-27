/*
	Drv.h
	LCP2368/78 驱动库通用头文件(只需包含此文件不必包含单个驱动文件)
	驱动中所有的序列都从0记起,除特殊注明均使用ISO标准单位.
	参数名称的一般性含义
	No代表序列号
	Ch代表通道号
	Buff/Buffer缓存
	BuffNum缓存长度
	Code代码(数据/命令)与具体芯片相关
	QueueSize列队缓冲长度
	Data数据(待发送/待处理,一般为数组指针)
	Len长度(一般指缓冲区数据的/传入的数组指针中包含的数据的)
	Offset偏移量以字节为单位
	BUAA 2008-7-14
	张以成
*/

#ifndef __DRV_H__
#define __DRV_H__

#include "UART.h"
#include "LPC2300PinCfg.h"
#include "Gpio.h"
#include "Timer.h"
#include "SPI.h"
#include "SSP.h"
#include "SSP0.h"
#include "Flash.h"
#include "FPGA.h"
#include "AD.h"
#include "DA.h"
#include "CAN.h"

#endif
