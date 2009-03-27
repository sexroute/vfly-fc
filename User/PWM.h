#ifndef __PWM_H
#define __PWM_H

#define PWMZero 0x0147//=0.5ms//0x028F

#define PitchChannel 1	//����	CH2	FPGA2
#define RollChannel 0		//��ת	CH1	FPGA1
#define DirectChannel 7	//����	CH4	FPGA8
#define DistanceChannel 3	//�ܾ�	CH6	FPGA4

#define ThrottleChannel 4	//����	CH3	FPGA5
#define SwitchChannel 6	//�л�	CH9	FPGA7

#define GyroChannel 2	//����	CH5	FPGA3
#define RPMChannel 5		//ת��	CH8	FPGA6
#define ReserveChannel 8	//����	CH7	FPGA9

void GetPWM(void);
void SetPWM(void);

#endif
