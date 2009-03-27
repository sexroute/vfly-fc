#ifndef __AD_H
#define __AD_H

#define AD0_DOWN	SelectY3
#define AD1_DOWN	SelectY4
#define AD_UP		SelectY0

#define ADVref 2048//参考电压

//AD通道宏定义
//因为AD芯片序列及PCB走线的原因，通道号并不是顺序的
//因此要使用此宏来调用函数(以PCB序号为标准)
#define AEX00 1
#define AEX01 4
#define AEX02 0
#define AEX03 5
#define AEX04 3
#define AEX05 6
#define AEX06 2
#define AEX07 7
#define AEX08 1
#define AEX09 4
#define AEX10 0
#define AEX11 5
#define AEX12 3
#define AEX13 6
#define AEX14 2
#define AEX15 7

int16 ADReadCode(uint8 No,uint8 Ch);
int16 ADReadVoltage(uint8 No,uint8 Ch);

#endif

