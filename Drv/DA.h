#ifndef __DA_H
#define __DA_H

#define DAFSDOWN	SelectY1//启动通讯
#define DALDDOWN	SelectY2//启动转换
#define DAUP			SelectY0//取消选择

#define DAVref 2.048//参考电压

uint8 DASetCode(uint8 Ch,uint16 Code);
uint8 DASetVoltage(uint8 Ch,fp32 Voltage);
//void DAOutEnable(void);

#endif
