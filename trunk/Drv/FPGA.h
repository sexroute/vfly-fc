#ifndef __FPGA_H
#define __FPGA_H

//定义片选
#define FPGA_DOWN	SelectY6
#define FPGA_UP		SelectY0
//定义命令字
#define READTIME	0x11//读取周期
#define SETTIME		0x21//设置周期
#define READSTEP	0x31//读取高电平时间 
#define SETSTEP		0x41//设置高电平时间
#define READFRE		0x51//读取信号频率
#define TESTFPGA	0xf0//检测FPGA是否工作

uint8 FPGACheck(void);
uint16 FPGAReadTime(uint8 Num);
uint16 FPGAReadStep(uint8 Num);
uint16 FPGAReadFre(uint8 Num);
uint16 FPGASetTime(uint8 Num,uint16 Data);
uint16 FPGASetStep(uint8 Num,uint16 Data);

#endif

