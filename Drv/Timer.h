#ifndef __TIMER_H 
#define __TIMER_H

/* depending on the CCLK and PCLK setting, e.g. CCLK = 60Mhz, 
PCLK = 1/4 CCLK, then, 10mSec = 150.000-1 counts */
// #define TIME_INTERVAL	149999
	
#define TIME_INTERVAL	(Fpclk/100 - 1)

extern void DelayMs(uint8 TimerNum, uint32 DelayInMs);
extern uint32 TimerInit( uint8 TimerNum, uint32 TimerInterval );
extern void TimerEnable( uint8 TimerNum );
extern void TimerDisable( uint8 TimerNum );
extern void TimerReset( uint8 TimerNum );

#endif
