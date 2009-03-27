#ifndef __PWM_H
#define __PWM_H

#define PWMZero 0x0147//=0.5ms//0x028F

#define PitchChannel 1	//俯仰	CH2	FPGA2
#define RollChannel 0		//滚转	CH1	FPGA1
#define DirectChannel 7	//航向	CH4	FPGA8
#define DistanceChannel 3	//总距	CH6	FPGA4

#define ThrottleChannel 4	//油门	CH3	FPGA5
#define SwitchChannel 6	//切换	CH9	FPGA7

#define GyroChannel 2	//陀螺	CH5	FPGA3
#define RPMChannel 5		//转速	CH8	FPGA6
#define ReserveChannel 8	//保留	CH7	FPGA9

void GetPWM(void);
void SetPWM(void);

#endif
