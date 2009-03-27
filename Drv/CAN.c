/*
	CAN.c
	CAN�����շ���������
	�ṩ��ʼ��������֡����(Ⱥ��)���жϷ�ʽ���ա�����ID�����ݽ�����
	�޸������������ʾ������
	BUAA 2008-7-23
	���Գ�
*/
#include "Config.h"
#include "Drv.h"
#include "CAN.h"

MessageDetail TempFrame;//����֡
MessageDetail CAN0Frame;//CAN0����֡
MessageDetail CAN1Frame;//CAN1����֡

uint8 EnterSWReset(uint8 Ch);
uint8 QuitSWReset(uint8 Ch);
void __irq CANIRQ(void);

//��ʼ��CAN���߿�����
//ChΪ0-1
//Baud(����)ʹ��CAN.h�е��Ѷ���ֵ
uint8 CANInit(uint8 Ch, uint32 Baud)
{
	uint32 regaddr;
	
	switch(Ch)//����CAN����������
	{
		case 0:
			PCONP |= 0x01L<<13;//��CAN1��������Դ
			PINSEL0 &= ~(0x03L<<0);//RD1
			PINSEL0 |= (0x01L<<0);
			PINSEL0 &= ~(0x03L<<2);//TD1
			PINSEL0 |= (0x01L<<2);
			break;
		case 1:
			PCONP |= 0x01L<<14;//��CAN1��������Դ
			PINSEL0 &= ~(0x03L<<8);//RD2
			PINSEL0 |= (0x02L<<8);
			PINSEL0 &= ~(0x03L<<10);//TD2
			PINSEL0 |= (0x02L<<10);
			break;
		default:
			break;
	}
	
	EnterSWReset(Ch);
	
	regaddr = (uint32)(&CAN1BTR)+Ch*CANOFFSET;
	RGE(regaddr) = Baud;
	
	CAN_AFMR &= ~0x07;//���չ�������Ϊ��·ģʽ
	CAN_AFMR|=(1<<1);

	if(Ch == 0)
	{
		CAN1CMR |= (0x01<<2);//�ͷŽ��ջ�����
		CAN1ICR &= ~(0x01);
		CAN1IER |= ((1<<0)|(1<<2));//ʹ��CAN0��������󱨾��ж�
	}
	else
	{
		CAN2CMR |= (0x01<<2);
		CAN2ICR &= ~(0x01);
		CAN2IER |= ((1<<0)|(1<<2));//ʹ��CAN1��������󱨾��ж�
	}

	if(InstallIRQ(CAN_INT,(void *)CANIRQ,HIGHEST_PRIORITY)==FALSE)
	{
		return FALSE;
	}

	QuitSWReset(Ch);
	
	return TRUE;
}

//CAN���������������λģʽ
uint8 EnterSWReset(uint8 Ch)
{
	uint32 regaddr;
	REG_CANMOD regmod;
	regaddr = (uint32)(&CAN1MOD)+Ch*CANOFFSET;			
	regmod.DWord = RGE(regaddr);//��ȡCAN1MOD�Ĵ���
	regmod.Bits.RM = 1;//RMλ�á�1��
	RGE(regaddr) = regmod.DWord;//��дCAN1MOD�Ĵ���
	regmod.DWord = RGE(regaddr);//��֤д��ֵ	
	return (0 != regmod.Bits.RM)? TRUE:FALSE; 
}

//CAN�������˳������λģʽ
uint8 QuitSWReset(uint8 Ch)
{
	uint32 regaddr;
	REG_CANMOD regmod;
	regaddr = (uint32)(&CAN1MOD)+Ch*CANOFFSET;			
	regmod.DWord = RGE(regaddr);//��ȡCAN1MOD�Ĵ���
	regmod.Bits.RM = 0;//RMλ�á�0��
	RGE(regaddr) = regmod.DWord;//��дCAN1MOD�Ĵ���
	regmod.DWord = RGE(regaddr);//��֤д��ֵ	
	return (0 != regmod.Bits.RM)? FALSE:TRUE; 
}

//����CAN֡�������ָ���Ľṹ���� 
uint8 CANReceiveFrame(uint8 Ch, MessageDetail *Frame)
{
	uint32 mes;
	uint32 regaddr;

	regaddr = (uint32)(&CAN1RFS)+Ch*CANOFFSET;//����DLC��RTR��FF
	mes = RGE(regaddr);
	Frame->LEN =  (mes >>16) & 0x0F;//��ȡ֡����
	Frame->FF  =  (mes >>31);//��ȡFF

	regaddr = (uint32)(&CAN1RID)+Ch*CANOFFSET;//����ID
	mes = RGE(regaddr);
	
	if(Frame->FF)//FFΪ1��IDΪ29λ
	{
		Frame->CANID = mes & 0x1fffffff;
	}
	else//FFΪ0 ��IDΪ11λ
	{	
		Frame->CANID = mes & 0x000007ff;
	}
	
	regaddr = (uint32)(&CAN1RDA)+Ch*CANOFFSET;//���� ����A
	mes = RGE(regaddr);
	Frame->DATAA = mes; 
   
	regaddr = (uint32)(&CAN1RDB)+Ch*CANOFFSET;//���� ����B
	mes = RGE(regaddr);
	Frame->DATAB = mes;

	return TRUE;
}

