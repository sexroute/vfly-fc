#ifndef __DA_H
#define __DA_H

#define DAFSDOWN	SelectY1//����ͨѶ
#define DALDDOWN	SelectY2//����ת��
#define DAUP			SelectY0//ȡ��ѡ��

#define DAVref 2.048//�ο���ѹ

uint8 DASetCode(uint8 Ch,uint16 Code);
uint8 DASetVoltage(uint8 Ch,fp32 Voltage);
//void DAOutEnable(void);

#endif
