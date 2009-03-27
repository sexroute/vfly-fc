/****************************************Copyright (c)**************************************************
**                               Guangzou ZLG-MCU Development Co.,LTD.
**                                      graduate school
**                                 http://www.zlgmcu.com
**
**--------------File Info-------------------------------------------------------------------------------
** File name:			target.c
** Last modified Date:	2004-09-17
** Last Version:		1.0
** Descriptions:		header file of the specific codes for LPC2100 target boards
**						Every project should include a copy of this file, user may modify it as needed
**------------------------------------------------------------------------------------------------------
** Created by:			Chenmingji
** Created date:		2004-02-02
** Version:				1.0
** Descriptions:		The original version
**
**------------------------------------------------------------------------------------------------------
** Modified by:			Chenmingji
** Modified date:		2004-09-17
** Version:				1.01
** Descriptions:		Renewed the template, added more compiler supports 
**
**------------------------------------------------------------------------------------------------------
**Modified by:			LinEnqiang
** Modified date:		2007/05/26	
** Version:				1.02
** Descriptions: 		Modified for NXP23XX
**
********************************************************************************************************/


#define IN_TARGET
#include "config.h"

/*********************************************************************************************************
** Function name:			FIQ_Exception
**
** Descriptions:			Fast interrupt exceptional handler , change it as needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void FIQ_Exception(void)
{
    while(1);                   // change it to your code  这一句替换为自己的代码
}

/*********************************************************************************************************
** Function name:			TargetInit
**
** Descriptions:			Initialize the target board; it is called in a necessary place, change it as 
**							needed
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TargetInit(void)
{
    /* 添加自己的代码 */
    /* Add your codes here */
}

/*********************************************************************************************************
** Function name:			TargetResetInit
**
** Descriptions:			Initialize the target 
**
** input parameters:		None
** Returned value:			None
**         
** Used global variables:	None
** Calling modules:			None
**
** Created by:				Chenmingji
** Created Date:			2004/02/02
**-------------------------------------------------------------------------------------------------------
** Modified by:				LinEnqiang
** Modified date:			2007/05/26
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void TargetResetInit(void)
{
	uint32 i;
#ifdef __DEBUG_RAM    
    MEMMAP = 0x2;                  	 	// remap
#endif

#ifdef __DEBUG_FLASH    
    MEMMAP = 0x1;                   	// remap
#endif

#ifdef __IN_CHIP    
    MEMMAP = 0x1;                   	// remap
#endif 		
	if ( PLLSTAT & (1 << 25) )
    {
		PLLCON = 1;						// Enable PLL, disconnected 
		PLLFEED = 0xAA;
		PLLFEED = 0x55;
    }

    PLLCON = 0;							//Disable PLL, disconnected 
    PLLFEED = 0xAA;
    PLLFEED = 0x55;
    while(PLLSTAT & (3 << 24));	
	
    SCS = (SCS & 0x04)|0x20;             // Enable the main OSC,,1MHz～20MHz       							      
	while((SCS & 0x40) == 0 );			 // Wait until main OSC is usable 
	
	CLKSRCSEL = 0x01;					 // select main OSC as the PLL clock source 	
										 
    PLLCFG  = (((PLL_NValue) << 16)|(PLL_MValue));    
      
	PLLCON = 1;
    PLLFEED = 0xAA;							// Enable but disconnect the PLL
    PLLFEED = 0x55;	
    while (((PLLSTAT & (1 << 24)) == 0));	// Wait until the PLL is usable
	
	CCLKCFG = CCLKDivValue;	
	   	
	#if USE_USB 			
 		USBCLKCFG = USBCLKDivValue;			// usbclk = 288 MHz/6 = 48 MHz 
 		PCONP |= 0x80000000;				// Turn On USB PCLK
	#else 	  		
		USBCLKCFG = USBCLKDivValue;	
		PCONP &= 0x7FFFFFFF;    	
	#endif
	for(i=0;i<10000;i++);
	while ( ((PLLSTAT & (1 << 26)) == 0) );	// Check lock bit status 
	while (((PLLSTAT & 0x00007FFF) != PLL_MValue) && (((PLLSTAT & 0x00FF0000) >> 16) != PLL_NValue));
	
 	PLLCON  = 3;								// connect the PLL 
    PLLFEED = 0xAA;
    PLLFEED = 0x55;	    						
	while (((PLLSTAT & (1 << 25))!= (1 << 25)));// Wait until the PLL is connected and locked
	
// Set system timers for each component 
#if (Fpclk / (Fcclk / 4)) == 1

    PCLKSEL0 = 0x00000000;				// PCLK is 1/4 CCLK 
    PCLKSEL1 = 0x00000000;

#else

    #if (Fpclk / (Fcclk / 4)) == 2

    PCLKSEL0 = 0xAAAAAAAA;				// PCLK is 1/2 CCLK 
    PCLKSEL1 = 0xAAAAAAAA;	 

    #else

    PCLKSEL0 = 0x55555555;				// PCLK is the same as CCLK 
    PCLKSEL1 = 0x55555555;	
    
    #endif 
#endif

/* Set memory accelerater module*/     
   MAMCR = 0;
#if Fcclk < 20000000
    MAMTIM = 1;
#else
    #if Fcclk < 40000000
    	MAMTIM = 2;
    #else
   		MAMTIM = 3;
    #endif
#endif
    MAMCR = 2;      	
    /* Add your codes here */
	InitVIC();
    return;
}
/*********************************************************************************************************
**                  以下为一些与系统相关的库函数的实现
**                  具体作用请ads的参考编译器与库函数手册
**                  用户可以根据自己的要求修改        
********************************************************************************************************/
/*********************************************************************************************************
**                  The implementations for some library functions
**                  For more details, please refer to the ADS compiler handbook and The library 
**					function manual
**                  User could change it as needed       
********************************************************************************************************/

#include "rt_sys.h"
#include "stdio.h"
#pragma import(__use_no_semihosting_swi)
#pragma import(__use_two_region_memory)

void _ttywrch(int ch)
{
    ch = ch;
}

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
