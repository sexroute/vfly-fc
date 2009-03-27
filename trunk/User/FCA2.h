#ifndef __FCA2_H
#define __FCA2_H

#define  MtMk 0.0111195
void FCALimitPWM(void);
void FCAGetNav(void);
//void FCACollectPosYaw(uint8 Long);
void FCACollectMedianDataFun(uint8 Long);
void FCAPosXY(void);
void FCAVelXY(void);
void FCAYaw(void);
void FCAHei(void);
void SmoothPWM(uint8 Long);
void FCAPWMOut(void);

#endif
