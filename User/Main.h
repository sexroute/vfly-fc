#ifndef __MAIN_H
#define __MAIN_H

#define FCUARTPORT 0
#define FCUARTBPS 115200
#define INSUARTPORT 2
#define INSUARTBPS 115200

#define FlashQueueSize 200

#define MainLoopHz 40

//RunState分配
//初始化0x0?
#define OKCPU 0x01
#define OKGPIO 0x02
#define OKUART0 0x03
#define OKUART2 0x04
#define OKFPGA 0x05
#define OKFLASH 0x06
#define OKTimer0 0x07
#define OKTimer1 0x08
#define OKTimer2 0x09
#define OKSetting 0x0A
#define OKGPS 0x0B
//初始化错误0xE?
#define ErrCPU 0xE1
#define ErrGPIO 0xE2
#define ErrUART0 0xE3
#define ErrUART2 0xE4
#define ErrFPGA 0xE5
#define ErrFLASH 0xE6
#define ErrTimer0 0xE7
#define ErrTimer1 0xE8
#define ErrTimer2 0xE9
#define ErrSetting 0xEA
#define ErrGPS 0xEB
//运行模式0x1?
#define MODEManual 0x10
#define MODEMix  0x11
#define MODETarget0 0x12
#define MODETargetP 0x13
#define MODETargetV 0x14
#define MODETargetX 0x15
#define MODEFactory 0x16
//#define MODEINS 0x15

//运行指令
#define CollectMedianData 0x20

#define DefaultRunState MODEManual

#define DoNothing 0
/*
#define DoINS 1
#define DoFCA 2
#define DoPWM 3
#define DoGCS 4
#define DoReturn1 5
#define DoReturn2 6
*/
#define DoWorkLoop 10
#define DoFactoryLoop 11

#define SwitchMODEManual	GpioClrLOW(P1_25)
#define SwitchMODEAuto	GpioSetLOW(P1_25)

#endif
