#ifndef __SSP_H
#define  __SSP_H

//Mode…Ë÷√Œª
#define CPOL (1<<6)
#define CPHA (1<<7)

void  SSPInit(uint8 PortNum,uint32 Fdiv,uint32 Mode);
uint8  SSPSendByte(uint8 PortNum,uint8 data);
void SSPSend(uint8 PortNum,uint8* Data,uint32 Len);
void SSPSend(uint8 PortNum,uint8* Data,uint32 Len);

#endif

