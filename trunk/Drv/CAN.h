#ifndef _CAN_H_
#define _CAN_H_

//CAN�������ʺ궨��(������48MHz��Ƶ��)
#define BPS_1000K	0x140002
#define BPS_800K	0x160002
#define BPS_500K	0x1C0002
#define BPS_250K	0x1C0005
#define BPS_125K	0x1C000B
#define BPS_100K	0x1C000E
#define BPS_50K		0x1C001D
#define BPS_20K		0x1C004A
#define BPS_10K		0x1C0095

//�Ĵ������ݺ�
#define CANOFFSET 0x4000
#define RGE(ADDR) (*((volatile uint32*)ADDR))

typedef struct _MessageDetail
{
	uint8 LEN;
	uint8 FF;// �Ƿ��׼֡
	uint32 CANID;// ������FF�й�
	uint32 DATAA;
	uint32 DATAB;
}MessageDetail;


//���� CANMOD �Ĵ���:
//This register controls the basic operating mode of the CAN Controller. 
//Bits not listed read as 0 and should be written as 0.
typedef	union _canmod_
{
	uint32 DWord;
	struct
	{
		uint32	RM 		:1;//����RMλ(��λλ)
		uint32	LOM 	:1;//����LOMλ(ֻ��ģʽλ)
		
		//"1"�Բ���ģʽ,CANCMR��SRRλһ��ʹ�ã����Դﵽ�Է����յ�Ч��
		uint32	STM		:1;//����STMλ "0"���͵���Ϣ���뱻Ӧ��ű��Ͽ�
		
		//"1"3�����ͻ����������ȼ��ɸ��Ե�Tx���ȼ������
		uint32	TPM 	:1;//����TPMλ "0"3�����ͻ����������ȼ��ɸ��Ե�CAN ID����
		uint32	SM 		:1;//����SMλ(˯��ģʽλ)
		
		//"1"��ת����ģʽ--��������������λ��Rx��Ϊ�ߵ�ƽ
		uint32	RPM 	:1;//����RPMλ "0"��������������λ��Rx��Tx��Ϊ�͵�ƽ
		uint32	RSV1 	:1;//����λ
		uint32	TM 		:1;//����TMλ(����ģʽ)
		uint32	RSV24	:24;	//����λ
	}Bits;
}REG_CANMOD;

//���� CAN ֡�ṹ��:
typedef struct _CANFRMAE
{
	uint32 ID;//֡ID
	uint8 LEN;//���ݳ���
	uint8 RTR;//Զ��֡��־
	uint8 FF;//��չ֡��־
	uint8 DATA[8];//֡����
}CANFRAME;

uint8 CANInit(uint8 Ch, uint32 Baud);
uint8 CANReceiveFrame(uint8 Ch, MessageDetail *MessageCAN);
uint8 CANWriteFrame(uint8 LEN,uint8 FF, uint32 ID,uint8 *data,MessageDetail *CANMessage);
uint8 CANSendFrame(uint8 Ch, uint8 BufNum,MessageDetail *CANMessage);
uint8 CANSendMessage(uint8 Ch,uint8 ID,uint8 Source, uint8 Target,uint8 Word,uint8 Len,uint8 *data);

#endif