//��ָ��֡�ṹ����д������
//LEN���ݳ���(����<=8��ʣ�ಹ��)
//FF��չ֡ģʽ���ڴ�������ΪTRUE:1��
//ID29λ���ݣ�Լ��Ϊ5λѭ��ID+8λԴ��ַ+8λĿ���ַ+8λ���������δӸߵ��͡�
//data֡����(8byte)
//FrameĿ������֡
uint8 CANWriteFrame(uint8 LEN,uint8 FF, uint32 ID,uint8 *data,MessageDetail *Frame)
{
	uint8 i;
	
	if(LEN>8)
	{
		return FALSE;
	}
	else
	{
		Frame->LEN=LEN;
		Frame->FF=FF;
		Frame->CANID=ID;
		Frame->DATAA=0;//������
		Frame->DATAB=0;

		//�˴���������˳��
		for(i=0;i<LEN;i++)
		{
			if(i<4)
			{
				Frame->DATAA |= (*(data+i)<<(i*8)); 
			}
			else
			{
				Frame->DATAB |= (*(data+i)<<((i-4)*8));
			}
		}
	//Frame->DATAB = ID;	
	return TRUE;
	}
}

//����ָ��֡
//BufNum������ʹ�õĻ���ţ�һ���Խ������ͨ���š�
//Frame������֡
uint8 CANSendFrame(uint8 Ch, uint8 BufNum,MessageDetail *Frame)
{
	uint32 CAN32reg;
	uint32 regaddr;
	uint8 FFflag,t;
	
	//BufNum-=1;		// �����ַ���㡣//TODO:DEL
	
	regaddr = (uint32)(&CAN1TFI1)+Ch*CANOFFSET+BufNum*0X10;// 3���������ַ�� 0x10
	CAN32reg = RGE(regaddr);
	CAN32reg &= ~((0x0fL<<16) | (0x01L<<30) | (0x80000000));//�� DLC,RTR.FFλ
	CAN32reg |= ((uint32)(*Frame).LEN<<16) | ((uint32)(*Frame).FF<<31);
	RGE(regaddr)=CAN32reg;
	
	FFflag = (*Frame).FF;
	
	regaddr = (uint32)(&CAN1TID1)+Ch*CANOFFSET+BufNum*0X10;//д֡ID
	CAN32reg = RGE(regaddr);
	if(FFflag)//FFΪ1��IDΪ29λ
	{
		CAN32reg &=0x70000000;
		CAN32reg |= ((*Frame).CANID & 0x1fffffff);
	}
	else
	{
		CAN32reg &= 0xfffff800;//FFΪ0 ��IDΪ11λ
		CAN32reg |= ((*Frame).CANID & 0x000007ff);
	}
	RGE(regaddr)=CAN32reg;
	
	regaddr = (uint32)(&CAN1TDA1)+Ch*CANOFFSET+BufNum*0X10;// д֡����A
	RGE(regaddr) = (*Frame).DATAA;
	
	regaddr = (uint32)(&CAN1TDB1)+Ch*CANOFFSET+BufNum*0X10;// д֡����B
	RGE(regaddr) = (*Frame).DATAB;
	
	regaddr = (uint32)(&CAN1CMR)+Ch*CANOFFSET;//д���ƼĴ���������
	CAN32reg = RGE(regaddr);
	
	CAN32reg &= ~0x03;
	CAN32reg |= 0x03;
	CAN32reg &= ~((1<<5)|(1<<6)|(1<<7));
	switch(BufNum)
	{
		case 0: CAN32reg |= 01<<5;break;
		case 1: CAN32reg |= 01<<6;break;
		case 2: CAN32reg |= 01<<7;break;
		default: break;
	}
	RGE(regaddr)=CAN32reg;
	
	regaddr = (uint32)(&CAN1GSR)+Ch*CANOFFSET;//��ѯ����״̬
	CAN32reg = RGE(regaddr);
	
	//IRQDisable();
	//r=FALSE;
	for(t=0;t<100;t++)
	{
		CAN32reg=RGE(regaddr);
		if(CAN32reg&(1<<3))
		{
			//r=TRUE;
			break;
		}
	}
	//IRQEnable();
	return TRUE;
}

//��ָ����ID��ʽ������Ϣ(�Ƽ�ʹ�ô˺������������ݣ��Լ��ٶ�����������ֱ�ӵ��á�)
//֡IDԼ��Ϊ5λѭ��ID+8λԴ��ַ+8λĿ���ַ+8λ���������δӸߵ��͡�
//IDѭ��ID
//SourceԴ�豸��ַ
//TargetĿ���豸��ַ
//Word������
//Len���ݳ���(����<=8)
//data�����͵�����
uint8 CANSendMessage(uint8 Ch,uint8 ID,uint8 Source, uint8 Target,uint8 Word,uint8 Len,uint8 *data)
{
	uint32 t;
	MessageDetail* SendFrame;

	if(Ch>1||ID>0x1F||Len>8||data==NULL)
	{
		t=FALSE;
	}
	else
	{
		ID=ID&0x1F;
		t=(ID<<24)|(Source<<16)|(Target<<8)|(Word<<0);

		SendFrame=&TempFrame;
		//if(Ch==1)
		//{
		//	SendFrame=&CAN1Frame;
		//}
		if(CANWriteFrame(Len, 1, t, data, SendFrame)==TRUE)
		{
			t=FALSE;
			t=CANSendFrame(Ch,Ch,SendFrame);
		}
	}
	return t;
}

