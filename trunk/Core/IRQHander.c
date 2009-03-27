/*
	IRQHander.c
	IRQ管理程序
	提供IRQ初始化(启动时已执行)，链接(安装)。
	使能和禁止IRQ的函数在Target.h中
	BUAA 2008-7-11
	张以成
*/
#include "Config.h"
#include "IRQHander.h"

//初始化IRQ控制器(启动时已执行)
void InitVIC(void) 
{
    uint32 i = 0;
    uint32 *vect_addr, *vect_prio;
   	
    /* initialize VIC*/
    VICIntEnClr = 0xffffffff;
    VICVectAddr = 0;
    VICIntSelect = 0;

    /* set all the vector and vector control register to 0 */
    for ( i = 0; i < VIC_SIZE; i++ )
    {
		vect_addr = (uint32 *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + i*4);
		vect_prio = (uint32 *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + i*4);
		*vect_addr = 0x0;	
		*vect_prio = 0xF;
    }
    return;
}

//安装IRQ
//IntNumber设备中断号(请参考IRQHander.h文件中的定义)
//HandlerAddr中断函数地址
//Priority优先级
uint32 InstallIRQ( uint32 IntNumber, void *HandlerAddr, uint32 Priority )
{
    uint32 *vect_addr;
    uint32 *vect_prio;
      
    VICIntEnClr = 1 << IntNumber;	/* Disable Interrupt */
    if ( IntNumber >= VIC_SIZE )
    {
		return ( FALSE );
    }
    else
    {
		/* find first un-assigned VIC address for the handler */
		vect_addr = (uint32 *)(VIC_BASE_ADDR + VECT_ADDR_INDEX + IntNumber*4);
		vect_prio = (uint32 *)(VIC_BASE_ADDR + VECT_PRIO_INDEX + IntNumber*4);
		*vect_addr = (uint32)HandlerAddr;	/* set interrupt vector */
		*vect_prio = Priority;
		VICIntEnable = 1 << IntNumber;	/* Enable Interrupt */
		return( TRUE );
    }
}

