/*
	SSP0.c
	SSP0控制器驱动程序
	提供SSP0控制器所连接的三种设备的通讯模式协调功能。
	BUAA 2008-7-26
	张以成
*/
#include "Config.h"
#include "Drv.h"
#include "74LV138.h"
#include "SSP0.h"

//启动SSP控制器SPI通讯以FPGA模式
uint8 SSP0FPGAMode()
{
	SSPInit(0,192,0|CPHA|CPOL);//TODO:Up Clock
   	SelectY0;
	return TRUE;
}

//启动SSP控制器SPI通讯以AD模式
uint8 SSP0ADMode()
{
	SSPInit(0,192,0);
   	SelectY0;
	return TRUE;
}

//启动SSP控制器SPI通讯以DA模式
uint8 SSP0DAMode()
{
	SSPInit(0,192,0|CPHA);
   	SelectY0;
	return TRUE;
}