//���յ�CAN����������֡ʱ�����õ�Message������(��Ҫ�ڴ˺�����ִ�����ڹ����Ĳ���)
//IDѭ��ID
//SourceԴ�豸��ַ
//TargetĿ���豸��ַ
//Word������
//Len���ݳ���(<=8)
//DATAA��8λ����
//DATAB��8λ����(����μ����͹���)
//������֡Ϊ����չ֡(��FF=0)ʱ�˺������ᱻ���á�
void CANProcessMessage(uint8 Ch,uint8 ID,uint8 Source, uint8 Target,uint8 Word,uint8 Len,uint32 DataA,uint32 DataB)
{
}

//CAN�������жϴ�����
void __irq CANIRQ()
{
 	uint8 ch;
	uint32 CAN32reg,mes;
	uint32 regaddr;
	//IRQDisable();

	for(ch=0;ch<2;ch++)//���CANͨ����Ϊ2���ֱ�������ͨ�����ж� 
	{
		regaddr = (uint32)(&CAN1ICR)+ch*CANOFFSET;	
		CAN32reg=RGE(regaddr);	
		
		if((CAN32reg&(1<<0))!= 0)	//RI �����ж�
		{
			MessageDetail* ReceiveFrame;
			
			CAN32reg &= ~(0x01);
			RGE(regaddr) = CAN32reg;

			ReceiveFrame=&CAN0Frame;
			if(ch==1)
			{
				ReceiveFrame=&CAN1Frame;
			}

			CANReceiveFrame(ch, ReceiveFrame);//�յ�CAN0�ж�,����֡
			
			
			//Test Code
			if(ch==0)
			{
				UARTSendChar(0,0xCC);
				//CANSendFrame(0,0,ReceiveFrame);
				
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>24));
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>16));
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>8));
				UARTSendChar(0,(uint8)(ReceiveFrame->CANID>>0));
				
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>24));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>16));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>8));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAB>>0));
				
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>24));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>16));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>8));
				UARTSendChar(0,(uint8)(ReceiveFrame->DATAA>>0));
				
			}
			else
			{
				CANSendFrame(1,1,ReceiveFrame);
			}
			

			if(ReceiveFrame->FF==1)
			{
				uint8 ID,Source,Target,Word;
				uint32 DATAA,DATAB;
				
				ID=ReceiveFrame->CANID>>24;
				ID=ID&0x1F;
				Source=ReceiveFrame->CANID>>16;
				Target=ReceiveFrame->CANID>>8;
				Word=ReceiveFrame->CANID>>0;
				DATAA=ReceiveFrame->DATAA;
				DATAB=ReceiveFrame->DATAB;
				
				CANProcessMessage(ch, ID,Source,Target,Word, ReceiveFrame->LEN, DATAA,DATAB);
			}
			
			if(ch==0)
			{
				regaddr = (uint32)(&CAN1CMR)+ch*CANOFFSET;				
				mes=RGE(regaddr);
				mes |= (1<<2);//�ͷŽ��ջ�����
				RGE(regaddr)=mes;
			}
			if(ch==1)
			{
				regaddr = (uint32)(&CAN1CMR)+ch*CANOFFSET;				
				mes=RGE(regaddr);
				mes |= (1<<2);//�ͷŽ��ջ�����
				RGE(regaddr)=mes;
			}
		}
		//������Ҫ���
		/*
		if((CAN32reg&(1<<1))!= 0)// TI1 ��һ���ͻ�������������ж�

		{
			;
		}
		if((CAN32reg&(1<<9))!= 0)// TI2 �ڶ����ͻ�������������ж�

		{
			;
		}
		if((CAN32reg&(1<<10))!= 0)// TI3 �������ͻ�������������ж�
		{
			;
		}
		*/
		if((CAN32reg&(1<<7))!= 0)	// BEI ���ߴ����ж�
		{
			EnterSWReset(ch);
			regaddr = (uint32)(&CAN1GSR)+ch*CANOFFSET;				
			mes=RGE(regaddr);
			mes &=0x00ff;												
			RGE(regaddr)=mes;//���ߴ����������
			QuitSWReset(ch);
		}
		if((CAN32reg&(1<<6))!= 0)	// ALI �ٲö�ʧ�ж�
		{
			//����û�����
		}
		if((CAN32reg&(1<<3))!= 0)
		{
			//����û�����
		}
	}
	//IRQEnable();
	VICVectAddr = 0;	
}

