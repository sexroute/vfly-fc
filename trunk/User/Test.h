//测试函数集
#ifndef __TEST_H
#define __TEST_H

#include "74LV138.h"
#include "INSFrame.h"

void UARTTest1()
{
	UARTInit(2, 115200);
	UARTSendChar(2,0xFF);
	UARTSendChar(2,0xFF);
	while(1)
	{
		//PrintUART0("OK");
		//BeepON();
		//Delay();
		//BeepOFF();
		//Delay();
	}
}

void UARTTest2()
{
	UARTInit(0, 115200);
	while(1)
	{
		BeepON();
		PrintUART0("Hellow ");
		Delay();
		BeepOFF();
		PrintUART0("World!");
		Delay();
	}
}

void UARTTest3()
{
	uint8 c[11]="0123456789";
	UARTInit(0, 115200);
	while(1)
	{
		uint8 i=0;
		LED2ON();
		for(i=0;i<10;i++)
		{
			UARTBufferSend(0,c,5);
		}
		LED2OFF();
		DelayMs(3, 1000);
	}
}

void LEDTest()
{
	while(1)
	{
		LED1ON();
		Delay();
		LED1OFF();
		Delay();
		LED2ON();
		Delay();
		LED2OFF();
		Delay();    
	}
}

void testIO()
{
	uint8 temp = 0;
	GpioSpeedHigh();
	FIO0DIR = 0x0;
	
	while(1)
	{
		GpioClrHI(P0_13);
		temp = GpioGetHI(P0_13);
		//BeepON();
		UARTSend(0, (uint8 *) &temp, 1);
		Delay();
		GpioCplHI(P0_13);
		//GpioClrHI(P0_15);
		temp = GpioGetHI(P0_13);
		//BeepOFF();
		UARTSend(0, (uint8 *) &temp, 1);
		Delay();
	}
	/*
	while(1)
	{
		temp = GpioGetLOW(P0_15);
		GpioCplLOW(P0_15);
		temp = GpioGetLOW(P0_15);
	}
	*/
}

void TimerTest()
{
	//TimerDisable(1);
	//TimerDisable(2);
	//TimerInit(2,2000);
	//TimerInit(3,1000);
	//TimerEnable(2);
	//TimerEnable(3);
	
	//TimerReset(2);
	//TimerEnable(1);
	//while(1);
	//delayMs(2,10000);
	//TimerDisable(0);
	//TimerDisable(2);
	while(1)
	{
		LED1ON();
		DelayMs(3,1000);
		LED1OFF();
		DelayMs(3,1000);
		LED2ON();
		DelayMs(2,2000);
		LED2OFF();
		DelayMs(2,2000);
	}
}

const uint32 LED_TBL[] = 
{
	0x00, 0xFF,									// 全部熄灭后，再全部点亮
	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80,	// 依次逐个点亮
	0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF,	// 依次逐个叠加
	0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01,	// 依次逐个递减
	0x81, 0x42, 0x24, 0x18, 0x18, 0x24, 0x42, 0x81,	// 两个靠拢后分开
	0x81, 0xC3, 0xE7, 0xFF, 0xFF, 0xE7, 0xC3, 0x81	// 从两边叠加后递减
};
#define SPI_CS       	(1 << 16) 
#define CE_Low()		(IO0CLR |=  SPI_CS)
#define CE_High()		(IO0SET |=  SPI_CS)			

void SPITest()
{
	uint32 i = 0;
	PINSEL0  =	0;
	//UARTInit(0,9600);		
   	IO0DIR 	|=  SPI_CS;		// 设置SPI的片选引脚
   	SPIInit();
	while (1)	
	{ 
    	for (i = 0; i < 42; i++)
	  	{   
	  		GpioClrLOW(P0_16);
			//CE_Low();
			SPISendByte(LED_TBL[i]);
			//CE_High();
			GpioSetLOW(P0_16);
			Delay();
	  	} 
    }
}

extern uint8 F,C[3];

