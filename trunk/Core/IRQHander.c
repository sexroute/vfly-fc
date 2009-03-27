/*
	IRQHander.c
	IRQ�������
	�ṩIRQ��ʼ��(����ʱ��ִ��)������(��װ)��
	ʹ�ܺͽ�ֹIRQ�ĺ�����Target.h��
	BUAA 2008-7-11
	���Գ�
*/
#include "Config.h"
#include "IRQHander.h"

//��ʼ��IRQ������(����ʱ��ִ��)
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

//��װIRQ
//IntNumber�豸�жϺ�(��ο�IRQHander.h�ļ��еĶ���)
//HandlerAddr�жϺ�����ַ
//Priority���ȼ�
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

