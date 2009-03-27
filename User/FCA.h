#ifndef __FCA_H
#define __FCA_H

//MtMk 0.185325
//12756200*3.1415926/360*1e-7=0.0111318843122556
//0.185325*60*10000*1e-7=0.0111195
#define  MtMk 0.0111195//经度线上万分之一分到米的换算单位

void FCA(void);
void SmoothPWM(uint8 Long);
void CollectMedianDataFun(uint8 Long);

#endif