void SPIUART()
{
	//uint32 i = 0;
	//uint8 c=0;
	PINSEL0  =	0;
	UARTInit(0,115200);
	PrintUART0("@@");
   	IO0DIR 	|=  SPI_CS;		// 设置SPI的片选引脚
   	SPIInit();
	while (1)
	{ 
		if(F==1)
	  	{
	  		/*
	  		if(C==0xF0)
	  		{
	  			GpioClrLOW(P0_16);
		  		c=SPISendByte(C);
	  			UARTSendChar(0,c);
		  		c=SPISendByte(0);
	  			UARTSendChar(0,c);
		  		c=SPISendByte(0);
	  			UARTSendChar(0,c);
	  			GpioSetLOW(P0_16);
	  		}
	  		else
	  		{
				GpioClrLOW(P0_16);
	  			c=SPISendByte(C);
	  			GpioSetLOW(P0_16);
				UARTSendChar(0,c);
			}
			F=0;
			*/
	  	} 
	  	//Delay();
    }
}

void SSPUART()
{
	//uint8 c=0;
	//UARTInit(0,115200);
	//PrintUART0("@@");
	SSP0FPGAMode();
   	//SSPInit(0,240,0|CPHA|CPOL);
   	//GpioSetLOW(P0_23);
	//GpioSetLOW(P0_24);
	//GpioSetLOW(P0_29);
	while (1)
	{
		uint8 t=0;
		uint16 r=0x0147*3;
		r=FPGACheck();
		for(t=0;t<250;t++);
		for(t=0;t<250;t++);
		for(t=0;t<250;t++);
		//FPGASetStep(t,r);
		//r=FPGAReadStep(t);
		//r=FPGAReadStep(t);
    }
}

#define Flash0_Down GpioClrHI(P2_03)
#define Flash0_Up GpioSetHI(P2_03)
#define Flash1_Down GpioClrHI(P2_03);GpioSetHI(P2_02)
#define Flash1_Up GpioSetHI(P2_03);GpioClrHI(P2_02)

void FlashUART()
{
	uint16 c=0,i;
	//uint8 t;
	uint8 u[25],n[50] = {0};
	for(c=0;c<25;c++)
		u[c]=c;
	//FlashChipSelect(1);
	//PageModeSwitch();
	i=FlashInit(30);
	FlashChipSelect(1);
	//UARTSendChar(0,i);
	
	while(1)
	{
		Delay();
		//i = BlockErase(0);
		//UARTSendChar(0,i);
		//i = PageErase(0);	
		//UARTSendChar(0,i);
		//i = BufferWrite(0, u, 10);
		//UARTSendChar(0,i);
		//i = PageWrite(0);
		//UARTSendChar(0,i);
		//i = PageErase(0);
		//UARTSendChar(0,i);
		//i = SetAddress(0);
		//UARTSendChar(0,i);
		//for (j = 0;j<3;j++)
		//{
			//i =  StreamQueue(u,10);
			//UARTSendChar(0,i);
		//}
		//i = PageRead(0);
		//UARTSendChar(0,i);
		//i = BufferRead(0,n, 10);
		//UARTSendChar(0,i);
		//for(c=0;c<10;c++)
		//	UARTSendChar(0,n[c]);
		ChangeWritePageNo(0);
		for(c=0;c<300;c++)
		{
			StreamWrite(u,25);
		}
		StreamWriteForce();
		//PageErase(0);
		ChangeReadSet(0,0);
		for(c=0;c<150;c++)
		{
			StreamRead(n,50);
			UARTSend(0,n,50);
		}
		while(1);
    } 
}

void SSPTest()
{
	uint32 i = 0;
	SSPInit(0,2,0|CPHA);
	while (1)	
	{ 
		for (i = 0; i < 42; i++)
		{   
			SSPSendByte(0,LED_TBL[i]);	
			Delay();
		} 
    }
}

