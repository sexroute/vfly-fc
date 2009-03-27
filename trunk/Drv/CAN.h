#ifndef _CAN_H_
#define _CAN_H_

//CAN总线速率宏定义(适用于48MHz主频下)
#define BPS_1000K	0x140002
#define BPS_800K	0x160002
#define BPS_500K	0x1C0002
#define BPS_250K	0x1C0005
#define BPS_125K	0x1C000B
#define BPS_100K	0x1C000E
#define BPS_50K		0x1C001D
#define BPS_20K		0x1C004A
#define BPS_10K		0x1C0095

//寄存器操纵宏
#define CANOFFSET 0x4000
#define RGE(ADDR) (*((volatile uint32*)ADDR))

typedef struct _MessageDetail
{
	uint8 LEN;
	uint8 FF;// 是否标准帧
	uint32 CANID;// 长度与FF有关
	uint32 DATAA;
	uint32 DATAB;
}MessageDetail;


//定义 CANMOD 寄存器:
//This register controls the basic operating mode of the CAN Controller. 
//Bits not listed read as 0 and should be written as 0.
typedef	union _canmod_
{
	uint32 DWord;
	struct
	{
		uint32	RM 		:1;//定义RM位(复位位)
		uint32	LOM 	:1;//定义LOM位(只听模式位)
		
		//"1"自测试模式,CANCMR的SRR位一起使用，可以达到自发自收的效果
		uint32	STM		:1;//定义STM位 "0"发送的信息必须被应答才被认可
		
		//"1"3个发送缓冲区的优先级由各自的Tx优先级域决定
		uint32	TPM 	:1;//定义TPM位 "0"3个发送缓冲区的优先级由各自的CAN ID决定
		uint32	SM 		:1;//定义SM位(睡眠模式位)
		
		//"1"翻转极性模式--如果传输的是显性位，Rx脚为高电平
		uint32	RPM 	:1;//定义RPM位 "0"如果传输的是显性位，Rx和Tx脚为低电平
		uint32	RSV1 	:1;//保留位
		uint32	TM 		:1;//定义TM位(测试模式)
		uint32	RSV24	:24;	//保留位
	}Bits;
}REG_CANMOD;

//定义 CAN 帧结构体:
typedef struct _CANFRMAE
{
	uint32 ID;//帧ID
	uint8 LEN;//数据长度
	uint8 RTR;//远程帧标志
	uint8 FF;//扩展帧标志
	uint8 DATA[8];//帧数据
}CANFRAME;

uint8 CANInit(uint8 Ch, uint32 Baud);
uint8 CANReceiveFrame(uint8 Ch, MessageDetail *MessageCAN);
uint8 CANWriteFrame(uint8 LEN,uint8 FF, uint32 ID,uint8 *data,MessageDetail *CANMessage);
uint8 CANSendFrame(uint8 Ch, uint8 BufNum,MessageDetail *CANMessage);
uint8 CANSendMessage(uint8 Ch,uint8 ID,uint8 Source, uint8 Target,uint8 Word,uint8 Len,uint8 *data);

#endif

