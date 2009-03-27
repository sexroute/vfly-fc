#ifndef __FPGA_H
#define __FPGA_H

//����Ƭѡ
#define FPGA_DOWN	SelectY6
#define FPGA_UP		SelectY0
//����������
#define READTIME	0x11//��ȡ����
#define SETTIME		0x21//��������
#define READSTEP	0x31//��ȡ�ߵ�ƽʱ�� 
#define SETSTEP		0x41//���øߵ�ƽʱ��
#define READFRE		0x51//��ȡ�ź�Ƶ��
#define TESTFPGA	0xf0//���FPGA�Ƿ���

uint8 FPGACheck(void);
uint16 FPGAReadTime(uint8 Num);
uint16 FPGAReadStep(uint8 Num);
uint16 FPGAReadFre(uint8 Num);
uint16 FPGASetTime(uint8 Num,uint16 Data);
uint16 FPGASetStep(uint8 Num,uint16 Data);

#endif