void ADUART()
{
	UARTInit(0,9600);
	PrintUART0("@@");
	SSP0ADMode();
	/*
	GpioClrLOW(P0_23);GpioClrLOW(P0_24);GpioClrLOW(P0_29);
	GpioSetLOW(P0_23);GpioClrLOW(P0_24);GpioClrLOW(P0_29);
	GpioClrLOW(P0_23);GpioSetLOW(P0_24);GpioClrLOW(P0_29);
	GpioSetLOW(P0_23);GpioSetLOW(P0_24);GpioClrLOW(P0_29);
	GpioClrLOW(P0_23);GpioClrLOW(P0_24);GpioSetLOW(P0_29);
	GpioSetLOW(P0_23);GpioClrLOW(P0_24);GpioSetLOW(P0_29);
	GpioClrLOW(P0_23);GpioSetLOW(P0_24);GpioSetLOW(P0_29);
	GpioSetLOW(P0_23);GpioSetLOW(P0_24);GpioSetLOW(P0_29);	
	*/
	
	while (1)
	{
		//int i;
		if(F==1)
	  	{
		  	F=0;
	  		if(C[0]==0xF0)
	  		{
	  			uint8 k;
		  		uint16 t=ADReadCode(C[1],C[2]);
				k=(uint8)(t>>8);
				UARTSendChar(0,k);
				k=(uint8)t;
				UARTSendChar(0,k);

	  		}
			if(C[0]==0xFF)
			{
				while(F==0)
				{
					int16 t;
					char TmpStr[10]="";
					t=ADReadCode(C[1],C[2]);
					t=2048+t*2048/0x8000;
					sprintf(TmpStr,"%d\r\t",t);
					UARTSend(0, (uint8*)TmpStr, strlen(TmpStr));
					//UARTSendChar(0, 0xAA);
			  		//UARTSend(0,(uint8*)&t,2);
			  		Delay();
				}
			}
	  	}
    }
}

void DAUART()
{
	UARTInit(0,9600);
	PrintUART0("@@");
	SSP0DAMode();
	//DAOutEnable();
	while (1)
	{
		//int i;
		if(F==1)
	  	{
		  	F=0;
			if(C[0]==0xF0)
	  		{
				DASetCode(0,0x7FF);				
	  		}
	  		if(C[0]==0xF1)
	  		{
				DASetCode(0, (C[1]<<8)|C[2]);				
	  		}
	  		if(C[0]==0xF2)
	  		{
		  		SSP0ADMode();
				SSP0DAMode();
	  		}
			if(C[0]==0xFF)
			{
				while(F==0)
				{
					UARTSendChar(0,0xFF);
					DASetCode(0, 0xFFF);
			  		Delay();
				}
			}
	  	}
    }
}

void CANTest()
{
	unsigned char data11[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	CANInit(0,BPS_1000K);                        // 初始化第1路CAN，波特率为1000K
	CANInit(1,BPS_1000K);                         // 初始化第2路CAN，波特率为500K
	
	/* 使用CAN1 通道时 注意硬件是否连接,通道是否已被初始化 */
	while(1)
	{
		LED1ON();
		CANSendMessage(0,1,1,1,1,8,data11);// 中断处理:CANIRQ()函数中完成
		//BeepON();
		Delay();
		LED1OFF();
		Delay();
		//BeepOFF();
	}
}

void CANUART()
{
	//uint8 i=0;
	//uint8 data11[]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
	//LED1ON();
	//Delay();
	//LED1OFF();
	CANInit(0,BPS_1000K);// 初始化第1路CAN，波特率为1000K
	//CANInit(1,BPS_1000K);// 初始化第2路CAN，波特率为1000K
	UARTInit(0,9600);
	PrintUART0("@@");
	//Delay();
	//LED1ON();
	//Delay();
	//LED1OFF();
	//TimerInit(0,2000);
	//TimerEnable(0);
	
	while (1)
	{
		//if(F==1)
	  	//{
		  	//F=0;
		  	//CANSendMessage(0,i,C[0],C[1],C[2],3,C);
		  	//CANSendMessage(0,1,1,1,1,8,data11);
			//UARTSendChar(0,0xAA);
		  	//DelayMs(0,5000);
		  	//UARTSendChar(0,0xAA);
		  	//Delay();
		  	//i++;
	  		Delay();
	  	//}
    }
}

#endif

