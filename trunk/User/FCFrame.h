#ifndef __FCFrame_
#define __FCFrame_

#define FCFrameS1 0xA5
#define FCFrameS2 0x5A
#define FCFrameLength 6
#define FCBlockLength 15
#define FCRecFrameS1 0xA5
#define FCRecFrameS2 0x5A

//定义FC基本帧结构(__packed表示取消对齐)
__packed struct _FCFrameStruct
{
	uint8 S1,S2,Source,Target,Reserve,BlockNum;
};
typedef struct _FCFrameStruct FCFrameStruct;


//定义FCA基本帧结构(__packed表示取消对齐)
struct _FCBlockStruct
{
	fp32 X,Y,Z;
};
typedef struct _FCBlockStruct FCBlockStruct;

uint8 FCMessageSend(const char *str);
uint8 FCErrorSend(uint8 ErrorID,uint8 p1,uint8 p2,uint8 p3,const char *str);
uint8 FCEventSend(uint8 EventID);
void FCFrameProcess(void);
void DataReturn(uint8  Part);
void DataDownload(void);

#endif

