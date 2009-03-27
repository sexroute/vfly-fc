#ifndef __PARA_H
#define __PARA_H

//==================================================
//FCA2.c
//FCA�㷨�����
extern fp32 FCAOutData[16][3];//CHANGE:��FCAЭ��0x50~5F����ͬ��
/*
00 λ�ÿ��������	3	ud, vd, wd
01 ������������	1	rd

02 �㷨����1	2	A1s; B1s; At��
03 �㷨����2	3	Am��Ay

04 ң��������1	3	A1s��B1s��At
05 ң��������2	2	Am��Ay

06 ң��������3	2	ת�ٿ���������������������

07 ����ϵλ��	3	ֱ�Ӳ�����ת��Ϊƽ������
08 ����ϵ�ٶ�	3	ֱ�Ӳ���
09 ����ϵ��̬	3	ֱ�Ӳ���
10 ����ϵ������	3	ֱ�Ӳ���
11 ����ϵλ��1	3	��Ҫת����У������ǣ�
12 ����ϵ�ٶ�1	3	��Ҫת��1��У������ǣ�2��У����̬�ǣ�
		
13 ָ��Ŀ��λ��	3	�㷨�ο���
14 ָ��Ŀ���ٶ�	3	�㷨�ο���
15 ָ��Ŀ����̬		�㷨�ο���
*/

//FCA��ͣ����
extern fp32 FCAMedianData[6][3];//CHANGE:��FCAЭ��0x60~6F����ͬ��
/*
00 ��ͣλ��		3	x��y��z
01 ��ͣ��̬		3	�ȣ��գ���
02 ��ͣ�ٶ�		3	u, v, w
03 ��ͣ���ٶ�	3	p, q, r
04 ��ͣ����1	3	A1s��B1s��At
05 ��ͣ����2	2	Am Ay

*/

//FCA�㷨Ŀ����
extern fp32 FCATarData[5][3];//CHANGE:��FCAЭ��0x70~7F����ͬ��
/*
00 λ��	3	����ϵ���λ��
01 �ٶ�	3	����ϵ�ٶȣ���������ǣ�
02 ��̬	3	����ϵ��̬
*/

//FCAϵ����
extern fp32 FCAKData[14][3];//CHANGE:��FCAЭ��0x80~0x8Fͬ��
/*
00 ˮƽλ��ͨ��x1	3	��������΢�֣������֣� kpx, kix
01 ˮƽλ��ͨ��x2	3	����ֱ��������˥�����ٶ���ֵxmax, kxmax, umax
02 ˮƽλ��ͨ��y1	3	��������΢�֣������֣�kpy, kiy
03 ˮƽλ��ͨ��y2	3	����ֱ��������˥�����ٶ���ֵymax, kymax, vmax
04 ˮƽ�ٶ���̬x1	3	����2����΢��1��kpu, kpp; kdp
05 ˮƽ�ٶ���̬x2	3	������1��������ϲ���2����kiu, kou, kop
06 ˮƽ�ٶ���̬y1	3	����2����΢��1��kpv, kpr, kdr
07 ˮƽ�ٶ���̬y2	3	������1��������ϲ���2����kiv, kov, kor

08 ��ֱ����ͨ��1	2	���������֣�kpay, kiay
09 ��ֱ����ͨ��2	3	����ֱ��������˥�����ٶ���ֵaymax, kaymax, armax
10 ��ֱ��������3	2	������΢��kpar, kdar
11 ��ֱ�߶�ͨ��1	2	���������֣�kph, kih
12 ��ֱ�߶�ͨ��2	3	����ֱ��������˥�����ٶ���ֵhmax, khmax, wmax
13 ��ֱ�߶�ͨ��3	2	������΢��kpw, kdw
*/

//==================================================
//FCA.c
//FCA����֡
extern fp32 FCAData[19][3];//CHANGE:��FCAЭ�����ͬ��
/*
00:��̬��λ*3 
01:�ٶ���λ*3 
02:���ٶ���λ*3 
03:��������λ*3 
04:�㷨������*3 
05:ң�ؿ�����*3 
06:���������*3 
07:����ϵ��K1-3 
08:����ϵ��K4-6 
09:����ϵ��K7-9 
10:����ϵ��K10-12 
11:��ر���*3 
12:Ŀ��λ��X+Ŀ��λ��Y 

13:�ٶ�*3 (����ϵ)*
14:���ٶ�*3 int16 AngVeloBodyX;//0.01��/s int16 AngVeloBodyY;//0.01��/s int16 AngVeloBodyZ;//0.01��/s
15:�Ƕ�*3 int16 Pitch;//0.1��int16 Roll;//0.1��int16 Yaw;//0.1��
16:λ��*3 int32 Lo;// 1e-7 degree int32 La;// 1e-7 degree int32 Height;// millmeter
17:�Ƕ�����*3 *
18:���ٶ�����*3 *
*/

//==================================================
//FCFrame.c
//FCͳ������
extern uint32 FCBlockError;
//�������֡ͷ
extern FCFrameStruct FCFrameObj;
//On BlockNum=0
//�ռ���λ���ݵı�־λ
extern uint8 CollectMedianDataState;
//For F
//�ط���־λ,0-255�������256��Э,0��ʹ��.
extern uint8 DataSendFlag[256];//0-2λ�ֱ�����λط��������ط������ݼ�¼
//For 1
//��Ҫ���ص����ݳ���
extern uint32 DataToDownload;

//==================================================
//INSFrame.c
//INSͳ������
extern uint32 INSFrameError;
//INS֡����
extern INSFrameStruct INSFrameObj;

//==================================================
//Main.c
extern uint8 RunState;//����״̬
extern uint8 Do;//ִ�в���
extern uint8 DataSendDF;//���ݷ��ͷ�Ƶ
extern uint8 DataRecDF;//���ݼ�¼��Ƶ
extern uint8 ErrorNum;//�������
extern uint8 LastErrorNo;//���һ�������Ĵ�����
extern uint8 LastErrorPara[3];//���һ�������Ĵ������
extern uint32 MainLoopRunCounter;//��ѭ��������
extern uint8 PreAutoMode;//Ԥ�����Լ�ģʽ


//==================================================
//PWM.c
//����PWM�ߵ�ƽʱ��
extern float PWMIn[9];//-1~-1
//���PWM�ߵ�ƽʱ��
extern float PWMOut[9];//-1~1

#endif